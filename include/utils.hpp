/**
 * @file input_utils.hpp
 * @brief Utility functions for input handling.
 *
 * This file contains general-purpose utility functions commonly used
 * for processing user input and string manipulation.
 */

#pragma once
#include <string>

/**
 * @brief Removes leading and trailing whitespace from a string.
 *
 * Returns a new string with all whitespace characters removed
 * from the beginning and end of the input string.
 *
 * @param [in] s Input string to trim
 * @return A trimmed copy of the input string
 */
std::string trim(const std::string &s);

/**
 * @brief Gets an integer input from the user within a specified range.
 *
 * Prompts the user to enter an integer value and validates it against
 * the given minimum and maximum values.
 *
 * @param [out] outValue Variable where the valid input will be stored
 * @param [in] min Minimum allowed value (inclusive)
 * @param [in] max Maximum allowed value (inclusive)
 * @return true if a valid integer was entered, false otherwise
 */
bool getIntegerInput(int &outValue, int min, int max);