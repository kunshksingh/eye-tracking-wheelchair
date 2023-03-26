#Sources
#https://realpython.com/python-sockets/

import socket

HOST = "172.20.10.7"  # Run ipconfig on this computer and grab ipv4 address
PORT = 80  # The port used by the server (can be changed to anything not reserved)

# initialize tcp connection
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"Hello, world")

