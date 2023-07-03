# TODO
from cs50 import get_float

# Ask how many cents the customer is owed
while True:
    dollars = get_float("Change owed: ")
    if dollars <= 0:
        continue
    dollars = round(dollars, 2)

# Calculate the number of quarters to give the customer
    quarters = int(dollars / 0.25)
    dollars = dollars - quarters * 0.25

# Calculate the number of dimes to give the customer
    dimes = int(dollars / 0.1)
    dollars = dollars - dimes * 0.1

# Calculate the number of nickels to give the customer
    nickels = round(dollars / 0.05)
    dollars = dollars - nickels * 0.05

# Calculate the number of pennies to give the customer
    pennies = round(dollars / 0.01)
    dollars = dollars - pennies * 0.01

# Sum coins
    coins = quarters + dimes + nickels + pennies

# Print total number of coins to give the customer
    print(f"{coins}")
    break
