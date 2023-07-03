# TODO
from cs50 import get_string

# get string
while True:
    text = get_string("Text: ")

    letters = 0
    space = 0
    end_punct = 0

# check if text[i] is an alphabet
    for i in text:
        if i.isalpha():
            letters += 1

# check if text[i] is a blank
        elif i.isspace():
            space += 1

# check if text[i] is a ', ? !'
        elif i == '.' or i == '?' or i == '!':
            end_punct += 1

    words = space + 1

# Coleman-liau index | index = 0.0588 * L - 0.296 * S - 15.8
    L = float(letters * 100 / words)
    S = float(end_punct * 100 / words)
    cli = float((0.0588 * L) - (0.296 * S) - 15.8)
    clir = round(cli)

# print grade | if less than 1, state so and if above 16, 16+.

    if clir < 1:
        print("Before Grade 1")
    elif 1 <= clir and clir <= 16:
        print(f"Grade {clir}")
    elif (clir > 16):
        print("Grade 16+")

    break
