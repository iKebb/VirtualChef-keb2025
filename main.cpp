#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

using json = nlohmann::json;

// Principal class to represent recipes
// => Encapsulates recipe data on objects
class Recipe
{
public:
  std::string recipe_name;
  std::map<std::string, double> ingredients;
  std::string steps_preparation;

  Recipe(const std::string &recipe_name,
         const std::map<std::string, double> &ingredients,
         const std::string &steps_preparation)
      : recipe_name(recipe_name),
        ingredients(ingredients),
        steps_preparation(steps_preparation) {}
};

// Class to manage ingredients (basic structure)
// => Encapsulates ingredient data on legible objects
class Ingredient
{
public:
  std::string name;
  double quantity;
  std::string unit;

  Ingredient(const std::string &name, double quantity, const std::string &unit)
      : name(name), quantity(quantity), unit(unit) {}
};

// Recipe Manager class
// => Manages collections of recipes and provides methods to manipulate'em
class RecipeManager
{
private:
  // Collection of recipes
  std::vector<Recipe> recipes;

public:
  // => shows avilable recipes based on ingredients loaded.
  void showRecipes() const;

  // => shows all recipes loaded in the manager
  void showAllRecipes() const;

  // => loads recipes from json on program startup
  void loadRecipesFromJson(const std::string &filename);
};

void RecipeManager::loadRecipesFromJson(const std::string &filename)
{

  /* glosary:
  - recfile => Recipe file
  - recipes => Our collection of recipes seen above
  - j => json object to parse the file
  - loadRec => Each recipe loaded from the json file to the program
  - ingrJson => Each ingredient loaded from the json file to the program
  - Qty, Qties => Quantity, Quantities => ingrQty means the quantity of the ingredient
  */
  std::ifstream recfile(filename);
  if (!recfile.is_open())
  {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return;
  }

  json j;
  recfile >> j;

  recipes.clear();

  for (const auto &loadRec : j)
  {
    std::string recipeName = loadRec["name"];
    std::string recipeSteps = loadRec["instructions"];

    std::map<std::string, double> recIngrMap;

    for (const auto &ingrJson : loadRec["ingredients"])
    {
      std::string ingrName = ingrJson["name"];
      double ingrQty = ingrJson["quantity"];
      recIngrMap[ingrName] = ingrQty;
    }

    recipes.emplace_back(recipeName, recIngrMap, recipeSteps);
  }
};