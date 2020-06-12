import socket

HOST, PORT = "", 12345
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))

while(True):
    s.listen(0)
    print("The Grading server for HW2 is running..")

    while(True):
        client, address = s.accept()
        print(str(address)+" connected")
        try:
            client.send(b"Welcome to HW2 P1 Local Server. Please give me your identity. What's your name?\n")
            student_name = client.recv(1000).decode('utf-8')
            client.send(b"What's your student ID?\n")
            student_id = client.recv(1000).upper().decode('utf-8')
            client.send(b"What's your favorite food?\n")
            favorite_food = client.recv(1000).lower().decode('utf-8')


            client.send(("Hi " + student_name +
                    ", your student id is " + student_id +
                    ". And you love " + favorite_food + ". Is it correct? (Y/N)\n").encode('utf-8'))
            YorN = client.recv(1000).decode('utf-8')

            if YorN == None:
                client.send(("Please retry it again.\n").encode('utf-8'))
            elif YorN!="Y" and YorN!="y":
                client.send(("Please retry it again.\n").encode('utf-8'))
            else:
                client.send(("Thanks. Your response has been recorded. " +
                        "Please remeber to print-screen this execution, and have a nice day! " +
                        "(Session End)\n").encode('utf-8'))
                print("Hi " + student_name +
                    ", your student id is " + student_id +
                    ". And you love " + favorite_food+"\n")
                client.close()
        except:
            print("except")


