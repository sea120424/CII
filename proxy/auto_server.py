import socket
import select
import queue

server = socket.socket()
server_IP = '127.0.0.1'
server_PORT = 3457
server.bind((server_IP, server_PORT))
server.listen(5)
r_list = [server,]
count = 0 
wait_queue = queue.Queue()
list_start = []
list_target = []

while True:
    readarray, writearray, errorarray = select.select(r_list,[],[],10)
    count = count+1
    print(count)
    for fd in readarray:
        print(fd)
        if fd == server: #新連線
            clientfd , addr = fd.accept()
            r_list.append(clientfd)
            print(addr)
            if wait_queue.empty():
                wait_queue.put(clientfd)
                list_start.append(clientfd)
            else:
                connfd = wait_queue.get()
                clientfd.sendall(b"find pair")
                connfd.sendall(b"find pair")
                list_start.append(clientfd)
                list_target.append(clientfd)
                list_target.append(connfd)

        else:
            try:
                msg = fd.recv(1024)
                i=0
                while(i<len(list_start)):
                    if fd == list_start[i]:
                        list_target[i].sendall(msg)
                        break
                    i=i+1
                #if fd == list_start[0]:
                #    list_target[0].sendall(msg)
                #else:
                #    list_target[1].sendall(msg)
                
                #print(msg)
                #fd.sendall('second'.encode())
            except ConnectionAbortedError:
                i=0
                
                while(i<len(list_start)):
                    if fd == list_start[i]:
                        conn = i
                        break
                    i=i+1
                list_target[conn].sendall(b"other side is quit")
                r_list.remove(list_start[conn]) #停止監聽
                r_list.remove(list_target[conn])
                start_fd = list_start[conn]
                end_fd = list_target[conn]
                list_start.remove(end_fd)
                list_start.remove(start_fd)
                list_target.remove(end_fd)
                list_target.remove(start_fd)
                start_fd.close()
                end_fd.close()
                
                
                
server.close()      
