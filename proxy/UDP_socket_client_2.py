#UDP client
import socket
import time
HOST, PORT = "123.192.190.173", 12345
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
count = 10000
while(count > 0 ):
    count = count - 1
    addr = (HOST, PORT)
    s.sendto(b"J", addr)
time.sleep(5)    
s.sendto(b"end", addr)
