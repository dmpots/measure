#include "stdafx.h"
#include "CppUnitTest.h"
#include "../measure/measure.hpp"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {
    template <> static std::wstring ToString(const long long& q) { return std::to_wstring(q); }
    template <> static std::wstring ToString(const long long* q) { return std::to_wstring(*q); }
    template <> static std::wstring ToString(long long* q) { return std::to_wstring(*q); }
}}}

namespace test
{		
  TEST_CLASS(UnitTest1)
  {
    using str = std::string;
    using DB = DurationPrinter::DurationBreakdown;
	public:
      DurationPrinter m_printer;
      TEST_METHOD(TestNanoSecondsOnly)
      {
        std::chrono::nanoseconds d(10);
        DB db(d);
        Assert::AreEqual(0, db.hours);
        Assert::AreEqual(0, db.minutes);
        Assert::AreEqual(0ll, db.seconds);
        Assert::AreEqual(0ll, db.milliseconds);
        Assert::AreEqual(0ll, db.microseconds);
        Assert::AreEqual(10ll, db.nanoseconds);
      }
      
      TEST_METHOD(TestNanoSecondsAndMicroseconds)
      {
        std::chrono::nanoseconds d(7003);
        DB db(d);
        Assert::AreEqual(0, db.hours);
        Assert::AreEqual(0, db.minutes);
        Assert::AreEqual(0ll, db.seconds);
        Assert::AreEqual(0ll, db.milliseconds);
        Assert::AreEqual(7ll, db.microseconds);
        Assert::AreEqual(3ll, db.nanoseconds);
      }
      
      TEST_METHOD(TestMulti)
      {
        std::chrono::nanoseconds d(2000795143);
        DB db(d);
        Assert::AreEqual(0, db.hours);
        Assert::AreEqual(0, db.minutes);
        Assert::AreEqual(2ll, db.seconds);
        Assert::AreEqual(0ll, db.milliseconds);
        Assert::AreEqual(795ll, db.microseconds);
        Assert::AreEqual(143ll, db.nanoseconds);
      }
      
      TEST_METHOD(TestHours)
      {
        std::chrono::nanoseconds d(10800000000000);
        DB db(d);
        Assert::AreEqual(3, db.hours);
        Assert::AreEqual(0, db.minutes);
        Assert::AreEqual(0ll, db.seconds);
        Assert::AreEqual(0ll, db.milliseconds);
        Assert::AreEqual(0ll, db.microseconds);
        Assert::AreEqual(0ll, db.nanoseconds);
      }
      
      TEST_METHOD(TestAll)
      {
        std::chrono::nanoseconds d(10950700800900ll);
        DB db(d);
        Assert::AreEqual(3, db.hours);
        Assert::AreEqual(2, db.minutes);
        Assert::AreEqual(30ll, db.seconds);
        Assert::AreEqual(700ll, db.milliseconds);
        Assert::AreEqual(800ll, db.microseconds);
        Assert::AreEqual(900ll, db.nanoseconds);
      }
  };
}