/**
 * @file main.cpp
 * @brief Startup point. Main entry of Virtual Chef.
 */

#include <iostream>
#include "../include/recipeManager.hpp"
#include "../include/trim.hpp"

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
   * @param rm                  Class RecipeManager instance that allows recipes management.
   * @param ingredientsFile     Relative path to the ingredients file.
   * @param recipesFile         Relative path to the recipes file.
   * @note Paths are relative to the current working directory.
   */
  RecipeManager rm;
  const std::string ingredientsFile = "../../data/ingredients.txt";
  const std::string recipesFile = "../../data/recipes.json";
  rm.loadIngredientsFromFile(ingredientsFile);
  rm.loadRecipesFromJson(recipesFile);

  int choice;
  do
  {
    /// @brief Shows startup menu on terminal
    std::cout << "--- Virtual Chef ---" << std::endl;
    std::cout << "1. Show all recipes" << std::endl;
    std::cout << "2. Show available recipes" << std::endl;
    std::cout << "3. Select a recipe" << std::endl;
    std::cout << "4. Show all ingredients" << std::endl;
    std::cout << "5. Add ingredients manually" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Choose an option: ";

    std::cin >> choice;
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
      std::cout << "Exiting program..." << std::endl; ///< Ends execution of program.
      break;
    default:
      std::cout << "Input a number between 1 and 6." << std::endl; ///< Dumb checker
    }
  } while (choice != 6);

  return 0;
};