#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

using json = nlohmann::json;

// trim text
std::string trim(const std::string &s)
{
  if (s.empty())
    return s;

  size_t first = s.find_first_not_of(' ');
  size_t last = s.find_last_not_of(' ');
  return s.substr(first, last - first + 1);
}

class Ingredient
{
public:
  std::string name;
  int quantity;
  std::string unit;

  Ingredient(const std::string &name, int quantity, const std::string &unit)
      : name(name), quantity(quantity), unit(unit) {}
};

class Recipe
{
public:
  int id;
  std::string recipe_name;
  std::vector<Ingredient> ingredients;
  std::string instructions;

  Recipe(int id,
         const std::string &recipe_name,
         const std::vector<Ingredient> &ingredients,
         const std::string &instructions)
      : id(id),
        recipe_name(recipe_name),
        ingredients(ingredients),
        instructions(instructions)
  {
  }
};

class RecipeManager
{
private:
  std::vector<Recipe> recipes;
  std::vector<Ingredient> ingredients;

public:
  void loadIngredientsFromFile(const std::string &filename);
  void manuallyAddIngredients() const;
  void mergeIngredients() const;

  void loadRecipesFromJson(const std::string &filename);
  void availableRecipesLogic() const;
  void showAvailableRecipes(const std::string &filename);
};

// Methos beneath this comment

void RecipeManager::loadIngredientsFromFile(const std::string &filename)
{
  std::ifstream ingredientsFile(filename);
  if (!ingredientsFile.is_open())
  {
    std::cerr << "File " << filename << "not found or cannot be opened." << std::endl;
    return;
  }

  std::string line;
  while (std::getline(ingredientsFile, line))
  {
    std::istringstream ss(line);
    std::string name, quantityString, unit;

    if (std::getline(ss, name, ',') &&
        std::getline(ss, quantityString, ',') &&
        std::getline(ss, unit, ','))
    {
      name = trim(name);
      unit = trim(unit);
      int quantity = std::stoi(trim(quantityString));
      ingredients.emplace_back(Ingredient{name, quantity, unit});
    }
  }

  std::cout << "--- Ingredients loaded ---" << std::endl;

  for (const auto &ingredient : ingredients)
  {
    std::cout << "- " << ingredient.name
              << ": Quantity: " << ingredient.quantity
              << " " << ingredient.unit << std::endl;
  }
  std::cout << "" << std::endl;
};

void RecipeManager::loadRecipesFromJson(const std::string &filename)
{
  std::ifstream recipesFile(filename);
  if (!recipesFile.is_open())
  {
    std::cerr << "File " << filename << "not found or cannot be opened." << std::endl;
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
    for (const auto &ingredient : recipeJson["ingredients"])
    {
      std::string name = ingredient.value("name", "");
      int quantity = ingredient.value("quantity", 0);
      std::string unit = ingredient.value("unit", "");

      recipe_ingredients.emplace_back(name, quantity, unit);
    }
    recipes.emplace_back(Recipe{
        id, recipe_name, recipe_ingredients, instructions});
  }
  std::cout << "--- Recipes loaded ---" << std::endl;

  for (const auto &recipe : recipes)
  {
    std::cout << "- Recipe ID: " << recipe.id << std::endl;
    std::cout << "  Name: " << recipe.recipe_name << std::endl;
    std::cout << "  Ingredients:\n";
    for (const auto &ing : recipe.ingredients)
    {
      std::cout << "    - " << ing.name << ": " << ing.quantity << " " << ing.unit << std::endl;
    }
    std::cout << "  Instructions: " << recipe.instructions << "\n\n";
  }
};

// void RecipeManager::showAvailableRecipes(const std::string &filename) {};

/*
Execution sequency:

  => loadIngredientsFromFile
  loads the ingredients from the .txt or .csv file
  and storage'em onto a ingredient vector in RecipeManager.

  => loadRecipesFromJson
  load sthe entire recipe file and storage them on the program,
  then, they're procesed onto a Recipe object.

  => void availableRecipesLogic
  logic to process the loaded ingredients and recipes
  and waits for being call from showAvailableRecipes.

  => showAvailableRecipes
  print the recipes only available by the ingredients
  loaded in the program.

  => manuallyAddIngredients
  adds new ingredients inputed by the user.

  => mergeIngredients
  merge the inputed ingredients onto the ingredients vector.


  THIS IS ALL IN THEORY. FOR NOW, JUST THE FIRST 2 STEPS WORKS
*/

int main()
{
  RecipeManager manager;
  std::string i = "../data/ingredients.txt";
  std::string r = "../data/recipes.json";

  try
  {
    manager.loadIngredientsFromFile(i);
    manager.loadRecipesFromJson(r);
  }
  catch (const std::ifstream::failure &e)
  {
    std::cerr << "File operation failed: " << e.what() << '\n';
  }
  catch (const nlohmann::json::parse_error &e)
  {
    std::cerr << "JSON parse error: " << e.what() << '\n';
  }
  catch (const std::exception &e)
  {
    std::cerr << "An error occurred: " << e.what() << '\n';
  }
  catch (...)
  {
    std::cerr << "Unknown error occurred.\n";
  }

  return 0;
}