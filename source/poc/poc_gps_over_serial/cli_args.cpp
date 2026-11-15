/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/
#include "cli_args.h"
#include <stdexcept>
#include <string>
#include <ctime>

using namespace ::std::string_literals;

//------------------------------------------------------------------------------------------------
CliArgs CliArgs::parse(int argc, char* argv[])
{
  const auto usage= "Usage: <program> <serial_device> [baud_rate]"s;
  if (argc<2)
  {
    throw ::std::invalid_argument("Missing required serial device argument. e.g. /dev/ttyUSB0\n"s
                                  + usage);
  }

  const auto serialDevice= ::std::string{ argv[1]};
  auto baudRate= 4800;
  try
  {
    if (argc>=3)
    {
      baudRate= ::std::stoi(argv[2]);
    }
  }
  catch (const ::std::exception& e)
  {
    throw ::std::invalid_argument("Invalid baud rate argument. e.g. 4800\n"s + usage);
  }

  ::std::string dumpFile;
  if (argc>=4)
  {
    dumpFile= ::std::string{ argv[3]};
  }
  else
  {
    // Creates a dumpFile name with the current timestamp in the format "dump_YYYYMMDD_HHMMSS.log"
    auto t= ::time(nullptr);
    auto tm= *::localtime(&t);
    char buffer[64];
    ::strftime(buffer, sizeof(buffer), "dump_%Y%m%d_%H%M%S.log", &tm);
    dumpFile= ::std::string{ buffer };
  }

  return CliArgs(serialDevice, baudRate, dumpFile);
}

//------------------------------------------------------------------------------------------------
::std::string CliArgs::toString() const
{
  return "(serialDevice: "s + _serialDevice + ", baudRate: "
        + ::std::to_string(_baudRate)
        + ", dumpFile: " + _dumpFile + ")";
}