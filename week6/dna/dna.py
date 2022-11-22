import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py databases/large.csv sequences/5.txt ")

    # TODO: Read database file into a variable
    database_file = sys.argv[1]
    with open(database_file) as csvfile:
        reader = csv.reader(csvfile)
        database = list(reader)

    # TODO: Read DNA sequence file into a variable
    dna_txt = sys.argv[2]
    with open(dna_txt) as f:
        sequence = f.read()

    # TODO: Find longest match of each STR in DNA sequence
    # We iterate from after the 1st line in the database, that's why we start with 1
    matches = []
    for i in range(1, len(database[0])):

        # if the sequence number match with a number for someone in the databse, we add this match to our list
        matches.append(longest_match(sequence, database[0][i]))

    # TODO: Check database for matching profiles
    # We set the matching person variable to no match as we are not sure yet if there are any.
    # We'll change it to the name of a person if we find a match.
    match_counter = 0
    matching_person = 'No Match'

    # Iterating through the database from the second line (1) and through the matches to see if we can find a match between both.
    for i in range(1, len(database)):
        for j in range(len(matches)):

            # The name is all the way to the left of the databse, at the first place
            name = database[i][0]
            # We need to use int to convert the elements in the list
            if matches[j] == int(database[i][j+1]):
                match_counter += 1

        # To find the exact match, the match count needs to match the len of the matches we got from longest_match
        if match_counter == len(matches):
            matching_person = name
            break
        else:
            match_counter = 0
    print(matching_person)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
