#include <iostream>
#include <string>
#include <string.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/asio/basic_datagram_socket.hpp>
#include <cstdio>

namespace b1 = boost::asio;
namespace b2 = boost::system;
using boost::array;
using b1::ip::udp;
using std::cout;
using std::endl;
using std::string;
using std::cerr;

// UDPServer::UDPServer(boost::asio::io_service &io) :
//    m_socket(io, udp::v4()),
//    m_endpoint()
// {
//    boost::asio::socket_base::reuse_address option(true);
//    this->m_socket.set_option(option);
//    this->m_socket.bind(udp::endpoint(udp::v4(), 3838));
//    this->start_receive();
// }

int main()
{
  try
  {
    b1::io_service io;
    udp::endpoint local_endpoint;
    udp::endpoint remote_endpoint;
    // int i=0;
    udp::socket sock(io);
    // boost::asio::ip::udp::acceptor acceptor(io);
    boost::asio::socket_base::reuse_address option(true);
    
    sock.open(udp::v4());
    sock.set_option(option);

    remote_endpoint = udp::endpoint(b1::ip::address_v4::any(), 3838);
    local_endpoint = udp::endpoint(b1::ip::address_v4::any(), 3838);
    sock.bind(local_endpoint);

    for (;;)
    {
      
      array<char, 1024> pesank;
      // udp::endpoint endpointk;
      b2::error_code perror;
      // i++;
      // sock.get_option(option);
      // bool is_set = option.value();

      size_t panjang;

      panjang = sock.receive_from(b1::buffer(pesank), remote_endpoint, 0, perror);
      // if(perror == b1::error::eof)
      //   break;

      char buffer[panjang][5];
      for(int i = 0 ; i < panjang ; i++)
      {
        sprintf(buffer[i], "%02X", pesank[i]);
      }

      if(buffer[4][1] == '8'){
        // cout.write(pesank.data(), panjang);
        // cout << panjang << endl;
        // cout << *buffer[7] << endl;
        
        switch (buffer[7][1])
        {
          case '0':
            cout << "Initial" << endl;
            break;
          case '1':
            cout << "Ready" << endl;
            break;
          case '2':
            cout << "Set" << endl;
            break;
          case '3':
            cout << "Play" << endl;
            break;
          case '4':
            cout << "Finish" << endl;
            break;
          default:
            cout << "None" << endl;
        }

        int first_t = 19;
        int second_t = 89;

        if(buffer[first_t][1]=='0'){
          cout << "Blue Team Score: " << buffer[first_t+1][1] << endl;
        } else if(buffer[first_t][1]=='1'){
          cout << "Red Team Score: " << buffer[first_t+1][1] << endl;
        }
        if(buffer[second_t][1]=='0'){
          cout << "Blue Team Score: " << buffer[second_t+1][1] << endl;
        } else if(buffer[second_t][1]=='1'){
          cout << "Red Team Score: " << buffer[second_t+1][1] << endl;
        }

      }

      if (perror && perror != b1::error::message_size)
        throw b2::system_error(perror);
      
    }
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
  }

  return 0;
}