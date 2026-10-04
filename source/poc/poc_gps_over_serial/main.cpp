/*************************************************************************************************
 * Diego Minguzzi
 *************************************************************************************************/
#include "cli_args.h"
#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

namespace
{
::std::atomic_bool terminate{ false };

extern "C" void onSigInt( int )
{
  terminate= true;
}
}

void dumpGpsData(const CliArgs&);

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

void dumpGpsData(const CliArgs& cliArgs)
{
  ::std::signal(SIGINT, onSigInt);
  while(!terminate)
  {
    ::std::cerr <<"Cycle executed." <<::std::endl;
    ::std::this_thread::sleep_for( ::std::chrono::milliseconds{ 500});
  }
  ::std::cerr <<"dumpGpsData terminating." <<::std::endl;
}