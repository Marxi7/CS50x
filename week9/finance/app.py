import os
from site import USER_SITE

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, jsonify, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import re

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    stocks = db.execute(
        "SELECT symbol, name, price, SUM(shares) as shares FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    cash_db = db.execute("SELECT cash FROM users WHERE id = :id", id=user_id)
    cash = cash_db[0]["cash"]

    total = cash

    for stock in stocks:
        total += stock["price"] * stock["shares"]

    return render_template("index.html", stocks=stocks, cash=cash, usd=usd, total=total)
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol)

        if not symbol:
            return apology("You must enter a correct Symbol!")
        elif not stock:
            return apology("This symbol is either INVALID or does NOT exist.")

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("You must enter a number.")
        
        if shares <= 0:
            return apology("You must enter a positive Integer. You can't buy a negative number of shares.")

        user_id = session["user_id"]
        cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = cash_db[0]["cash"]

        stock_name = stock["name"]
        stock_price = stock["price"]
        total_stock_price = stock_price * shares

        updated_cash = cash - total_stock_price

        if cash < total_stock_price:
            return apology("Not enough cash to buy this stock..")
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)
            db.execute("INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES (?, ?, ?, ?, ?, ?)", 
                       user_id, stock_name, shares, stock_price, 'Buy', symbol)

        flash("Bought!")

        return redirect("/")
    else:
        return render_template("buy.html")
     

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    # getting the data needed for our histor pages - transaction table
    transactions = db.execute("SELECT type, symbol, price, shares, time FROM transactions WHERE user_id = ?", user_id)

    # getting the data needed for our histor pages - movements table (withdrawals, deposits)
    movements = db.execute("SELECT type, amount, time FROM movements WHERE user_id = ?", user_id)

    return render_template("history.html", movements=movements, transactions=transactions, usd=usd)


@app.route("/top_up", methods=["GET", "POST"])
@login_required
def top_up():
    """Show history of transactions"""
    if request.method == "POST":
        topup_amount = int(request.form.get("topup"))

        if not topup_amount:
            return apology("Please, enter an amount if you want to top up your account.")

        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = :id", id=user_id)
        user_cash = user_cash_db[0]["cash"]

        updated_cash_amount = user_cash + topup_amount

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash_amount, user_id)
        db.execute("INSERT INTO movements (user_id, type, amount) VALUES (?, ?, ?)", 
                   user_id, 'Deposit', topup_amount)

        flash("The Top Up amount has been added to your account successfully!")
        return redirect("/")
        
    else:
        return render_template("topup.html")
        

@app.route("/withdraw", methods=["GET", "POST"])
@login_required
def withdraw():
    """Show history of transactions"""
    if request.method == "POST":
        withdraw_amount = int(request.form.get("withdraw"))

        if not withdraw:
            return apology("Please, enter an amount to withdraw.")

        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = :id", id=user_id)
        user_cash = user_cash_db[0]["cash"]

        updated_cash_amount = user_cash - withdraw_amount

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash_amount, user_id)
        db.execute("INSERT INTO movements (user_id, type, amount) VALUES (?, ?, ?)", 
                   user_id, 'Withdrawal', (-1)*withdraw_amount)

        flash("The amount entered has been withdrawed successfully!")
        return redirect("/")

    else:
        return render_template("withdraw.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("You must give a Symbol in order for us to give you a Quote")
        
        stock = lookup(symbol.upper())

        if not stock:
            return apology("Symbol does Not Exist")

        return render_template("quoted.html", stock=stock, usd=usd)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("Please, provide a Username")

        if not password:
            return apology("Please, enter a Password")
        
        if not confirmation:
            return apology("You must confirm your password!")

        # password check -> must contain at least 3 charachters of which one is a special charachter 
        if len(password) < 3:
            return apology("Your password must contain at least 3 charachters")

        # defining special characters
        special_char = re.compile('[@_!#$%^&*()<>?/\|}{~:]')

        # check if string contains special characters or not
        if(special_char.search(password) == None):
            return apology("Your password must contain at least 1 special charachter")

        # making sure the two outut matches
        if password != confirmation:
            return apology("Passwords Do Not Match!")

        hash = generate_password_hash(password)

        try:
            new_registered_user = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        except:
            return apology("Username already exists")
        
        session["user_id"] = new_registered_user

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        requested_shares = int(request.form.get("shares"))

        if requested_shares <= 0:
            return apology("You can't sell a negative number of shares. Please enter an amount of shares equal or smaller than what you own.")

        stock = lookup(symbol)
        stock_price = stock["price"]
        stock_name = stock["name"]
        total_price = requested_shares * stock_price

        shares_owned_by_user = db.execute(
            "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)[0]["shares"]

        if shares_owned_by_user < requested_shares:
            return apology("You don't have anought shares.")

        cash_currently_owned = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        updated_cash = cash_currently_owned + total_price

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)
        db.execute("INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES (?, ?, ?, ?, ?, ?)", 
                   user_id, stock_name, (-1)*requested_shares, stock_price, 'Sell', symbol)

        flash("Sold!")
        return redirect("/")

    else:
        user_id = session["user_id"]
        symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
        return render_template("sell.html", symbols=symbols)


@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changepassword():
    """Show history of transactions"""
    if request.method == "POST":
        user_id = session["user_id"]
        new_password = request.form.get("newpassword")
        confirmation = request.form.get("confirmation")

        if not new_password or not confirmation:
            return apology("Please, provide a New Password")
        
        # password check -> must contain at least 3 charachters of which one is a speciual charachter 
        if len(new_password) < 3:
            return apology("Your password must contain at least 3 charachters")

        # special characters
        special_char = re.compile('[@_!#$%^&*()<>?/\|}{~:]')

        # check string contains special characters or not
        if(special_char.search(new_password) == None):
            return apology("Your password must contain at least 1 special charachter")
        
        # making sure that the new password entered matches in the two inputs
        if new_password != confirmation:
            return apology("The password and the confirmation you entered don't match!")

        new_hash = generate_password_hash(new_password)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, user_id)

        flash("Your Password Has Been Successfully Changed!")
        return redirect("/")

    else:
        return render_template("changepassword.html")


@app.route("/close_account", methods=["GET", "POST"])
@login_required
def close_account():
    """Show history of transactions"""
    if request.method == "POST":
        user_id = session["user_id"]
        close_account_text_validation = request.form.get("close_account")

        if not close_account_text_validation:
            return apology("Please, confirm that you want to close your account by typing YES.")
        
        # Making sure that the user input YES to confirm the deletion of the account.
        elif close_account_text_validation != "YES":
            return apology("You Must type YES in order to submit the form and therefore close your Account.")

        # logging out the user
        session.clear()

        db.execute("DELETE FROM users WHERE id = ?", user_id)

        flash("Your Account Has Been Successfully Deleted!")
        return render_template("register.html")

    else:
        return render_template("close_account.html")
    