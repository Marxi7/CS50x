# TODO
import sys
import re


def main():

    card = card_input()
    check_number_digit(card)
    luhn_check(card)


def card_input():
    while True:
        try:
            card = input("Number: ")
            if len(card) > 0 and int(card):
                break
        except ValueError:
            print("INVALID")
    return card


# Checking the length of the card
def check_number_digit(card):
    if len(card) not in (13, 15, 16):
        print("INVALID")
        exit()
    else:
        return card


def luhn_check(card):
    # Convert to integer list and put it into a list
    digits = [int(x) for x in card]

    # Reverse the card numbers
    digits = digits[::-1]

    # Getting the numbers we will multiply by two, even numbers from the right
    even = digits[1::2]

    # Getting the odd numbers from the right, numbers to sum simply
    odd = digits[0::2]

    # Findind the sum of the odd numbers
    sum_odd = 0
    sum_odd += sum(odd)

    # Looping through the list of even numbers and finding the sum of the even numbers
    # For each loop, we add the the first and second number coming from the product of x * 2
    sum_even = 0
    for number in even:
        sum_even += (number * 2 // 10) + (number * 2 % 10)

    # Finding the total sum and checking its validity through Luhn's algorithm
    total = 0
    total += sum_odd + sum_even

    if total % 10 == 0:
        pass
    else:
        print("INVALID LUHN CARD")
        return

    # Isolating the first two numbers into a list that we need to
    # Transform into int again as the list will transform the elemnts inside into string automatically
    start_of_card = int(card[:2])

    # MASTERCARD
    if start_of_card // 10 == 5 and start_of_card % 10 in (1, 2, 3, 4, 5) and len(card) == 16:
        print("MASTERCARD")

    # AMEX
    elif start_of_card // 10 == 3 and start_of_card % 10 in (4, 7) and len(card) == 15:
        print("AMEX")

    # VISA
    elif start_of_card // 10 == 4 and len(card) in (13, 16):
        print("VISA")

    else:
        print("INVALID")


if __name__ == "__main__":
    main()