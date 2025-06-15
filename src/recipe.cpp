/**
 * @file recipe.cpp
 * @brief Implementation of the Recipe class. Constructor and destructor.
 */

#include "../include/recipe.hpp"

/**
 * @brief Construct a new Recipe object.
 *
 * Example:
 * ```cpp
 * Recipe recipe(2002,
 *              "Cheese Burger",
 *              {{"Bread", 2, "slices"},
 *               {"Lettuce", 2, "slices"},
 *               {"Mayonnaise", 8, "grams"}},
 *              "Start by placing the burger patty on a heated grill...");
 * ```
 * @param id Unique identifier for the recipe.
 * @param recipe_name Name of the recipe.
 * @param ingredients List of required ingredients.
 * @param instructions Step-by-step instructions to prepare the recipe.
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

/// @brief Destroy the Recipe object.

Recipe::~Recipe() = default;