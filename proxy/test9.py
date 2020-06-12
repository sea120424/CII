import socket
HOST, PORT = "", 1322
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
while(True):
    s.listen(1)
    print("server is work on " + HOST )

    while(True):
        client, address = s.accept()
        print(str(address)+" connected")
        req_str = client.recv(1000).decode('utf-8') 
        print(req_str)
        print("\n ~~~~~~~~ \n")
        
        total_num = len(req_str.splitlines())
        num = 0 ;
        while(num < total_num):
            print(req_str.splitlines()[num])
            if(req_str.splitlines()[num][0] == 'G' and
               req_str.splitlines()[num][1] == 'E' and
               req_str.splitlines()[num][2] == 'T' ):
                print("In GET line\n")
                now = 3 
                while(req_str.splitlines()[num][now] != '/'):
                    now = now +1 ;
                    
                now = now +1 ;   
                search_str = ""
                while(req_str.splitlines()[num][now] != ' '):
                    search_str = search_str + req_str.splitlines()[num][now]
                    now = now +1 ;
            
                #print(search_str)
                try:
                    file = open(search_str , "r")
                except IOError:
                    print("404 Error!!")
                    req_str = req_str + "<html> <body> 404 Not Found</body></html>"
                    print(req_str)
                    req_str = str.encode(req_str)
                    header = "HTTP/1.1 404\r\n" + "Content-Type: text/html; charset=utf-8\r\n"
                    header = header + "Server: nginx/1.11.8\r\n" + "transfer-encoding: chunked\r\n"
                    header = header + "Connection: Close\r\n" + "<!doctype html>\r\n"
                    header = header + '<html lang= "en">\r\n' 
                    header = header + "<html> <body> 404 Not Found </body> </html>"
                    
                    print(header)
                    header = str.encode(header)
                    client.send(header)
                    #client.send(b"<html> <body> 404 Not Found</body> </html>")
                else:
                    web_data = ""
                    for line in file:
                        data = line[:-1]
                        print(data)
                        web_data = web_data + data     
                    file.close()
                    web_data = web_data + ">"
                    print(web_data)
                    web_data = str.encode(web_data)
                    client.send(web_data)
                break
            num = num + 1

        client.close()
