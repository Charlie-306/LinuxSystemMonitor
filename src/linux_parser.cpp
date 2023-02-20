#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (auto const &one_path:std::filesystem::directory_iterator{kProcDirectory}){
    string path_str = one_path.path().c_str();
    string folder_str = one_path.path().filename().c_str();
    if (one_path.is_directory() &&
        std::all_of(folder_str.begin(), folder_str.end(), isdigit)){
      int pid = stoi((std::string)one_path.path().filename());
      pids.emplace_back(pid);
    }
  }
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;
  std::getline(stream, line);
  return stoi(matchOnePattern(line, std::regex(R"(([\w.]+)\s)")));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

string LinuxParser::Command(int pid) {
  std::ifstream stream(LinuxParser::kProcDirectory +
                       to_string(pid) +
                       LinuxParser::kCmdlineFilename);
  string line;
  std::getline(stream, line);
  return line;
}

string LinuxParser::Ram(int pid) {
  std::ifstream stream(LinuxParser::kProcDirectory +
                       to_string(pid) +
                       LinuxParser::kStatusFilename);
  std::regex pattern(R"(VmSize:\s+?(\d+)\s)");
  auto ram_num = stod(matchOnePattern(stream, pattern)) / 1024;
  std::ostringstream oss;
  oss.precision(2);
  oss << std::fixed << ram_num;
  string result = oss.str();
  return result;
}

string LinuxParser::Uid(int pid) {
  std::ifstream stream(LinuxParser::kProcDirectory +
                         to_string(pid) +
                         LinuxParser::kStatusFilename);
  std::regex pattern(R"(Uid:\s+?(\w+)\s)");
  return matchOnePattern(stream, pattern);
}

string LinuxParser::User(int pid){
  std::ifstream stream_usr(LinuxParser::kPasswordPath);
  std::regex user_pt(R"((^\w+):x:)" + Uid(pid));
  return matchOnePattern(stream_usr, user_pt);
}

long LinuxParser::UpTime(int pid) {
  return starttime(pid) / sysconf(_SC_CLK_TCK);
}

string LinuxParser::matchOnePattern(const std::string& src, const std::regex& pattern){
  std::smatch result;
  std::regex_search(src, result, pattern);
  return result[1];
}

string LinuxParser::matchOnePattern(std::ifstream &stream, const std::regex& pattern){
  string line;
  std::smatch result;
  while(std::getline(stream, line)){
    if(std::regex_search(line, pattern)){
      std::regex_search(line, result, pattern);
      break;
    }
  }
  return result[1];
}

string LinuxParser::ProcessStat(int pid){
  std::ifstream stream(kProcDirectory + to_string(pid) +
                       kStatFilename);
  string line;
  std::getline(stream, line);
  return line;
}

float LinuxParser::CpuUtilization(int pid) {
  float total_time = utime(pid) + stime(pid) + cutime(pid) + cstime(pid);
  float seconds = UpTime() - UpTime(pid);
  return ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
}

int LinuxParser::utime(int pid){
  return stoi(matchOnePattern(ProcessStat(pid),
                              std::regex(R"((?:[^\s]+\s){13}(\w+))")));
}

int LinuxParser::stime(int pid){
  return stoi(matchOnePattern(ProcessStat(pid),
                              std::regex(R"((?:[^\s]+\s){14}(\w+))")));
}

int LinuxParser::cutime(int pid){
  return stoi(matchOnePattern(ProcessStat(pid),
                              std::regex(R"((?:[^\s]+\s){15}(\w+))")));
}

int LinuxParser::cstime(int pid){
  return stoi(matchOnePattern(ProcessStat(pid),
                              std::regex(R"((?:[^\s]+\s){16}(\w+))")));
}

int LinuxParser::starttime(int pid){
  return stoi(matchOnePattern(ProcessStat(pid),
                              std::regex(R"((?:[^\s]+\s){21}(\w+))")));
}