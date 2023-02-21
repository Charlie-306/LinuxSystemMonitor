#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


vector<Processor>& System::Cpu(){
  if (cpus_.empty()){
    for (auto const &name:LinuxParser::getCpuNames())
      cpus_.emplace_back(name);
  }
  return cpus_;
}

vector<Process>& System::Processes() {
  processes_.clear();
  for(auto const& pid:LinuxParser::Pids()){
    processes_.emplace_back(pid);
  }
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization();}

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses();}

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
