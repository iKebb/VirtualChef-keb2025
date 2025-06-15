/**
 * @file trim.cpp
 * @brief Implementation of the 'trim' function for _string_ manipulation.
 */

#include "../include/utils.hpp"
#include <iostream>
#include <limits>

std::string trim(const std::string &s)
{
  if (s.empty())
    return s;
  size_t first = s.find_first_not_of(' ');
  size_t last = s.find_last_not_of(' ');
  return s.substr(first, last - first + 1);
};

bool getIntegerInput(int &outValue, int min, int max)
{
  if (!(std::cin >> outValue))
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input.\n"
              << std::endl;
    return false;
  }
  else
  {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  if (outValue < min || outValue > max)
  {
    std::cout << "Choice out of range. Input a number between "
              << min << " and " << max << ".\n"
              << std::endl;
    return false;
  }

  return true;
}