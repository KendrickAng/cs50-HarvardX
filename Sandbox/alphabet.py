import cs50
import string

chars = []

for letter in string.ascii_letters:
    print(letter)
    chars.append(letter)

print(*chars,sep='\n')