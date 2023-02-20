#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid): pid_(pid){}
Process::Process(): pid_(0){}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() {
  return LinuxParser::CpuUtilization(Pid());
}

string Process::Command() {
  return LinuxParser::Command(Pid());
}

string Process::Ram() {
  return LinuxParser::Ram(Pid());
}

string Process::User() {
  return LinuxParser::User(Pid());
}

long int Process::UpTime() {
  return LinuxParser::UpTime(Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
