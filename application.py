from flask import Flask, redirect, render_template, request, url_for

import os
import sys
import helpers
from analyzer import Analyzer

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
    tweets = helpers.get_user_timeline(screen_name , count=100)
    # TODO
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    analyzer = Analyzer(positives, negatives)
    n = 0
    
    positive, negative, neutral = 0.0, 0.0, 0.0
    
    for each_tweet in tweets:
        ## 取出第幾則 tweet 來進行 tokenize 
        tokens =  tweets[n]
        n += 1
        # analyze tweet
        ## 再透過 analye 將一則 tweet 轉多個文字
        score = analyzer.analyze(tokens)
        
        if score > 0.0:
            positive +=1
        elif score < 0.0:
            negative +=1
        else:
            neutral +=1

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
