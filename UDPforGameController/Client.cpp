#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace b1 = boost::asio;
using boost::array;
using b1::ip::udp;
using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char* argv[])
{
  try
  {
    b1::io_service io;

    udp::resolver penentu(io);
    udp::resolver::query pilihan(udp::v4(), "localhost", "12345");
    udp::endpoint endpointk = *penentu.resolve(pilihan);

    udp::socket sock(io);
    sock.open(udp::v4());

    array<char, 10> pesank  = { "dijajal!" };
    sock.send_to(b1::buffer(pesank), endpointk);

    array<char, 128> pesans;
    udp::endpoint endpoints;
    size_t panjang = sock.receive_from(b1::buffer(pesans), endpoints);

    cout.write(pesans.data(), panjang);
    cout << endl;
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
  }

  return 0;
}