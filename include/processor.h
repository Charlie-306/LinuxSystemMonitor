#ifndef PROCESSOR_H
#define PROCESSOR_H


#include <string>
#include <vector>
#include <memory>


class Processor {
 public:
  Processor();
  explicit Processor(std::string name);
  [[nodiscard]] std::string Name() const;
  float Utilization();

 private:
  std::string name_;
  std::shared_ptr<std::vector<int>> preInfo;
};

#endif