import cs50
import string
import crypt
import sys

def Check(stringLength, genString, inputString):

    if(stringLength == len(genString)):
        # hash plaintext to be checked against input
        encStr = crypt.crypt("".join(genString), "50")

        # check for a match
        if(encStr == inputString):
            print("".join(genString))
            sys.exit(0)

    return 1;


def CrackPassword(input):

    chars = []

    # convert string of lower+uppercase alphabet to list
    for letter in string.ascii_letters:
        chars.append(letter)

    # generate 1-character plaintexts
    stringLength = 1
    genString = ["a"]

    for letter in chars:
        genString[0] = letter

        # Check the crypted word
        Check(stringLength, genString, input)

    # generate 2-character plaintexts
    stringLength = 2
    genString = ["a", "a"]

    for letter in chars:
        genString[0] = letter

        for letter2 in chars:
            genString[1] = letter2

            # Check the crypted word
            Check(stringLength, genString, input)

    # generate 3-character plaintexts
    stringLength = 3
    genString = ["a", "a", "a"]

    for letter in chars:
        genString[0] = letter

        for letter2 in chars:
            genString[1] = letter2

            for letter3 in chars:
                genString[2] = letter3

                # Check the crypted word
                Check(stringLength, genString, input)

    # generate 4-character plaintexts
    stringLength = 4
    genString = ["a", "a", "a", "a"]

    for letter in chars:
        genString[0] = letter

        for letter2 in chars:
            genString[1] = letter2

            for letter3 in chars:
                genString[2] = letter3

                for letter4 in chars:
                    genString[3] = letter4

                    # Check the crypted word
                    Check(stringLength, genString, input)

    # if no matches are found
    print("""The answer you're looking for either has more than 4 characters,
        doesnt consist ONLY of alphabets, or doesn't exist!""")
    sys.exit(1)

def main():

    if(len(sys.argv)) != 2:
        print("Error: Only accepting one command-line argument!")
        return 1;

    else:
        CrackPassword(sys.argv[1])

if __name__ == "__main__":
    main()