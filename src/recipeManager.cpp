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
#include "trim.hpp"

using json = nlohmann::json;

/**
 * @brief Loads ingredients from a CSV file into the internal list.
 *
 * This function opens a file and reads it line by line using std::getline().
 * Each line is expected to be in the format:
 *   "name,quantity,unit"
 *
 * For each line:
 * - The string is split into three parts using ',' as delimiter.
 * - Leading/trailing whitespace is removed with trim().
 * - The quantity is converted from string to integer using std::stoi().
 * - A new Ingredient object is created and added to the internal vector.
 *
 * If the file cannot be opened, an error message is printed and the function returns early.
 *
 * @param filename Path to the ingredients CSV file.
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
 * @brief Allows the user to add ingredients manually via console input.
 *
 * Prompts the user for ingredient name, quantity, and unit of measure,
 * then stores the new ingredient in the internal ingredients vector.
 *
 * After each addition, the user is asked if they want to add another.
 * The loop continues until the user enters "n" or "no" (case-insensitive).
 *
 * @note Input is trimmed and converted to lowercase to handle whitespace
 *       and case variations gracefully. Example: "   SaD oNiOn   " → "sad onion".
 */
void RecipeManager::manuallyAddIngredients()
{
  bool s = true;
  while (s)
  {
    std::string name, unit;
    int quantity;

    std::cout << "Ingredient Name: ";
    std::cin >> name;

    std::cout << "Quantity: ";
    std::cin >> quantity;

    std::cout << "Unit: ";
    std::cin >> unit;

    ingredients.push_back({name, quantity, unit});
    std::cout << "Ingredient " << name << " added successfully!" << std::endl;

    bool validReponse = false;
    while (!validReponse)
    {
      std::cout << "Whant to add another ingredient? (y/n): " << std::endl;
      std::string c;
      std::cin >> c;
      c = trim(c);
      std::transform(c.begin(), c.end(), c.begin(), ::tolower);

      if (c == "y" || c == "yes")
      {
        validReponse = true;
      }
      else if (c == "n" || c == "no")
      {
        validReponse = false;
        s = false;
        break;
      }
      else
      {
        std::cout << "Please input 'y/yes' or 'n/no'" << std::endl;
      }
    };
  }
}

/**
 * @brief Displays all currently loaded ingredients to the user.
 *   simple for each loop that shows every ingredient loaded on program.

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
 * @brief Loads recipes from a JSON file into the internal list.
 *
 * This function opens and parses a JSON file using the nlohmann/json library.
 * Each recipe object in the JSON array is processed to extract:
 * - Recipe ID
 * - Name of the recipe
 * - Instructions
 * - List of required ingredients (name, quantity, unit)
 *
 * If any field is missing in the JSON, a default value is used:
 * - Missing ID → 0
 * - Missing name or unit → empty string
 * - Missing quantity → 0
 *
 * The extracted data is stored in a Recipe object and added to the internal recipes vector.
 *
 * @param filename Path to the JSON file containing recipes.
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
 * @brief Displays the names and IDs of all available recipes.
 *   // for-each logic to read every recipe from vector recipes and shows in order.
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
 * @brief Displays only the recipes that can be made with the current ingredients.
 *
 * Iterates through all stored recipes and checks if each one can be fully prepared
 * using the available ingredients. A recipe is considered available if:
 * - All required ingredients exist in the user's ingredient list.
 * - Each ingredient has sufficient quantity.
 * - The unit of measure matches exactly.
 *
 * If any required ingredient is missing or insufficient, the recipe is skipped.
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
 * @brief Allows the user to select a recipe and view its full details.
 *
 * Displays a numbered list of all available recipes. The user selects one by number.
 * If the selection is valid, the function shows:
 * - Recipe name
 * - List of required ingredients with quantities and units
 * - Full preparation instructions
 *
 * If there are no recipes or the selection is invalid, appropriate messages are shown.
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
  std::cin >> choice;
  if (choice < 1 || choice > static_cast<int>(recipes.size()))
  {
    std::cout << "Invalid selection." << std::endl;
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