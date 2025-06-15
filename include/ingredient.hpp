/**
 * @file ingredient.hpp
 * @brief Definition of the Ingredient struct.
 *
 * This file contains a simple structure to represent an ingredient,
 * including its name, quantity and unit of measurement.
 */

#pragma once

#include <string>

/**
 * @struct Ingredient
 * @brief Represents an ingredient with name, quantity, and unit.
 *
 * This structure is used to store basic information about an ingredient.
 */

struct Ingredient
{
  std::string name; ///< Name of the ingredient (e.g., "Flour")
  int quantity;     ///< Quantity available or required
  std::string unit; ///< Unit of measurement (e.g., "grams", "ml", "units")
};