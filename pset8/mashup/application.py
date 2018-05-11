import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo."""

    geoArg = request.args.get("geo")

    # Ensure geo is defined
    if not geoArg:
        raise RuntimeError("No geo argument")

    # lookup articles
    else:
        arti = lookup(geoArg)

    return jsonify(arti)

@app.route("/search")
def search():
    """Search for places that match query."""

    q = request.args.get("q")
    print(q)

    # Ensure q is filled
    if not q:
        raise RuntimeError("No q argument")

    # Split string and add wildcard character
    q = q.split(", ")
    query = [value + "%" for value in q]
    print(query)

    # Check database for matches (postal code if 1 arg)
    if len(q) == 1:
        rows = db.execute("SELECT * FROM places WHERE postal_code LIKE :q", q = query[0])

        # Check city name if no return value
        if not rows:
            rows = db.execute("SELECT * FROM places WHERE place_name LIKE :q", q = query[0])

        return jsonify(rows)

    # Check place name, admin name, admin code if 2 args
    if len(q) == 2:
        rows = db.execute("SELECT * FROM places WHERE place_name LIKE :q1 AND admin_name1 LIKE :q2 \
                                                    OR place_name LIKE :q1 AND admin_code1 LIKE :q2", \
                                                    q1 = query[0], q2 = query[1])

    # Check place name, admin name, country code if 3 args
    if len(q) == 3:
        rows = db.execute("SELECT * FROM places WHERE place_name LIKE :q1 AND admin_name1 LIKE :q2 \
                                                    AND (postal_code LIKE :q3 OR country_code LIKE :q3)", \
                                                    q1 = query[0], q2 = query[1], q3 = query[2])

    return jsonify(rows)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
