/**
 * @file utils.cpp
 * @brief Implementation of utility functions for string and input handling.
 *
 * This file contains low-level utility functions commonly used throughout the application,
 * including string manipulation and robust user input validation routines.
 */

#include "../include/utils.hpp"
#include <iostream>
#include <limits>

/**
 * @brief Trims leading and trailing spaces from a string.
 *
 * Uses std::string's find methods to locate the first and last non-space characters.
 * If the string is empty or consists entirely of spaces, an empty string is returned.
 *
 * @note This function only removes ASCII space (' ') characters. For full whitespace trimming,
 *       consider using std::isspace with a custom loop.
 *
 * @param [in] s Input string to be trimmed
 * @return A new std::string object containing the trimmed result
 */
std::string trim(const std::string &s)
{
  if (s.empty())
    return s;
  size_t first = s.find_first_not_of(' ');
  size_t last = s.find_last_not_of(' ');
  return s.substr(first, last - first + 1);
};

/**
 * @brief Reads and validates integer input within a specified range.
 *
 * Attempts to read an integer from standard input and ensures:
 * - The input is a valid integer (not text or malformed input),
 * - It falls within the given min and max bounds (inclusive).
 *
 * Handles invalid input by clearing error flags and flushing the input buffer.
 *
 * @param [out] outValue Reference to store the validated integer
 * @param [in] min Minimum allowed value (inclusive)
 * @param [in] max Maximum allowed value (inclusive)
 * @return true if input was valid and within range, false otherwise
 */
bool getIntegerInput(int &outValue, int min, int max)
{
  // Attempt to extract integer from input stream

  if (!(std::cin >> outValue))
  {
    // Input extraction failed (e.g., user entered text instead of number)
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
    // Validate the integer is within the expected range
    std::cout << "Choice out of range. Input a number between "
              << min << " and " << max << ".\n"
              << std::endl;
    return false;
  }

  return true;
}