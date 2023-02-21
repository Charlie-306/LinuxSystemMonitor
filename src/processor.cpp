#include "processor.h"
#include "linux_parser.h"

#include <iostream>

Processor::Processor(std::string name): name_(name){
  preInfo = new std::vector<int>(10, 0);
}

std::string Processor::Name() const{
  return name_;
}

float Processor::Utilization() {
//  for(auto ele:preInfo) std::cout << ele << std::endl;
  auto crtInfo = LinuxParser::getCpuInfo(Name());
//  for(auto ele:crtInfo) std::cout << ele << std::endl;

  auto cpu_util = LinuxParser::CpuUtilization(crtInfo, (*preInfo));
  *preInfo = crtInfo;
  return cpu_util;
}