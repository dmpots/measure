// measure.cpp : Defines the entry point for the console application.
//

#include "targetver.h"
#include "measure.hpp"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <thread>

int main()
{
  StopWatch sw;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  sw.Stop();
  std::cout << "Finished in" << DurationPrinter::PrettyPrintDuration(sw.Elapsed()) << std::endl;
  return 0;
}

