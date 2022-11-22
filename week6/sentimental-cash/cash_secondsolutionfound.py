# TODO
while True:
    try:
        # multiplying by 100 to work with integer to avoid any problem with floating numbers for each steps
        money_owed = int(float(input("Change owed: "))*100)
        if money_owed >= 0:
            break
        else:
            print("You must enter positive numbers only!")
    except:
        print("Please, enter an integer only!")


# using the quotient to get how many times we can use a quarter/dimes/nickels/pennies
# updating the rest of the money that is owed each time
count_quarters = money_owed // 25
money_owed -= 25 * count_quarters

# numbers of dimes needed
count_dimes = money_owed // 10
money_owed -= 10 * count_dimes

# numbers of nickels needed
count_nickels = money_owed // 5
money_owed -= 5 * count_nickels

# numbers of pennies needed
count_pennies = money_owed // 1
money_owed -= 1 * count_pennies


total_count = count_quarters + count_dimes + count_nickels + count_pennies

print(total_count)
