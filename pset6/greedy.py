import cs50

def GetCoins(money):
    n = 0

    if(money > 0):
        while(money >= 25):
            n += 1
            money -= 25

        while(money >= 10):
            n += 1
            money -=10

        while(money >=5):
            n += 1
            money -= 5

        while(money >= 1):
            n += 1
            money -= 1

    print(n)
    return 0

def main():
    while True:
        print("O hai! How much change is owed?")
        change = cs50.get_float()
        if change > 0:
            break

    roundedChange = round(change * 100)
    GetCoins(roundedChange)

if __name__ == "__main__":
    main()
