/**
 * @file recipeManager.cpp
 * @brief Implementation of the RecipeManager class methods for recipe management.
 *
 * This file contains all method definitions for the RecipeManager class,
 * which manages recipes and ingredients from files and user input.
 */

#include "../include/recipeManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include "../imports/nlohmann/json.hpp"
#include "utils.hpp"

using json = nlohmann::json;

/**
 * @brief Loads ingredients from a CSV file into the internal list.
 *
 * The function opens the specified file and reads it line-by-line using std::getline().
 * Each line is expected to follow this format:
 *   "name,quantity,unit"
 *
 * For each line:
 * - The string is split using ',' as delimiter with std::getline()
 * - Leading/trailing spaces are trimmed using trim()
 * - Quantity is parsed via std::stoi()
 * - A new Ingredient object is constructed and added to the vector
 *
 * If the file cannot be opened, an error message is printed and execution stops early.
 *
 * @param [in] filename Path to the CSV file containing ingredients data
 */
void RecipeManager::loadIngredientsFromFile(const std::string &filename)
{
  std::ifstream ingredientsFile(filename);
  if (!ingredientsFile.is_open())
  {
    std::cerr << "File " << filename << " not found or cannot be opened." << std::endl;
    return;
  }
  std::string line;
  while (std::getline(ingredientsFile, line))
  {
    std::istringstream ss(line);
    std::string name, qtyString, unit;
    if (std::getline(ss, name, ',') &&
        std::getline(ss, qtyString, ',') &&
        std::getline(ss, unit, ','))
    {
      name = trim(name);
      unit = trim(unit);
      int quantity = std::stoi(trim(qtyString));
      ingredients.push_back({name, quantity, unit});
    }
  }
}

/**
 * @brief Allows the user to manually add ingredients via console input.
 *
 * Iteratively prompts the user to enter:
 * - Ingredient name
 * - Quantity (validated with getIntegerInput)
 * - Unit of measure
 *
 * After adding one ingredient, the user is asked whether to continue.
 * Input is trimmed and normalized to handle whitespace and case variations.
 *
 * @note This function uses std::ws to skip leading whitespace when reading lines.
 */
void RecipeManager::manuallyAddIngredients()
{
  bool isAdd = true;
  while (isAdd)
  {
    std::string name, unit;
    int quantity;

    std::cout << "Adding a new ingredient." << std::endl;

    std::cout << "Ingredient name: ";
    std::getline(std::cin >> std::ws, name);

    std::cout << "Quantity: ";
    if (!getIntegerInput(quantity, 1, 999999))
    {
      std::cout << "Invalid number. Please enter a natural (positive) number" << std::endl;
      continue;
    }

    std::cout << "Unit: ";
    std::getline(std::cin >> std::ws, unit);

    ingredients.push_back({name, quantity, unit});
    std::cout << "Ingredient " << name << " added successfully!" << std::endl;

    int choice;
    do
    {
      std::cout << "1. Yes\n";
      std::cout << "2. No\n";
      std::cout << "Want to add another ingredient? (1/2): " << std::endl;
    } while (!getIntegerInput(choice, 1, 2));

    if (choice == 2)
    {
      isAdd = false;
    }
  }
}

/**
 * @brief Displays all currently loaded ingredients to the console.
 *
 * Iterates over the internal ingredients vector and prints:
 * - Name
 * - Quantity
 * - Unit
 *
 * Output format:
 *   - Name: Quantity Unit
 */
void RecipeManager::showAllIngredients()
{
  std::cout << "\nShowing ALL ingredients on program\n"
            << std::endl;
  for (const auto &ingredient : ingredients)
  {
    std::cout << "- " << ingredient.name
              << ": Quantity: " << ingredient.quantity
              << " " << ingredient.unit << std::endl;
  }
  std::cout << std::endl;
}

/**
 * @brief Loads recipes from a JSON file using the nlohmann/json library.
 *
 * Parses a JSON array where each object represents a recipe with:
 * - id (optional, defaults to 0)
 * - name (optional, defaults to "")
 * - instructions (optional, defaults to "")
 * - ingredients (array of objects with name, quantity, unit)
 *
 * For missing fields, default values are used. Ingredients are validated
 * and constructed with empty strings or zero quantity if not present.
 *
 * @param [in] filename Path to the JSON file containing recipe data
 */
