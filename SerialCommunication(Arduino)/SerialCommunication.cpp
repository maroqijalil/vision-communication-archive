#include "boost/array.hpp"
#include "boost/asio.hpp"
#include "boost/thread.hpp"

#include <vector>
#include <iostream>

using namespace std;
using namespace boost::asio;

void read_command(serial_port& serial_port)
{
  uint8_t buffer[1024];
  size_t length;
  
  while(true)
  {
    length = read(serial_port, boost::asio::buffer(buffer));
    
    for(size_t i = 0; i < length; ++i)
    {
      cout << (buffer[i]) << flush;

      boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }
  }
}

void write_command(serial_port& serial_port)
{
  char message[1024];

  cout << "For the first message, type \"Initial\"" << endl;
  std::cin >> message;
  size_t length_m = strlen(message);

  if(!strcmp(message, "Initial"))
  {
    for (unsigned char i = 0; ; ++i)
    {
      write(serial_port, boost::asio::buffer(&i, sizeof i));
      boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }
  }
  else 
  {
    write(serial_port, boost::asio::buffer(message, length_m));
  }
}

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io;
    boost::asio::serial_port serial_port(io, "/dev/ttyUSB0");
    serial_port.set_option(boost::asio::serial_port_base::baud_rate(9600));

    if(argv[1]==NULL)
    {
      cout << "Enter the command such as following format: ./sc <command option>" << endl;
    }

    if(!strcmp(argv[1], "read"))
    {
      read_command(serial_port);
    }
    else if(!strcmp(argv[1], "write"))
    {
      write_command(serial_port);
    }
  }
  catch(exception& e)
  {
    cerr << e.what() << endl;
  }
  
  return 0;
}