import socket

sock=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('127.0.0.1', 1234))

while True:
  data, addr=sock.recvfrom(4096)
  if (data.decode() == 'True'):
    message="Data benar diterima"
  else:
    message="Data salah diterima"

  sock.sendto(message.encode(), addr)
