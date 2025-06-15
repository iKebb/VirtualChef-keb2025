#pragma once

#include <string>

/**
 * @brief Removes leading and trailing whitespace from a string.
 *
 * Trims text loaded from files (e.g., CSV or TXT), removing spaces at the start and end.
 * Example: "   Sasa Lele    " => "Sasa Lele"
 *
 * @note This function does NOT remove internal whitespace (e.g., "Sa sa Le le" remains unchanged).
 *
 * The function checks if the input string is empty and returns it immediately if so.
 *
 * It uses:
 * - `first`: to find the index of the first non-space character.
 * - `last`: to find the index of the last non-space character.
 *
 * Then it calls `substr(first, last - first + 1)` to extract the trimmed substring.
 *
 * Example:
 * ```cpp
 * std::string s = "   Sasa Lele   ";
 * // Indices: [0:' ', 1:' ', 2:' ', 3:'S', ..., 8:'e']
 * // first = 3, last = 8
 * // substr(3, 6) => "Sasa Lele"
 * ```
 *
 * @param s The input string to be trimmed.
 * @return A new string with leading and trailing spaces removed.
 */
std::string trim(const std::string &s);

/**
 * @brief Reads an integer input and validates it is within the given range [min, max].
 *
 * @param outValue Reference to store the valid integer.
 * @param min Minimum allowed value (inclusive).
 * @param max Maximum allowed value (inclusive).
 * @return true if input is valid, false otherwise.
 */
bool getIntegerInput(int &outValue, int min, int max);