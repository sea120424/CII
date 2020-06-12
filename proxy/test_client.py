import socket
import select
import sys
flag = 1
s = socket.socket()
s.connect(('127.0.0.1',3457))
print("serching !!!\n")
msg = s.recv(1024)
print(msg.decode())
r_list = [sys.stdin, s]
while flag:
    readarray, writearray, errorarray = select.select(r_list,[],[],10)
    for fd in readarray:
        if fd == s:
            data = s.recv(1024)
            print(data.decode())
        else:
            msg = sys.stdin.readline()
            if msg == '0':
                break
            s.send(msg.encode())
# input_msg = input('input>>>')
# if input_msg == '0':
#  break
# s.sendall(input_msg.encode())
# msg = s.recv(1024)
# print(msg.decode())

s.close()
