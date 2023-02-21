#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  std::vector<Processor>& Cpu();
  std::vector<Process>& Processes();
  static float MemoryUtilization();
  static long UpTime();
  static int TotalProcesses();
  static int RunningProcesses();
  static std::string Kernel();
  static std::string OperatingSystem();

 private:
  std::vector<Processor> cpus_ = {};
  std::vector<Process> processes_ = {};
};

#endif