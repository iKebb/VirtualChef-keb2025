#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

using json = nlohmann::json;

// => Trims leading and trailing spaces from a string
// Then "  Sasa Lele  " => "Sasa Lele"
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

// => Encapsulates recipe data on objects
class Recipe
{
public:
  int id;
  std::string recipe_name;
  std::vector<Ingredient> ingredients;
  std::string steps_preparation;

  // Constructor
  Recipe(const int id,
         std::string &recipe_name,
         std::vector<Ingredient> &ingredients,
         const std::string &steps_preparation)
      : id(id),
        recipe_name(recipe_name),
        ingredients(ingredients),
        steps_preparation(steps_preparation) {}
};

class IngredientManager
{
private:
  std::vector<Ingredient> ingredients;

public:
  void loadIngredientsFromFile(const std::string &filename);

  void showIngredients() const;

  void addIngredientsManually() const;
};

// => Manages collections of recipes
class RecipeManager
{
private:
  std::vector<Recipe> recipes;
  std::vector<Ingredient> availableIngredients;

public:
  void showAvilableRecipes() const;

  void showAllRecipes() const;

  void loadRecipesFromJson(const std::string &filename);
};

/* Methods are defined beneath this comment
 - showRecipes() => Shows available recipes based on loaded ingredients.
 - showAllRecipes() => Shows all recipes loaded in the program.
 - loadRecipesFromJson() => Loads recipes from json on program startup.
 - loadIngredientsFromFile() => Loads ingredients from a text file on program startup.
 - showIngredients() => Shows all available ingredients loaded from the file.
*/

/*
88                                                             88  88
88                                                             88  ""                            ,d
88                                                             88                                88
88  8b,dPPYba,    ,adPPYb,d8  8b,dPPYba,   ,adPPYba,   ,adPPYb,88  88   ,adPPYba,  8b,dPPYba,  MM88MMM  ,adPPYba,
88  88P'   `"8a  a8"    `Y88  88P'   "Y8  a8P_____88  a8"    `Y88  88  a8P_____88  88P'   `"8a   88     I8[    ""
88  88       88  8b       88  88          8PP"""""""  8b       88  88  8PP"""""""  88       88   88      `"Y8ba,
88  88       88  "8a,   ,d88  88          "8b,   ,aa  "8a,   ,d88  88  "8b,   ,aa  88       88   88,    aa    ]8I
88  88       88   `"YbbdP"Y8  88           `"Ybbd8"'   `"8bbdP"Y8  88   `"Ybbd8"'  88       88   "Y888  `"YbbdP"'
                  aa,    ,88
                   "Y8bbdP"
*/
void IngredientManager::loadIngredientsFromFile(const std::string &filename)
{
  std::ifstream ingrfile(filename);
  if (!ingrfile.is_open())
  {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(ingrfile, line))
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

void IngredientManager::showIngredients() const
{
  std::cout << "Available Ingredients:" << std::endl;
  for (const auto &ingredient : ingredients)
  {
    std::cout << "- " << ingredient.name
              << ": Quantity: " << ingredient.quantity
              << " " << ingredient.unit << std::endl;
  }
};

/*

88888888ba                          88
88      "8b                         ""
88      ,8P
88aaaaaa8P'  ,adPPYba,   ,adPPYba,  88  8b,dPPYba,    ,adPPYba,
88""""88'   a8P_____88  a8"     ""  88  88P'    "8a  a8P_____88
88    `8b   8PP"""""""  8b          88  88       d8  8PP"""""""
88     `8b  "8b,   ,aa  "8a,   ,aa  88  88b,   ,a8"  "8b,   ,aa
88      `8b  `"Ybbd8"'   `"Ybbd8"'  88  88`YbbdP"'    `"Ybbd8"'
                                        88
                                        88
*/

/*
88,dPYba,,adPYba,   ,adPPYYba,  88  8b,dPPYba,
88P'   "88"    "8a  ""     `Y8  88  88P'   `"8a
88      88      88  ,adPPPPP88  88  88       88
88      88      88  88,    ,88  88  88       88
88      88      88  `"8bbdP"Y8  88  88       88
*/

int main()
{
  RecipeManager recipeManager;
  IngredientManager ingredientManager;

  std::string recipeFile = "../../data/recipes.json";
  std::string ingredientFile = "../../data/ingredients.txt";

  ingredientManager.loadIngredientsFromFile(ingredientFile);
  // recipeManager.loadRecipesFromJson(recipeFile);

  ingredientManager.showIngredients();
  // recipeManager.showAllRecipes();
  return 0;
}