from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    try:
        # extract all necessary data
        monies = db.execute("SELECT cash FROM users WHERE id= :idd", idd=session["user_id"])
        stocks = db.execute("SELECT symbol, units FROM portfolio WHERE id= :idd", idd=session["user_id"])

        # initialize variable to store grand total
        for i in monies:
            grandTotal = i["cash"]

        # iterate over each dict in the list, lookup and store info into dict
        for li in stocks:
            quote = lookup(li["symbol"])
            li["name"] = quote["name"]
            li["price"] = quote["price"]
            li["total"] = quote["price"] * li["units"]
            grandTotal += li["total"]

        # convert to USD
        for i in range(0, len(monies)):
            monies[i]["cash"] = usd(monies[i]["cash"])

        for i in range(0, len(stocks)):
            stocks[i]["price"] = usd(stocks[i]["price"])
            stocks[i]["total"] = usd(stocks[i]["total"])


        # Pass info into index.html
        return render_template("index.html", stocks=stocks, monies=monies, grandTotal=usd(grandTotal))

    except:
        return apology("Error!")

    return apology("Fatal error!")

@app.route("/reset", methods=["GET", "POST"])
@login_required
def reset():
    """Change your password."""

    # if user submits a change password query
    if request.method == "POST":

        # check all fields are filled
        if not request.form.get("oldPw") or not request.form.get("newPw") or not request.form.get("newPw_copy"):
            return apology("Fill all fields!")

        # check new passwords match
        if request.form.get("newPw") != request.form.get("newPw_copy"):
            return apology("New passwords do not match!")

        # check old passwords match. If true, change passwords and update db
        pw_list = db.execute("SELECT hash FROM users WHERE id = :idd", idd=session["user_id"])

        if pwd_context.verify(request.form.get("oldPw"), pw_list[0]["hash"]):
            db.execute("UPDATE users SET hash = :hash WHERE id = :idd", hash=pwd_context.hash(request.form.get("newPw")), idd=session["user_id"])
            flash("Password updated successfully!")
            return redirect(url_for("reset"))

        else:
            return apology("Incorrect password!")

    # if the user reached the route via GET
    else:
        return render_template("reset.html")

    return apology("Fatal Error!")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reached route via POST
    if request.method == "POST":

        # check for valid input & number of shares
        if not request.form.get("stocksym") or not request.form.get("stocknum"):
            return apology("Fill all fields!")

        elif not request.form.get("stocknum").isdigit():
            return apology("Only positive whole numbers allowed!")

        # check if stock quote is valid
        else:
            quote = lookup(request.form.get("stocksym"))

            if not quote:
                return apology("Invalid stock quote!")

            # check if user has enough money to make purchase
            # if true, make the purchase. Else apologise
            outrows = db.execute("SELECT cash FROM users WHERE id = :idd", idd=session["user_id"])
            totalPurchase = int(request.form.get("stocknum")) * quote["price"]

            if totalPurchase < outrows[0]["cash"]:
                # update database
                db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :idd", purchase=totalPurchase, idd=session["user_id"])
                rowsUpdated = db.execute("UPDATE portfolio SET units = units + :newUnits, price = :price, total = :total WHERE id=:idd AND symbol = :sym",\
                                                            newUnits = int(request.form.get("stocknum")),\
                                                            price = quote["price"],\
                                                            total = totalPurchase,\
                                                            idd = session["user_id"],\
                                                            sym = request.form.get("stocksym"))
                # if no rows are updated, create a new entry
                if rowsUpdated < 1:
                    db.execute("INSERT INTO portfolio(id, symbol, stockname, units, price, total) VALUES(:id,:sym,:sto,:uni,:pri,:tot)",\
                                                            id = session["user_id"],\
                                                            sym = request.form.get("stocksym"),\
                                                            sto = quote["name"],\
                                                            uni = int(request.form.get("stocknum")),\
                                                            pri = quote["price"],\
                                                            tot = totalPurchase)

                # log transaction history
                db.execute("INSERT INTO transactions(Symbol, Shares, Price, id) VALUES(:sym,:sha,:pri,:idd)",\
                                                            sym = request.form.get("stocksym"),\
                                                            sha = int(request.form.get("stocknum")),\
                                                            pri = quote["price"],\
                                                            idd = session["user_id"])

                flash("Purchase successful!")
                return redirect(url_for("index"))
            else:
                return apology("You can't go into debt...")

    else:
        return render_template("buy.html")

    return apology("Fatal error!")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # if user reached route via GET
    if request.method == "GET":

        # query database for data
        table = db.execute("SELECT Symbol, Shares, Price, Transacted FROM transactions WHERE id=:idd", idd=session["user_id"])

        # convert to usd
        for i in range(0, len(table)):
            table[i]["Price"] = usd(table[i]["Price"])

        # display info as a table
        return render_template("history.html", table=table)

    else:

        return apology("Error!")

    return apology("Fatal Error!")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If user submitted form via POST (wants a quote)
    if request.method == "POST":

        # returns a dict called quote
        quote = lookup(request.form.get("quote"))

        # convert to usd
        quote["price"] = usd(quote["price"])

        # ensure stock is valid (filled)
        if not quote:
            return apology("Sorry, quote failed!")

        else:
            return render_template("quoted.html", **quote)

    # if user visited /quote via GET
    else:
        return render_template("quote.html")

    return apology("Fatal error!")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user id
    session.clear()

    # if user submitted form via POST
    if request.method == "POST":

        # ensure username is filled
        if not request.form.get("username"):
            return apology("Missing username!")

        # ensure passwords match and is filled
        if not request.form.get("password") or not request.form.get("password_copy"):
            return apology("Missing password!")

        elif request.form.get("password") != request.form.get("password_copy"):
            return apology("Passwords do not match!")

        # hash password and insert into table, ensuring username is unique
        hashpw = pwd_context.hash(request.form.get("password"))

        result = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",
                username=request.form.get("username"), hash=hashpw)
        if not result:
            return apology("Username already taken!")

        # log user in automatically if successfully registered
        try:
            session["user_id"] = result
            flash("You were successfully registered!")
            # redirect user to home page
            return redirect(url_for("index"))
        except:
            return apology("Something happened :/")
    # else if user reached route via GET (clicking a link)
    else:
        return render_template("register.html")

    # if any other error occurs:
    return apology("Fatal error!")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # display a table of the user's current stocks
    try:
    # extract all necessary data
        monies = db.execute("SELECT cash FROM users WHERE id= :idd", idd=session["user_id"])
        stocks = db.execute("SELECT symbol, units FROM portfolio WHERE id= :idd", idd=session["user_id"])

        # iterate over each dict in the list, lookup and store info into dict
        for li in stocks:
            quote = lookup(li["symbol"])
            li["name"] = quote["name"]
            li["price"] = usd(quote["price"])
            li["total"] = usd(quote["price"] * li["units"])

    except:
        return apology("Error!")

    # if user submits a sale
    if request.method == "POST":

        # ensure fields are filled
        if not request.form.get("stocksym") or not request.form.get("units"):
            return apology("Please fill all fields!")

        # ensure units are positive integers
        if not request.form.get("units").isdigit():
            return apology("Only positive whole numbers allowed!")

        # lookup stock user wishes to sell
        sellQuote = lookup(request.form.get("stocksym"))

        # calculate total sales
        totalSales = sellQuote["price"] * int(request.form.get("units"))

        # log sales and change in price
        db.execute("UPDATE users SET cash = cash + :sales WHERE id = :idd", idd=session["user_id"], sales=totalSales)
        db.execute("UPDATE portfolio SET units = units - :soldUnits, price = :newPrice, total = :total WHERE symbol = :sym AND id = :idd", \
                                                                        sym=request.form.get("stocksym"), \
                                                                        idd=session["user_id"], \
                                                                        total=totalSales, \
                                                                        newPrice=sellQuote["price"], \
                                                                        soldUnits=int(request.form.get("units")))
        # log transaction history
        db.execute("INSERT INTO transactions(Symbol, Shares, Price, id) VALUES(:sym,:sha,:pri,:idd)",\
                                                            sym = request.form.get("stocksym"),\
                                                            sha = -int(request.form.get("units")),\
                                                            pri = quote["price"],\
                                                            idd = session["user_id"])

        flash("Sale complete!")
        return redirect(url_for("index"))

    # else if user reached route via GET
    else:
        return render_template("sell.html", stocks=stocks, monies=monies)

    return apology("Fatal error!")
