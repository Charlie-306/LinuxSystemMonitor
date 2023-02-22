#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <regex>

class Process {
 public:
  [[nodiscard]] int Pid() const;
  [[nodiscard]] std::string User() const;
  [[nodiscard]] std::string Command() const;
  [[nodiscard]] float CpuUtilization() const;
  [[nodiscard]] std::string Ram() const;
  [[nodiscard]] long int UpTime() const;
  bool operator<(Process const& a) const;
  explicit Process(int pid);
  Process();
 private:
  int pid_;
  float cpu_util_;
};

#endif