import socket

sock=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

message="True"
sock.sendto(message.encode(), ('127.0.0.1', 1234))
data, addr=sock.recvfrom(1024)
print(data.decode())
sock.close()