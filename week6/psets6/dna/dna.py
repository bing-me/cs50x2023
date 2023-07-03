import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    database_file = open(sys.argv[1], 'r')
    database_reader = csv.DictReader(database_file)
    database = database_reader.fieldnames[1:]

    # TODO: Read DNA sequence file into a variable
    sequence_file = open(sys.argv[2], 'r')
    sequence = sequence_file.read()
    sequence_file.close()

    # TODO: Find longest match of each STR in DNA sequence
    # store the subsequences to a list
    # run throught the seiquence and look for the longest sequence of each subsequence
    sequence_fingerprint = {}
    for subsequence in database:
        sequence_fingerprint[subsequence] = longest_match(sequence, subsequence)

    # TODO: Check database for matching profiles
    # search through csv file to find match
    for row in database_reader:
        # if there is match, print name, close files, and end program
        if match(database, sequence_fingerprint, row):
            print(f"{row['name']}")
            database_file.close()
            return
    # else print No match and end program
    print("No match")
    database_file.close()


def match(database, sequence_fingerprint, row):
    for subsequence in database:
        if sequence_fingerprint[subsequence] != int(row[subsequence]):
            return False
    return True


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
