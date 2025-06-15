/**
 * @file recipe.cpp
 * @brief Implementation of the Recipe class methods.
 *
 * This file contains the implementation of the Recipe class,
 * including its constructor and destructor.
 */

#include "../include/recipe.hpp"

/**
 * @brief Constructs a new Recipe object.
 *
 * Initializes a recipe with the given ID, name, list of ingredients,
 * and preparation instructions. The data is stored internally for use
 * in recipe management systems.
 *
 * Example usage:
 * ```cpp
 * Recipe recipe(101,
 *              "Cheese Burger",
 *              {{"Bread", 2, "slices"},
 *               {"Lettuce", 2, "slices"},
 *               {"Mayonnaise", 8, "grams"}},
 *              "Start by placing the burger patty on a heated grill...");
 * ```
 *
 * @param [in] id Unique identifier for the recipe
 * @param [in] recipe_name Name of the recipe
 * @param [in] ingredients List of required ingredients
 * @param [in] instructions Step-by-step preparation instructions
 */

Recipe::Recipe(int id,
               const std::string &recipe_name,
               const std::vector<Ingredient> &ingredients,
               const std::string &instructions)
    : id(id),
      recipe_name(recipe_name),
      ingredients(ingredients),
      instructions(instructions)
{
}

/**
 * @brief Destroys the Recipe object.
 *
 * Currently performs no additional cleanup as all internal members
 * are handled automatically by their destructors.
 */
Recipe::~Recipe() = default;