// measure.cpp : Defines the entry point for the console application.
//

#include "targetver.h"
#include "measure.hpp"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <Psapi.h>

struct RunResult {
  bool succeeded;
  PROCESS_INFORMATION pi;
};

RunResult RunProcess(wchar_t *command) {
  STARTUPINFO si{};
  PROCESS_INFORMATION pi{};

  // Start the child process. 
  if (!CreateProcess(NULL,   // No module name (use command line)
    command,        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi)             // Pointer to PROCESS_INFORMATION structure
    )
  {
    //printf("CreateProcess failed (%d).\n", GetLastError());
    return RunResult{ false, pi };
  }
  
  // Wait until child process exits.
  WaitForSingleObject(pi.hProcess, INFINITE);

  return RunResult{ true, pi };
}

static void PrintMemStats(const PROCESS_INFORMATION &pi) {
  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc)))
  {

    std::cout << "\tPageFaultCount: " << MemorySizePrinter::PrettyPrintMemorySize(pmc.PeakWorkingSetSize) << std::endl;
#if 0
    printf("\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount);
    printf("\tPeakWorkingSetSize: 0x%08X\n",
      pmc.PeakWorkingSetSize);
    printf("\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize);
    printf("\tQuotaPeakPagedPoolUsage: 0x%08X\n",
      pmc.QuotaPeakPagedPoolUsage);
    printf("\tQuotaPagedPoolUsage: 0x%08X\n",
      pmc.QuotaPagedPoolUsage);
    printf("\tQuotaPeakNonPagedPoolUsage: 0x%08X\n",
      pmc.QuotaPeakNonPagedPoolUsage);
    printf("\tQuotaNonPagedPoolUsage: 0x%08X\n",
      pmc.QuotaNonPagedPoolUsage);
    printf("\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage);
    printf("\tPeakPagefileUsage: 0x%08X\n",
      pmc.PeakPagefileUsage);
#endif
  }
}

static void PrintTimeStats(const StopWatch &sw) {
  std::cout << "\n" << "Finished in"
            << DurationPrinter::PrettyPrintDuration(sw.Elapsed())
            << std::endl;
}

struct Options {
  std::unique_ptr<wchar_t[]> Command;
  bool ShowMemStats;
  bool ShowTimeStats;

  Options() 
  : Command(nullptr)
  , ShowMemStats(true)
  , ShowTimeStats(true)
  { }
};

static void PrintUsageAndExit(int exitCode = EXIT_FAILURE) {
  printf("usage: measure <command>");
  exit(exitCode);
}

Options ParseOptions(int argc, wchar_t *argv[]) {
  Options options{};
  if (argc <= 1) {
    PrintUsageAndExit();
  }


  std::wstring command;
  for (int i = 1; i < argc; i++) {
    command.append(argv[i]);
    command.append(L" ");
  }

  if (command.size()) {
    options.Command.reset(new wchar_t[command.size() + 1]);
    std::copy(command.begin(), command.end(), options.Command.get());
    options.Command.get()[command.size()] = L'\0';
  }

  if (!options.Command) {
    PrintUsageAndExit();
  }

  return options;
}

int wmain(int argc, wchar_t *argv[])
{
  Options options = ParseOptions(argc, argv);
  
  StopWatch sw;
  RunResult proc = RunProcess(options.Command.get());
  sw.Stop();
 
  if (!proc.succeeded) {
    wprintf(L"Failed to start process %s", options.Command.get());
    exit(EXIT_FAILURE);
  }

  if (options.ShowTimeStats) {
    PrintTimeStats(sw);
  }

  if (options.ShowMemStats) {
    PrintMemStats(proc.pi);
  }
  
  // Close process and thread handles. 
  CloseHandle(proc.pi.hProcess);
  CloseHandle(proc.pi.hThread);

  return EXIT_SUCCESS;
}

