/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/
#include "cli_args.h"
#include <atomic>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <poll.h>
#include <stdexcept>
#include <termios.h>
#include <thread>
#include <unistd.h>

namespace
{
static constexpr size_t RxBufferSize{1024};

::std::atomic_bool terminate{ false };

extern "C" void onSigInt( int )
{
  terminate= true;
}
}

namespace /*anonymous*/ {
void dumpGpsData(const CliArgs&);
} /*anonymous*/

//------------------------------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  try
  {
    auto cliArgs= CliArgs::parse(argc, argv);
    ::std::cout << "cliArgs: " << cliArgs.toString() << ::std::endl;
    dumpGpsData(cliArgs);
    return 0;
  }
  catch (const ::std::exception& e)
  {
    ::std::cerr << "Error: " << e.what() << ::std::endl;
    return -1;
  }
}

namespace /*anonymous*/ {

//------------------------------------------------------------------------------------------------
speed_t toSpeed(int baudRate)
{
  switch (baudRate)
  {
    case 1200:   return B1200;
    case 2400:   return B2400;
    case 4800:   return B4800;
    case 9600:   return B9600;
    case 19200:  return B19200;
    case 38400:  return B38400;
    case 57600:  return B57600;
    case 115200: return B115200;
    default:
      throw ::std::runtime_error{ "Unsupported baud rate: "
              + ::std::to_string(baudRate) };
  }
}

//------------------------------------------------------------------------------------------------
class SerialPort
{
public:
  SerialPort(const ::std::string& device, int baudRate)
  {
    _fd= ::open(device.c_str(), O_RDWR | O_NOCTTY);
    if (_fd < 0)
    {
      throw ::std::runtime_error{ "Failed to open " + device + ": " + ::std::strerror(errno) };
    }

    termios tty{};
    if (::tcgetattr(_fd, &tty) != 0)
    {
      fail("tcgetattr");
    }
    ::cfmakeraw(&tty);
    tty.c_cflag&= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
    tty.c_cflag|= CS8 | CLOCAL | CREAD;
    tty.c_cc[VMIN]=  0;
    tty.c_cc[VTIME]= 1; // read() times out after 100 ms.
    ::cfsetispeed(&tty, toSpeed(baudRate));
    ::cfsetospeed(&tty, toSpeed(baudRate));
    if (::tcsetattr(_fd, TCSANOW, &tty) != 0)
    {
      fail("tcsetattr");
    }
    ::tcflush(_fd, TCIOFLUSH);
  }

  ~SerialPort() { ::close(_fd); }
  SerialPort(const SerialPort&)=            delete;
  SerialPort& operator=(const SerialPort&)= delete;

  int fd() const { return _fd; }

private:
  [[noreturn]] void fail(const char* what)
  {
    const ::std::string msg= ::std::string{what} + " failed: " + ::std::strerror(errno);
    ::close(_fd);
    throw ::std::runtime_error{ msg };
  }

  int _fd{ -1 };
};

//------------------------------------------------------------------------------------------------
void dumpGpsData(const CliArgs& cliArgs)
{
  ::std::cerr <<  __func__ << " started." <<::std::endl;
  ::std::signal(SIGINT, onSigInt);

  // Opens the dump text file.
  ::std::ofstream dumpFile{ cliArgs.dumpFile() };
  if (!dumpFile)
  {
    throw ::std::runtime_error{ "Failed to open dump file: " + cliArgs.dumpFile() };
  }

  SerialPort serial{ cliArgs.serialDevice(), cliArgs.baudRate() };

  auto dataTimeout = ::std::chrono::milliseconds{ 3000};
  auto cycleTime = ::std::chrono::milliseconds{ 250};

  while(!terminate)
  {
    // Read text data from the serial line, waiting for at most dataTimeout.
    pollfd pfd{ serial.fd(), POLLIN, 0 };
    ::std::cerr <<  __func__ << " waiting data." <<::std::endl;
    const int ready= ::poll(&pfd, 1, static_cast<int>(dataTimeout.count()));
    if (ready < 0)
    {
      if (errno == EINTR)
      {
        continue; // Interrupted by SIGINT: the loop condition handles termination.
      }
      ::std::cerr <<"poll failed: " <<::std::strerror(errno) <<::std::endl;
      return;
    }
    if (ready == 0)
    {
      ::std::cerr <<"No data received within " <<dataTimeout.count() <<" ms." <<::std::endl;
      return;
    }

    char buffer[RxBufferSize]= {'\0'};
    char timestamp[16]= {'\0'};
    ::std::tm localTime{};

    if (const auto count= ::read(serial.fd(), buffer, sizeof(buffer)); count < 0)
    {
      if (errno == EINTR)
      {
        continue;
      }
      ::std::cerr <<"read failed: " <<::std::strerror(errno) <<::std::endl;
      return;
    }
    else if (0 == count)
    {
      ::std::cerr <<"Serial device closed or reported no data." <<::std::endl;
      return;
    }
    else
    {
      // Dump the read data to the dumpFile.
      dumpFile.write(buffer, count);
      dumpFile.flush();
      if (!dumpFile)
      {
        ::std::cerr <<"Failed to write to dump file." <<::std::endl;
        return;
      }

      const auto now= ::std::chrono::system_clock::now();
      const auto nowTime= ::std::chrono::system_clock::to_time_t(now);
      ::localtime_r(&nowTime, &localTime);
      ::std::strftime(timestamp, sizeof(timestamp)-1, "%H::%M::%S", &localTime);

      ::std::cerr <<"buffer:" <<buffer  <<::std::endl;
      ::std::cerr <<timestamp <<" " <<count <<" bytes.\n" <<::std::endl;
    }

    ::std::this_thread::sleep_for(cycleTime);
  }

  ::std::cerr <<  __func__ << " terminated." <<::std::endl;
}

} /*anonymous*/


