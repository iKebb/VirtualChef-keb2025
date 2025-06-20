/**
 * @file recipe_manager.hpp
 * @brief Definition of the RecipeManager class.
 *
 * The RecipeManager class provides functionality to manage a collection of recipes
 * and ingredients. It supports loading data from files, displaying available information,
 * and selecting specific recipes for further actions.
 */

#pragma once

#include <vector>
#include <string>
#include "recipe.hpp"
#include "ingredient.hpp"

/**
 * @class RecipeManager
 * @brief Manages a collection of recipes and ingredients.
 *
 * This class allows users to load, view, and interact with recipes and their ingredients.
 * It maintains internal lists of available recipes and ingredients.
 */

class RecipeManager
{
private:
  std::vector<Recipe> recipes;               ///< Collection of all loaded recipes
  std::vector<SimpleIngredient> ingredients; ///< Collection of all loaded ingredients

public:
  /**
   * @brief Loads ingredients from a text file.
   *
   * Parses a file containing ingredient data and populates the internal list.
   *
   * @param [in] filename Path to the input file
   */
  void loadIngredientsFromFile(const std::string &filename);

  /**
   * @brief Allows the user to manually add ingredients via console input.
   *
   * Prompts the user to enter ingredient details until they choose to stop.
   */
  void manuallyAddIngredients();

  /**
   * @brief Displays all currently stored ingredients.
   *
   * Prints the name, quantity, and unit of each ingredient in the console.
   */
  void showAllIngredients();

  /**
   * @brief Loads recipes from a JSON file.
   *
   * Parses a JSON file containing recipe data and adds them to the internal list.
   *
   * @param [in] filename Path to the JSON file
   */
  void loadRecipesFromJson(const std::string &filename);

  /**
   * @brief Displays all loaded recipes.
   *
   * Prints the ID and name of each recipe to the console.
   */
  void showAllRecipes() const;

  /**
   * @brief Displays only the recipes that can be prepared with available ingredients.
   *
   * Checks which recipes can be made based on current ingredient stock and shows them.
   */
  void showAvailableRecipes() const;

  /**
   * @brief Lets the user select a recipe to prepare.
   *
   * Prompts the user to choose a recipe by ID and simulates its selection process.
   */
  void selectRecipe();
};