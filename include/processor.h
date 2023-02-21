#ifndef PROCESSOR_H
#define PROCESSOR_H


#include <string>
#include <vector>


class Processor {
 public:
  Processor(std::string name);
  std::string Name() const;
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  std::string name_;
  std::vector<int> *preInfo;
};

#endif