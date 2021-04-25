#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>

int main (int argc, const char **argv)
{
  int srv_sock;
  srv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (srv_sock < 0) {
    std::cout << "Gagal menginisiasi server" << std::endl;
    return 1;
  }

  struct sockaddr_in srv_addr;
  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  srv_addr.sin_port = htons(1234);

  if (bind(srv_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    std::cout << "Gagal menginisiasi server" << std::endl;
    return 1;
  }

  while(true)
  {
    struct sockaddr_in cl_addr;
    socklen_t cl_addr_length = sizeof(cl_addr);
    char recvd_message[4095];
    if(recvfrom(
      srv_sock, recvd_message, sizeof(recvd_message), 0,
      (struct sockaddr *)&cl_addr, &cl_addr_length) <= 0) {
      std::cout << "Gagal menerima pesan" << std::endl;
      return 1;
    } else {
      std::cout << "Pesan dari client: " << recvd_message << std::endl;
    }
    
    const char * message = "Halo dari server";
    if (sendto(
      srv_sock, message, strlen(message) + 1, 0,
      (struct sockaddr *)&cl_addr, sizeof(cl_addr)) < 0) {
      std::cout << "Gagal mengirim balasan" << std::endl;
      return 1;
    } else {
      std::cout << "Berhasil mengirim balasan: " << message << std::endl;
    }
  }

  close(srv_sock);

  return 0;
}