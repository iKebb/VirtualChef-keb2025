/**
 * @file recipe.hpp
 * @brief Definition of the Recipe class.
 *
 * This file contains the ´declaration´ of the Recipe class, wich represents a recipe
 * with an: ID, name, list of ingredients and preparation instructions.
 */

#pragma once

#include <vector>
#include <string>
#include "ingredient.hpp"

/**
 * @class Recipe
 * @brief Represents a recipe with ID, name, ingredients, and instructions.
 *
 * The Recipe class stores all relevant information about a recipe,
 * including its unique identifier, name, list of ingredients, and step-by-step instructions.
 */

class Recipe
{
public:
  int id;                              ///< Unique identifier for the recipe
  std::string recipe_name;             ///< Name of the recipe (e.g., "Chocolate Cake")
  std::vector<Ingredient> ingredients; ///< List of required ingredients
  std::string instructions;            ///< Step-by-step preparation instructions

  /**
   * @brief Constructs a new Recipe object.
   *
   * Initializes the recipe with the given ID, name, ingredients, and instructions.
   *
   * @param [in] id Unique identifier for the recipe
   * @param [in] recipe_name Name of the recipe
   * @param [in] ingredients List of ingredients needed
   * @param [in] instructions Preparation steps
   */

  Recipe(int id,
         const std::string &recipe_name,
         const std::vector<Ingredient> &ingredients,
         const std::string &instructions);

  /**
   * @brief Destructor for the Recipe class.
   *
   * Cleans up any resources used by the Recipe object.
   */
  ~Recipe();
};