void RecipeManager::loadRecipesFromJson(const std::string &filename)
{
  std::ifstream recipesFile(filename);
  if (!recipesFile.is_open())
  {
    std::cerr << "File " << filename << " not found or cannot be opened." << std::endl;
    return;
  }
  json j;
  recipesFile >> j;
  for (const auto &recipeJson : j)
  {
    int id = recipeJson.value("id", 0);
    std::string recipe_name = recipeJson.value("name", "");
    std::string instructions = recipeJson.value("instructions", "");
    std::vector<Ingredient> recipe_ingredients;
    for (const auto &ingJson : recipeJson["ingredients"])
    {
      std::string name = ingJson.value("name", "");
      int quantity = ingJson.value("quantity", 0);
      std::string unit = ingJson.value("unit", "");
      recipe_ingredients.push_back({name, quantity, unit});
    }
    recipes.push_back(Recipe(id, recipe_name, recipe_ingredients, instructions));
  }
}

/**
 * @brief Displays the ID and name of every stored recipe.
 *
 * Iterates through the internal recipes vector and prints:
 * - ID followed by ". "
 * - Recipe name
 * - Outpud: "1. Spanish Omelette"
 */
void RecipeManager::showAllRecipes() const
{
  std::cout << "\nShowing ALL recipes on database\n"
            << std::endl;
  for (const auto &recipe : recipes)
  {
    std::cout << recipe.id << ". " << recipe.recipe_name << std::endl;
  }
  std::cout << "" << std::endl;
}

/**
 * @brief Shows only recipes that can be made with current ingredients.
 *
 * For each recipe, checks:
 * - All required ingredients exist in the ingredients list
 * - Quantities match or exceed what's needed
 * - Units of measure are identical
 *
 * A recipe is shown only if all its ingredients meet these conditions.
 */
void RecipeManager::showAvailableRecipes() const
{
  std::cout << "\nAvailable recipes with your ingredients are:\n"
            << std::endl;
  for (const auto &recipe : recipes)
  {
    bool isPossible = true;
    for (const auto &reqIngr : recipe.ingredients)
    {
      bool found = false;
      for (const auto &haveIngr : ingredients)
      {
        if (reqIngr.name == haveIngr.name &&
            reqIngr.unit == haveIngr.unit &&
            haveIngr.quantity >= reqIngr.quantity)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        isPossible = false;
        break;
      }
    }
    if (isPossible)
    {
      std::cout << recipe.id << ". " << recipe.recipe_name << std::endl;
    }
  }
  std::cout << std::endl;
}

/**
 * @brief Allows the user to select a recipe and view full details.
 *
 * Lists all available recipes and prompts the user to choose one by number.
 * On valid selection, displays:
 * - Recipe name
 * - Required ingredients (name, quantity, unit)
 * - Instructions
 *
 * If no recipes are available or the selection is invalid, appropriate messages are shown.
 */
void RecipeManager::selectRecipe()
{
  if (recipes.empty())
  {
    std::cout << "No recipes available." << std::endl;
    return;
  }
  std::cout << "\nAvailable recipes:\n"
            << std::endl;
  for (size_t i = 0; i < recipes.size(); ++i)
  {
    std::cout << i + 1 << ". " << recipes[i].recipe_name << std::endl;
  }
  int choice;
  std::cout << "Select a recipe (number): ";
  if (!getIntegerInput(choice, 1, static_cast<int>(recipes.size())))
  {
    return;
  }
  const Recipe &selected = recipes[choice - 1];
  std::cout << "\n--- " << selected.recipe_name << " ---" << std::endl;
  std::cout << "Ingredients:" << std::endl;
  for (const auto &ing : selected.ingredients)
  {
    std::cout << "- " << ing.name << ": "
              << ing.quantity << " " << ing.unit << std::endl;
  }
  std::cout << "\nInstructions: " << selected.instructions << std::endl;
  std::cout << std::endl;
}