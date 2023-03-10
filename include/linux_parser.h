#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
float getIdle(std::vector<int> &cpuInfo);
float getNonIdle(std::vector<int> &cpuInfo);
std::vector<std::string> getCpuNames();
std::vector<int> getCpuInfo(const std::string& cpu_name);
float CpuUtilization(std::vector<int> &crtInfo,
                     std::vector<int> &prevInfo);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
std::string ProcessStat(int pid);
std::string matchOnePattern(const std::string& src, const std::regex& pattern);
std::string matchOnePattern(std::ifstream &stream, const std::regex& pattern);
long UpTime(int pid);
float CpuUtilization(int pid);
int utime(int pid);
int stime(int pid);
int cutime(int pid);
int cstime(int pid);
int starttime(int pid);
bool ProcessAlive(int pid);
};  // namespace LinuxParser

#endif