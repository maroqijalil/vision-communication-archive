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

int main (int argc, const char **argv)
{
    int sock, jumlahk, i=1;
    char pesan[10000];
    const char *pesanp = argv[3];
    struct sockaddr_in alamat;
    socklen_t panjang;
    int port;
    char ip[100];

    port = atoi(argv[2]);
    strcpy(ip,argv[1]);

    if(argv[1]==NULL || argv[2]==NULL || argv[3]==NULL)
    {
        cout << "Masukkan dengan format ./execfile (alamat IP) (PORT) (pesan)" << endl;
        exit(EXIT_FAILURE);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&alamat, 0, sizeof(alamat));

    alamat.sin_family = AF_INET;
    alamat.sin_addr.s_addr = inet_addr(ip);
    alamat.sin_port = htons(port);
    // sendto(sock, pesanp, strlen(pesanp) + 1, MSG_CONFIRM, (struct sockaddr *)&alamat, sizeof(alamat));
    // while(1)
    // {
        // cout << "Masukkan pesan: ";
        // cin >> pesanp;

        sendto(sock, (const char *)pesanp, strlen(pesanp) + 1, MSG_CONFIRM, (struct sockaddr *)&alamat, sizeof(alamat));
        
        jumlahk = recvfrom(sock, pesan, sizeof(pesan), MSG_WAITALL, (struct sockaddr *)&alamat, &panjang);
        pesan[jumlahk] = '\0';
        cout << "Balasan: " << pesan << endl;
    // }

    close(sock);
    return 0;
}