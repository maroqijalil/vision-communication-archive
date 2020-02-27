#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace b1 = boost::asio;
namespace b2 = boost::system;
using boost::array;
using b1::ip::udp;
using std::cout;
using std::endl;
using std::string;
using std::cerr;

// string baca(udp::socket & sock)
// {
//   b1::streambuf pes;
//   b1::read_until(sock, pes, "!");
//   string data = b1::buffer_cast<const char*>(pes.data());
//   return data;
// }

int main()
{
  try
  {
    b1::io_service io;
    int i=0;
    udp::socket sock(io, udp::endpoint(udp::v4(), 12345));

    for (;;)
    {
      array<char, 128> pesank;
      udp::endpoint endpointk;
      b2::error_code perror;
      i++;

      size_t panjang;

      panjang = sock.receive_from(b1::buffer(pesank), endpointk, 0, perror);
      // if(perror == b1::error::eof)
      //   break;

      // string pesank2 = baca(sock);
      cout.write(pesank.data(), panjang);
      cout << " " << i <<endl;
      // cout << pesank2 << endl;

      if (perror && perror != b1::error::message_size)
        throw b2::system_error(perror);

      string pesans = "diterima";

      b2::error_code perror2;
      sock.send_to(b1::buffer(pesans), endpointk, 0, perror2);
    }
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
  }

  return 0;
}