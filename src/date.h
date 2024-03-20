// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2035827>
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
  void setDate(int y, int m, int d) noexcept;
  unsigned int getYear() const noexcept;
  unsigned int getMonth() const noexcept;
  unsigned int getDay() const noexcept;
  bool operator==(const Date &other) const;
  bool operator<(const Date &other) const;
};

struct InvalidDate : public std::invalid_argument
{
  explicit InvalidDate(const std::string &message) : std::invalid_argument(message) {}
  ~InvalidDate() override = default;
};

#endif
