# TODO
while True:
    try:
        # multiplying by 100 to work with integer to avoid any problem with floating numbers
        money_owed = int(float(input("Change owed: "))*100)
        if money_owed >= 0:
            break
        else:
            print("You must enter positive numbers only!")
    except:
        print("Please, enter an integer only!")

count_quarters = 0
count_dimes = 0
count_nickels = 0
count_pennies = 0

total_count = 0

# count for quarters
while money_owed >= 25:
    money_owed -= 25
    count_quarters += 1

# count for dimes
while money_owed >= 10:
    money_owed -= 10
    count_dimes += 1

# count for nickels
while money_owed >= 5:
    money_owed -= 5
    count_nickels += 1

# count for pennies
while money_owed >= 1:
    money_owed -= 1
    count_pennies += 1

total_count += count_quarters + count_dimes + count_nickels + count_pennies

print(total_count)
