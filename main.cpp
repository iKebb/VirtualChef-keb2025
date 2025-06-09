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
  std::vector<Ingredient>
      ingredients;
  std::string instructions;

  Recipe(const int id,
         std::string &recipe_name,
         std::vector<Ingredient> &ingredients,
         const std::string &instructions) {}
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
  void showAvilableRecipes(const std::string &filename);
};

// Methos beneath this comment

void RecipeManager::loadIngredientsFromFile(const std::string &filename)
{
  std::ifstream ingredientFile(filename);
  if (!ingredientFile.is_open())
  {
    std::cerr << "File not found or cannot be open: " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(ingredientFile, line))
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
      ingredients.emplace_back(name, quantity, unit);
    }
  }
}
