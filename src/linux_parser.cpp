#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>

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

float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  std::getline(stream, line);
  int memTotal = stoi(matchOnePattern(line,
                                      std::regex(R"(MemTotal:\s+(.*)\s)")));
  std::getline(stream, line);
  int memFree = stoi(matchOnePattern(line,
                                     std::regex(R"(MemFree:\s+(.*)\s)")));
  return (memTotal - memFree) / (float)memTotal;
}

long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;
  std::getline(stream, line);
  return stoi(matchOnePattern(line, std::regex(R"(([\w.]+)\s)")));
}

vector<string> LinuxParser::getCpuNames(){
  vector<string> names;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while(std::getline(stream, line)){
    std::smatch match;
    if(std::regex_search(line, std::regex("cpu"))){
      std::regex_search(line, match, std::regex(R"((cpu(\d+)?)\s)"));
      names.emplace_back(match[1]);
    }
  }
  return names;
}

vector<int> LinuxParser::getCpuInfo(const string& cpu_name){
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  vector<int> cpu_info;
  std::smatch match;
  while(std::getline(stream, line)){
    std::regex_search(line, match,
                          std::regex((cpu_name+"(.*)").c_str()));
    if(match[1].matched) break;
  }
  int info;
  std::stringstream ss(match[1]);
  while(ss >> info) cpu_info.emplace_back(info);
  return cpu_info;
}

float LinuxParser::getIdle(vector<int> &cpuInfo){
  return (float)cpuInfo[CPUStates::kIdle_] + cpuInfo[CPUStates::kIOwait_];
}

float LinuxParser::getNonIdle(std::vector<int> &cpuInfo){
  return (float)cpuInfo[CPUStates::kUser_] + cpuInfo[CPUStates::kNice_] +
         cpuInfo[CPUStates::kSystem_] + cpuInfo[CPUStates::kIRQ_] +
         cpuInfo[CPUStates::kSoftIRQ_] + cpuInfo[CPUStates::kSteal_];
}

float LinuxParser::CpuUtilization(std::vector<int> &crtInfo,
                                  std::vector<int> &prevInfo){
  auto prevIdle = getIdle(prevInfo);
  auto Idle = getIdle(crtInfo);
  auto prevNonIdle = getNonIdle(prevInfo);
  auto NonIdle = getNonIdle(crtInfo);

  auto prevTotal = prevIdle + prevNonIdle;
  auto Total = Idle + NonIdle;
  auto total_d = Total - prevTotal;
  auto idle_d = Idle - prevIdle;
  return (total_d - idle_d) / total_d;
}

int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  return stoi(matchOnePattern(stream,
                              std::regex(R"(processes\s(\w+))")));
}

int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  return stoi(matchOnePattern(stream,
                              std::regex(R"(procs_running\s(\w+))")));
}

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
  std::string user = matchOnePattern(stream_usr, user_pt);
  return user == "0"? "unknown": user;
}

long LinuxParser::UpTime(int pid) {
  return starttime(pid) / sysconf(_SC_CLK_TCK);
}

string LinuxParser::matchOnePattern(const std::string& src, const std::regex& pattern){
  std::smatch result;
  std::regex_search(src, result, pattern);
  return (!result.empty() && result[1].matched) ? result[1].str(): "0";
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
  return (!result.empty() && result[1].matched) ? result[1].str(): "0";
}

string LinuxParser::ProcessStat(int pid){
  std::ifstream stream(kProcDirectory + to_string(pid) +
                       kStatFilename);
  string line;
  std::getline(stream, line);
  return line;
}

float LinuxParser::CpuUtilization(int pid) {
  float total_time = (float)utime(pid) + stime(pid) + cutime(pid) + cstime(pid);
  float seconds = (float)UpTime() - UpTime(pid);
  return ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
}

bool LinuxParser::ProcessAlive(int pid){
  return CpuUtilization(pid) > .0;
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