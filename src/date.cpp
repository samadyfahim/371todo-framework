// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2023/24)
// Department of Computer Science, Swansea University
//
// Author: <2035827>
//
// Canvas: https://canvas.swansea.ac.uk/courses/44636
// -----------------------------------------------------

#include "date.h"

Date::Date() : year(0), month(0), day(0), initialised(false) {}

// TODO write a function setDateFromString that takes a string argument in
// "YYYY-MM-DD" format and sets the appropriate member variables (year, month,
// day, and initialised). If dateString is empty reset the object to its
// uninitialised state (see default constructor). If dateString is not valid
// throw an appropriate exception.
// Example:
//  Date d = Date();
//  d.setDateString("2024-01-01");

void Date::setDateFromString(const std::string &dateString)
{
    if (dateString.empty())
    {
        *this = Date();
        return;
    }

    std::istringstream iss(dateString);
    char dateDelimiter;

    if (!(iss >> year >> dateDelimiter >> month >> dateDelimiter >> day) || (dateDelimiter != '-') || iss.get() != EOF)
    {
        throw InvalidDate("Not valid date format. Expected format: YYYY-MM-DD please change the date format. ");
    }

    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        setDate(0, 0, 0);
        initialised = false;
        throw InvalidDate("Not valid date value.");
    }

    initialised = true;
}

// TODO Write a function, isInitialised, that takes no parameters and returns true
//  if the Date object is initialised, and false if it is NOT initialised.
// Example:
//  Date d = Date();
//  if(d.isInitialised()) {
//    ...
//  }

bool Date::isInitialised() const
{
    return initialised;
}

// TODO Write a function, str, that takes no parameters and returns a
// std::string representation of the Date object in YYYY-MM-DD format.  If the
// Date object is NOT initialised, return an empty string.
// Example:
//  Date d = Date();
//  d.setDate(2024,1,1);
//  std::cout << d.str() << std::endl;

std::string Date::str() const
{
    if (!initialised)
        return "";
    return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
}

// TODO create a function, setDate, that takes three parameters: year, month,
// day and sets the appropriate member variables to those parameters and also
// sets the initilized flag.
// Example:
//  Date d = Date();
//  d.setDate(2024, 1, 1);

void Date::setDate(int year, int month, int day) noexcept
{
    this->year = year;
    this->month = month;
    this->day = day;
    initialised = true;
}

// TODO Write a function, getYear, that takes no parameters and returns year
// member variable
//  Date d = Date();
//  auto year = d.getYear();

unsigned int Date::getYear() const noexcept
{
    return year;
}

// TODO Write a function, getMonth, that takes no parameters and returns month
// member variable
//  Date d = Date();
//  auto month = d.getMonth();

unsigned int Date::getMonth() const noexcept
{
    return month;
}

// TODO Write a function, getDay, that takes no parameters and returns day
// member variable
//  Date d = Date();
//  auto day = d.getDay();

unsigned int Date::getDay() const noexcept
{
    return day;
}
// TODO Write an == operator overload for the Date class, such that two
// Date objects are equal only if they have the same member variables.
// Example
//  Date d1 = Date();
//   d1.setDate(2024, 1, 1);
//   Date d2 = Date();
//   d2.setDate(2024, 1, 2);
//   if (d1 == d2) {
//     ...
//   }

bool Date::operator==(const Date &other) const
{
    return year == other.year && month == other.month && day == other.day;
}

// TODO Write an < operator overload for the Date class, that returns true if
// the first Date object is chronologically before the second date object.
// Otherwise return false. Note: You can ignore the 'initialised' member
// variable when drawing the comparison.

//  Date d1 = Date();
//   d1.setDate(2024, 1, 1);
//   Date d2 = Date();
//   d2.setDate(2024, 1, 2);
//   if (d1 < d2) {
//     ...
//   }

bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}
