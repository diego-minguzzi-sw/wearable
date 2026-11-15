/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/
#ifndef POC_GPS_OVER_SERIAL_CLI_ARGS_H
#define POC_GPS_OVER_SERIAL_CLI_ARGS_H

#include <string>

class CliArgs
{
public:
  static CliArgs parse(int argc, char* argv[]);

  const ::std::string& serialDevice() const { return _serialDevice; }
  int baudRate() const { return _baudRate; }
  const ::std::string& dumpFile() const { return _dumpFile; }

  ::std::string toString() const;

protected:
  CliArgs( const ::std::string serialDevice,
           int           baudRate=        4800,
           const ::std::string& dumpFile= ::std::string{})
    : _serialDevice{serialDevice}
    , _baudRate{ baudRate}
    , _dumpFile{ dumpFile}
  {}

private:
  ::std::string _serialDevice;
  int           _baudRate{};
  ::std::string _dumpFile;
};

#endif // POC_GPS_OVER_SERIAL_CLI_ARGS_H