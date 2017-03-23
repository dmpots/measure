#pragma once

#include <chrono>
#include <string>
#include <sstream>

class DurationPrinter {
public:
  using duration = std::chrono::duration<double>;
  struct DurationBreakdown {
    std::chrono::hours::rep hours;
    std::chrono::minutes::rep minutes;
    std::chrono::seconds::rep seconds;
    std::chrono::milliseconds::rep milliseconds;
    std::chrono::microseconds::rep microseconds;
    std::chrono::nanoseconds::rep nanoseconds;
    
    DurationBreakdown(duration d) {
      long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();
      hours        = ExtractTimeDuration<std::chrono::hours>(&ns);
      minutes      = ExtractTimeDuration<std::chrono::minutes>(&ns);
      seconds      = ExtractTimeDuration<std::chrono::seconds>(&ns);
      milliseconds = ExtractTimeDuration<std::chrono::milliseconds>(&ns);
      microseconds = ExtractTimeDuration<std::chrono::microseconds>(&ns);
      nanoseconds  = ExtractTimeDuration<std::chrono::nanoseconds>(&ns);
    }
    
    template <typename delta>
    static typename delta::rep ExtractTimeDuration(long long *pns) {
      long long &ns = *pns;
      delta t = std::chrono::duration_cast<delta>(std::chrono::nanoseconds(ns));
      ns -= std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
      return t.count();
    }
  };

  static void PrettyPrintDuration(std::ostream &out, duration elapsed) {
    DurationBreakdown db(elapsed);
    out << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() << " ns (";
    WriteTimeDuration(out, db.hours,        " hours");
    WriteTimeDuration(out, db.minutes,      " minutes");
    WriteTimeDuration(out, db.seconds,      " seconds");
    WriteTimeDuration(out, db.milliseconds, " milliseconds");
    WriteTimeDuration(out, db.microseconds, " microseconds");
    WriteTimeDuration(out, db.nanoseconds,  " nanoseconds");
    out << " )";
  }
  
  static std::string PrettyPrintDuration(duration elapsed) {
    std::ostringstream ss;
    PrettyPrintDuration(ss, elapsed);
    return ss.str();
  }

private:
    template <typename rep>
    static void WriteTimeDuration(std::ostream &out, rep component, const std::string &name) {
      if (component) {
        out << " " << component << name;
      }
  }
};

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

  duration Elapsed() const {
    return m_end - m_start;
  }
  
private:
  timepoint m_start;
  timepoint m_end;
};

class MemorySizePrinter {
public:
  enum Size {
    Bytes   = 1,
    KiBytes = Bytes   * 1024,
    MiBytes = KiBytes * 1024,
    GiBytes = MiBytes * 1024,
  };

  struct MemoryBreakdown {
    unsigned Gigabytes;
    unsigned Megabytes;
    unsigned Kilobytes;
    unsigned Bytes;

    MemoryBreakdown(size_t bytes) {
      Gigabytes = ExtractSize<GiBytes>(&bytes);
      Megabytes = ExtractSize<MiBytes>(&bytes);
      Kilobytes = ExtractSize<KiBytes>(&bytes);
      Bytes     = ExtractSize<Size::Bytes>(&bytes);
    }
  };
    
  template <unsigned Size>
  static unsigned ExtractSize(size_t *pBytes) {
    size_t &bytes = *pBytes;
    unsigned count = bytes / Size;
    bytes -= count * Size;
    return count;
  }
  
  static std::string PrettyPrintMemorySize(size_t bytes) {
    std::ostringstream ss;
    PrettyPrintMemorySize(ss, bytes);
    return ss.str();
  }
  
  static void PrettyPrintMemorySize(std::ostream &out, size_t bytes) {
    MemoryBreakdown mbd(bytes);
    out << " " << bytes << " bytes (";
    WriteMemorySize(out, mbd.Gigabytes, " GiB");
    WriteMemorySize(out, mbd.Megabytes, " MiB");
    WriteMemorySize(out, mbd.Kilobytes, " KiB");
    WriteMemorySize(out, mbd.Bytes,     " B");
    out << " )";
  }

private:
  static void WriteMemorySize(std::ostream &out, unsigned component, const std::string &name) {
    if (component) {
      out << " " << component << name;
    }
  }
};
