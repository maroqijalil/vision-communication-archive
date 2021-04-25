#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>
#include <string>

int main (int argc, const char **argv)
{
  int cl_sock;
  cl_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (cl_sock < 0) {
    std::cout << "Gagal menginisiasi client" << std::endl;
    return 1;
  }

  struct sockaddr_in srv_addr;
  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  srv_addr.sin_port = htons(1234);

  const char * message = "Halo dari client";
  if (sendto(
      cl_sock, message, strlen(message) + 1, 0,
      (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    std::cout << "Gagal mengirim pesan" << std::endl;
    return 1;
  } else {
    std::cout << "Berhasil mengirim pesan: " << message << std::endl;
  }

  struct sockaddr_in other_addr;
  socklen_t other_addr_length = sizeof(other_addr);
  char recvd_message[4095];
  if(recvfrom(
    cl_sock, recvd_message, sizeof(recvd_message), 0,
    (struct sockaddr *)&other_addr, &other_addr_length) < 0) {
    std::cout << "Gagal menerima pesan" << std::endl;
    return 1;
  } else {
    std::cout << "Pesan dari server: " << recvd_message << std::endl;
  }

  close(cl_sock);

  return 0;
}