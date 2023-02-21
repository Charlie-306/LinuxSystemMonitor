#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

vector<Processor>& System::Cpu(){
  if (cpus_.empty()){
    for (auto const &name:LinuxParser::getCpuNames())
      cpus_.emplace_back(Processor(name));
  }
  return cpus_;
}

vector<Process>& System::Processes() {
  processes_.clear();
  for(auto const& pid:LinuxParser::Pids()){
    processes_.emplace_back(Process(pid));
  }
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

long int System::UpTime() { return LinuxParser::UpTime(); }
