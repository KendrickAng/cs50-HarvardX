import cs50

def PrintRow(height):
    inc = height
    dec = height

    # setup number of spaces and 'stairs'
    for i in range(0, height):
        inc += 1
        dec -= 1

        for z in range(1, (dec + 1)):
            print(" ", end= "")

        for y in range(height, (inc + 1)):
            print("#", end= "")

        print("")

def main():

    # establish do-while
    while True:
        print("Height: ", end= "")
        h = cs50.get_int();
        if(h < 24 or h > -1):
            break;

    PrintRow(h)

if __name__ == "__main__":
    main()