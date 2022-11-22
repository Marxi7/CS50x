# TODO

while True:
    try:
        # prompting the user for the height of the pyramid with input
        height = int(input("Height: "))
        # making sure that the pyramid isn't smallr than 1 and bigger than 8
        if height > 0 and height < 9:
            break
        else:
            print("You must enter a number between 1 and 8!")
    except:
        print("Please, enter an integer between 1 and 8!")


for row in range(height):
    # the number of space for each new line is always height - row - 1
    space = height - row - 1

    # printing the number of spaces
    for spaces in range(space):
        print(" ", end="")

    # printing the number of column. for each line we'll add one more colomn as there is one more column than row for each new line.
    for column in range(row + 1):
        print("#", end="")

    # printing the middle separation with two spaces
    print("  ", end="")

    for column in range(row + 1):
        print("#", end="")

    # printing a line each time
    print()