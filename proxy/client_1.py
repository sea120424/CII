
import socket

HOST, PORT = "140.112.42.108", 12345
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("HW2 client is running")
s.connect((HOST, PORT))
print("connected to "+HOST)


while(True):
    question = s.recv(1000).decode('utf-8')
    print("Receive server message:\n")
    print(question)
    answer = input()
    answer_byte = str.encode(answer)
    s.send(answer_byte)

