import os from datetime import datetime from cs50 import SQL from flask import Flask, redirect, render_template, request, session, url_for from flask_session import Session from tempfile import mkdtemp from werkzeug.security import check_password_hash, generate_password_hash from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    """ lookup each stock """
    """ display a table of the summary of holdings """

    # Retrieve user's ID and cash balance
    user_id = session["user_id"]
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    # Retrieve user's transactions and calculate holdings
    transactions = db.execute(
        "SELECT symbol, SUM(shares) AS total_shares FROM transactions "
        "WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
        user_id,
    )

    # Calculate current value of each holding
    total_value = user_cash
    for transaction in transactions:
        symbol = transaction["symbol"]
        total_shares = transaction["total_shares"]
        quote_info = lookup(symbol)
        price_per_share = quote_info["price"]
        holding_value = price_per_share * total_shares
        transaction["price_per_share"] = price_per_share
        transaction["holding_value"] = holding_value
        total_value += holding_value

    return render_template(
        "index.html", cash=user_cash, transactions=transactions, total_value=total_value
    )


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        username = request.form.get("username")
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Check if the username already exists
        existing_user = db.execute("SELECT id FROM users WHERE username = ?", username)
        if existing_user:
            return apology("username already exists", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Check if the password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Insert new user into database
        result = db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            request.form.get("username"),
            generate_password_hash(request.form.get("password")),
        )

        # Log in the newly registered user
        session["user_id"] = result

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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


# Route for user admin page
@app.route("/admin")
@login_required
def admin():
    return render_template("admin.html")


# Route for changing password
@app.route("/admin/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "GET":
        return render_template("admin.html")
    else:
        # Get the current password, new password, and confirm password from the form
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirm_password = request.form.get("confirm_password")

        # Ensure all fields are filled
        if not current_password or not new_password or not confirm_password:
            return apology("All fields must be filled.", 400)

        # Retrieve the current user's information
        user_id = session["user_id"]
        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Verify the current password
        if not check_password_hash(user[0]["hash"], current_password):
            return apology("Incorrect current password.", 400)

        # Check if the new password and confirm password match
        if new_password != confirm_password:
            return apology("New passwords do not match.", 400)

        # Update the user's password in the database
        new_hash = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, user_id)

        return redirect(url_for("index"))


# Route for adding cash to the account
@app.route("/admin/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "GET":
        return render_template("admin.html")
    else:
        # Get the amount of cash to add from the form
        add_cash = float(request.form.get("cash"))

        # Validate the cash amount
        if add_cash <= 0:
            return apology("Invalid cash amount")

        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = :id", id=user_id)
        user_cash = user_cash_db[0]["cash"]

        new_cash = user_cash + add_cash

        # Update the cash balance in the database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)

        return redirect(url_for("index"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    """ GET displays form to request a quote """
    """ POST lookup the stock symbol be calling lookup function and display the results """

    # User reached route via GET (display form)
    if request.method == "GET":
        return render_template("quote.html")

    # User reached route via POST (lookup stock symbol)
    else:
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Lookup stock symbol
        symbol = request.form.get("symbol").upper()
        quote_info = lookup(symbol)

        # Check if symbol is valid
        if quote_info is None:
            return apology("invalid symbol", 400)

        # Display quote information
        return render_template("quote.html", quote_info=quote_info)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    """ GET disply form to buy a stock, render apology if symbol does not exist """
    """ POST purchase(inputs: symbol, no of shares) the stock as long as the user can afford it, redirect to home upon completion """
    """ render apology if user funds are insufficient """

    # User reached route via GET (display form)
    if request.method == "GET":
        symbol = request.args.get("symbol", "")
        return render_template("buy.html", symbol=symbol)

    # User reached route via POST (purchase stock)
    else:
        # Ensure symbol and shares were submitted
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol")

        # Lookup stock symbol
        quote_info = lookup(symbol)

        # Check if symbol is valid
        if quote_info is None:
            return apology("invalid symbol")

        # if (type(shares) == float or type(shares) == str or int(shares) <= 0):
        #     return apology("invalid number of shares")
        try:
            shares = float(shares)
            if shares <= 0 or shares % 1 != 0:
                raise ValueError
        except (ValueError, TypeError):
            return apology("invalid number of shares")

        shares = int(shares)
        # Calculate total cost
        price_per_share = quote_info["price"]
        total_cost = price_per_share * shares

        # Check if the user can afford the purchase
        user_id = session["user_id"]
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0][
            "cash"
        ]

        if user_cash < total_cost:
            return apology("insufficient funds")

        # Perform the stock purchase
        transaction_date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
            user_id,
            symbol,
            shares,
            price_per_share,
            transaction_date,
        )

        # Update user's cash balance
        updated_cash = user_cash - total_cost
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)

        # Redirect user to home page
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    """ input of symbol, apology if user fails to select stock """
    """ input of volume to sell, apology if the input is not positive or more than the owned amount """
    """ POST to sell """
    """ redirect to home """

    # User reached route via GET (display form)
    if request.method == "GET":
        # Retrieve user's ID and their stocks' symbols and quantities
        user_id = session["user_id"]
        stocks = db.execute(
            "SELECT symbol, SUM(shares) AS total_shares FROM transactions "
            "WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
            user_id,
        )
        symbol = request.args.get("symbol", "")
        return render_template("sell.html", stocks=stocks, symbol=symbol)

    # User reached route via POST (sell stocks)
    else:
        # Ensure symbol and shares were submitted
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        try:
            shares = float(shares)
            if shares <= 0 or shares % 1 != 0:
                raise ValueError
        except (ValueError, TypeError):
            return apology("invalid number of shares", 400)

        shares = int(shares)

        # Retrieve user's ID and their stock's quantity
        user_id = session["user_id"]
        stock_info = db.execute(
            "SELECT symbol, SUM(shares) AS total_shares FROM transactions "
            "WHERE user_id = ? AND symbol = ? GROUP BY symbol",
            user_id,
            symbol,
        )

        # Check if the user owns the specified stock
        if not stock_info or stock_info[0]["total_shares"] < shares:
            return apology("insufficient shares", 400)

        # Lookup stock symbol
        quote_info = lookup(symbol)

        # Calculate total sale value
        price_per_share = quote_info["price"]
        total_value = price_per_share * shares

        # Perform the stock sale
        transaction_date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
            user_id,
            symbol,
            -shares,
            price_per_share,
            transaction_date,
        )

        # Update user's cash balance
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0][
            "cash"
        ]
        updated_cash = user_cash + total_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)

        # Redirect user to home page
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve user's ID
    user_id = session["user_id"]

    # Retrieve user's transaction history
    transactions = db.execute(
        "SELECT symbol, shares, price, date, CASE WHEN shares > 0 THEN 'Buy' ELSE 'Sell' END AS action "
        "FROM transactions WHERE user_id = ? ORDER BY date DESC",
        user_id,
    )

    return render_template("history.html", transactions=transactions)
