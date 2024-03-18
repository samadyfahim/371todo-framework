// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------
// An Date class that contains the following member variables
// - year
// - month
// - day
// - initialised
//
// A Date object should only be set to initialised if it is a valid date.
// Otherwise it is should remain unintialised.
// -----------------------------------------------------

#ifndef DATE_H
#define DATE_H
#include <sstream>
#include "lib_json.hpp"

class Date
{
private:
  unsigned int year, month, day;
  bool initialised;

public:
  Date();
  void setDateFromString(const std::string &dateString);
  bool isInitialised() const;
  std::string str() const;
  void setDate(int y, int m, int d);
  unsigned int getYear() const;
  unsigned int getMonth() const;
  unsigned int getDay() const;
  bool operator==(const Date &other) const;
  bool operator<(const Date &other) const;
};

#endif
