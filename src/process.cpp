
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid): pid_(pid),
                            cpu_util_(LinuxParser::CpuUtilization(pid)){}
Process::Process(): pid_(0), cpu_util_(.0){}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
  return cpu_util_;
}

string Process::Command() const {
  return LinuxParser::Command(Pid());
}

string Process::Ram() const {
  return LinuxParser::Ram(Pid());
}

string Process::User() const {
  return LinuxParser::User(Pid());
}

long int Process::UpTime() const {
  return LinuxParser::UpTime(Pid());
}

bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}
