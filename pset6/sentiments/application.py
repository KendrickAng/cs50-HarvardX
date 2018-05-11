from flask import Flask, redirect, render_template, request, url_for
from helpers import get_user_timeline
from analyzer import Analyzer
from nltk.tokenize import TweetTokenizer
from termcolor import colored

import helpers
import os
import sys


app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)

    # TODO: redirect to index if None
    if tweets == None:
        return redirect(url_for("index"))

    # get tweets
    tweetlist = []
    tweetanlyze = []
    tweetscore = 0

    try:
        # stores each tweet in the list tweet. one tweet per index.
        tweetlist = get_user_timeline(sys.argv[1])

        if tweetlist == None:
            sys.exit("Something cropped up :/")

    except RuntimeError:
        sys.exit("Run-time error!")

    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # initialise Analyzer
    analyzer = Analyzer(positives, negatives)

    # initialise TweetTokenizer
    tknzr = TweetTokenizer(preserve_case=False, strip_handles=True, reduce_len=True)

    # reset sentiment percentages
    positive, negative, neutral = 0.0, 0.0, 100.0

    for tweetite in tweetlist:

        # reset tweet score
        tweetscore = 0

        # return a list of words tweetanlyze from each tweet
        # convert each tweet into a string to tokenize
        tweetanlyze = tknzr.tokenize(tweetite)

        # analyze each word in tweetite
        for tweetite2 in tweetanlyze:
            score = analyzer.analyze(tweetite2)
            tweetscore += score

            if tweetscore > 0.0:
                positive += 1
            elif tweetscore < 0.0:
                negative += 1
            else:
                neutral += 1

    # calculate percentages
    positive=(100*positive)/(positive+negative+neutral)
    negative=(100*negative)/(positive+negative+neutral)
    neutral=(100*neutral)/(positive+negative+neutral)

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
