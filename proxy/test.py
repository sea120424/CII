import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('www.python.org', 80)
client_socket.connect(server_address)

request_header = 'GET / HTTP/1.0\r\nHost: www.python.org\r\n\r\n'
request_header = str.encode(request_header)
client_socket.send(request_header)

response = ''
while True:
    recv = client_socket.recv(1024)
    if not recv:
        break
    #response += recv 
    print(recv)
    print('\n');
print(response)
client_socket.close()    
