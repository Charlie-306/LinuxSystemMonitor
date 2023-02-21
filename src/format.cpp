#include <string>
#include <chrono>

#include "format.h"

using std::string;


string CompensateZero(int num){
  return num >= 10 ? std::to_string(num): "0" + std::to_string(num);
}
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  return CompensateZero(seconds / 3600) + ":" +
         CompensateZero(seconds % 3600 / 60) + ":" +
         CompensateZero(seconds % 60);
}