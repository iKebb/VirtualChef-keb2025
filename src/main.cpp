/**
 * @file main.cpp
 * @brief Main entry point of the Virtual Chef application.
 *
 * This file contains the program's starting point. It initializes the RecipeManager,
 * loads data from files, and provides a console-based menu for interacting with recipes
 * and ingredients.
 */

#include <iostream>
#include "../include/recipeManager.hpp"
#include "../include/utils.hpp"

int main()
{
  /**
   * @brief Initializes the RecipeManager and loads required data files.
   *
   * Loads:
   * - Ingredients from a text file.
   * - Recipes from a JSON file.
   *
   *
   * @param [in] rm                  Class RecipeManager instance that allows recipes management.
   * @param [in] ingredientsFile     Relative path to the ingredients file.
   * @param [in] recipesFile         Relative path to the recipes file.
   * @note Paths are relative to the current working directory.
   */
  RecipeManager rm;
  const std::string ingredientsFile = "../data/ingredients.txt";
  const std::string recipesFile = "../data/recipes.json";
  rm.loadIngredientsFromFile(ingredientsFile);
  rm.loadRecipesFromJson(recipesFile);

  int choice;
  do
  {
    // Display main menu options
    std::cout << "--- Virtual Chef ---" << std::endl;
    std::cout << "1. Show all recipes" << std::endl;
    std::cout << "2. Show available recipes" << std::endl;
    std::cout << "3. Select a recipe" << std::endl;
    std::cout << "4. Show all ingredients" << std::endl;
    std::cout << "5. Add ingredients manually" << std::endl;
    std::cout << "6. Load ingredients from file" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "Choose an option: ";

    if (!getIntegerInput(choice, 1, 7))
    {
      continue;
    };

    switch (choice)
    {
    case 1:
      rm.showAllRecipes(); ///< Shows all recipes of the JSON file.
      break;
    case 2:
      rm.showAvailableRecipes(); ///< Shows ONLY availabe recipes the user can do by their loaded ingredients.
      break;
    case 3:
      rm.selectRecipe(); ///< Allows the user to select a Recipe to show all their data.
      break;
    case 4:
      rm.showAllIngredients(); ///< Shows all ingredients loaded on program.
      break;
    case 5:
      rm.manuallyAddIngredients(); ///< Allows the user to manually add an ingredient to the program.
      break;
    case 6:
      std::cout << "To load new ingredients from a file, drop a txt or a csv with ingredients on 'data' folder";
      std::cout << " with the format: \negg\nsalt\npepper\netc...\n " << std::endl;
      std::cout << "Loading ingredients..." << std::endl;
      rm.loadIngredientsFromFile(recipesFile);
      break;
    case 7:
      std::cout << "Exiting program..." << std::endl; ///< Ends execution of program.
      break;
    }
  } while (choice != 7);

  return 0;
};