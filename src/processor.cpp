#include "processor.h"
#include "linux_parser.h"

#include <utility>


Processor::Processor(){
  preInfo = std::make_shared<std::vector<int>>(10, 0);
}

Processor::Processor(std::string name): name_(std::move(name)){
  preInfo = std::make_unique<std::vector<int>>(10, 0);
}

std::string Processor::Name() const{
  return name_;
}

float Processor::Utilization() {
  auto crtInfo = LinuxParser::getCpuInfo(Name());

  auto cpu_util = LinuxParser::CpuUtilization(crtInfo, (*preInfo));
  *preInfo = crtInfo;
  return cpu_util;
}