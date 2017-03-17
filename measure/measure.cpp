// measure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>

using namespace std::chrono;

class StopWatch {
public:
  using hrc = std::chrono::high_resolution_clock;
  using timepoint = std::chrono::time_point<hrc>;
  using duration = std::chrono::duration<double>;

  StopWatch()
    : m_start(hrc::now())
  {}

  void Stop() {
    m_end = hrc::now();
  }

  duration Elapsed() {
    return m_end - m_start;
  }
  
  static void PrettyPrintDuration(std::ostream &out, duration elapsed) {
    std::chrono::minutes minutes = duration_cast<std::chrono::minutes>(elapsed);
    out << minutes.count() << " minutes";
   
    std::chrono::seconds seconds = duration_cast<std::chrono::seconds>(elapsed);
    out << seconds.count() << " seconds";
  }

  void PrettyPrintDuration(std::ostream &out) {
    PrettyPrintDuration(out, m_end - m_start);
  }

private:
  timepoint m_start;
  timepoint m_end;
};

class DurationPrinter {
public:
  using duration = std::chrono::duration<double>;
  
  static void PrettyPrintDuration(std::ostream &out, duration elapsed) {
    std::chrono::minutes minutes = duration_cast<std::chrono::minutes>(elapsed);
    out << minutes.count() << " minutes ";
   
    std::chrono::seconds seconds = duration_cast<std::chrono::seconds>(elapsed);
    out << seconds.count() << " seconds ";
  }
  
  static std::string PrettyPrintDuration(duration elapsed) {
    std::ostringstream ss;
    PrettyPrintDuration(ss, elapsed);
    return ss.str();
  }
};

int main()
{
  StopWatch sw;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  sw.Stop();
  std::cout << "Finished in " << DurationPrinter::PrettyPrintDuration(sw.Elapsed()) << std::endl;
  return 0;
}

