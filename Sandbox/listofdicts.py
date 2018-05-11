myList = [{'age':x} for x in range(1,10)]
print(myList)

stocks = {"price" : 40, "symbol" : "STI"}

for dicts in myList:
    dicts["price"] = stocks["price"]
    print(dicts)

print(myList)