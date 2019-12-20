#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>

using namespace std;

#define MV 10000;

int main (int argc, const char **argv)
{
    int sock, jumlahk, i=1;
    char pesan[10000];
    char *pesanp_;
    const char*pesanp="ok";
    struct sockaddr_in alamat, alamatl;
    socklen_t panjang;
    int port;
    char ip[100];

    port = atoi(argv[2]);
    strcpy(ip,argv[1]);    

    if(argv[1]==NULL || argv[2]==NULL)
    {
        cout << "Masukkan dengan format ./execfile (alamat IP) (PORT)" << endl;
        exit(EXIT_FAILURE);
    }
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&alamat, 0, sizeof(alamat));
    memset(&alamatl, 0, sizeof(alamatl));

    alamat.sin_family = AF_INET;
    alamat.sin_addr.s_addr = inet_addr(ip);
    alamat.sin_port = htons(port);

    bind(sock, (struct sockaddr *)&alamat, sizeof(alamat));

    cout << "IP: " << argv[1] << "PORT: " << port << endl;
    while(1)
    {
        panjang = sizeof(alamatl);

        jumlahk = recvfrom(sock, pesan, sizeof(pesan), MSG_WAITALL, (struct sockaddr *)&alamatl, &panjang);
        pesan[jumlahk] = '\0';
        cout << "Pesan ke-" << i << " " << pesan << endl;
        
        // strcpy(pesanp_,"ok");
        // strcpy(pesanp,pesanp_);
        sendto(sock, pesanp, strlen(pesanp) + 1, MSG_CONFIRM, (struct sockaddr *)&alamatl, panjang);
        i++;
    }

    return 0;
}