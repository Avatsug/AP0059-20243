import operations
import time

while True:
        menu = operations.Menu()
        menu.showOptions()
        option = menu.chooseOption()

        if option == 1:
                object1 = operations.Enter_Data()
                a = object1.enterA()
                b = object1.enterB()

                object1 = operations.Add_Sub()
                print(f"Resultado: {object1.add(a, b)}")
                time.sleep(1)

        if option == 2:
                object1 = operations.Enter_Data()
                a = object1.enterA()
                b = object1.enterB()

                object1 = operations.Add_Sub()
                print(f"Resultado: {object1.sub(a, b)}")
                time.sleep(1)

        if option == 3:
                object1 = operations.Enter_Data()
                a = object1.enterA()
                b = object1.enterB()

                object1 = operations.Mult_Div()
                print(f"Resultado: {object1.mult(a, b)}")
                time.sleep(1)

        if option == 4:
                object1 = operations.Enter_Data()
                a = object1.enterA()
                b = object1.enterB()

                object1 = operations.Mult_Div()
                print(f"Resultado: {object1.div(a, b)}")
                time.sleep(1)

        if option == 5:
                object1 = operations.Enter_Data()
                a = object1.enterA()

                object1 = operations.Square(a)
                print(f"Resultado: {object1.getValue()}")
                time.sleep(1)


