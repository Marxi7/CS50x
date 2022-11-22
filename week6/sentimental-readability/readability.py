# TODO
from cs50 import get_string

text = get_string("Text: ")

letters = 0
words = 1
sentences = 0

# Iterating through the text and count the letters, words and sentences.
for c in text:
    if c.isalpha():
        letters += 1

    elif c.isspace():
        words += 1

    elif c == '.' or c == '?' or c == '!':
        sentences += 1

# Calculating the average number of letters and sentences
# We define L & S as float to be precise, but then, we'll round up

L = float(letters) / float(words) * 100
S = float(sentences) / float(words) * 100

# Index formula - plus, we add the round function to round up the result:
index = round(0.0588 * L - 0.296 * S - 15.8)

print(index)

if index >= 16:
    print("Grade 16+")

elif index < 1:
    print("Before Grade 1")

else:
    print("Grade", index)

