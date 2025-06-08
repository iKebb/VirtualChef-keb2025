#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

using json = nlohmann::json;

// => Encapsulates recipe data on objects
class Recipe
{
public:
  int id;
  std::string recipe_name;
  std::map<std::string, double> ingredients;
  std::string steps_preparation;

  Recipe(const int id,
         std::string &recipe_name,
         const std::map<std::string, double> &ingredients,
         const std::string &steps_preparation)
      : id(id),
        recipe_name(recipe_name),
        ingredients(ingredients),
        steps_preparation(steps_preparation) {}
};

// Class to manage ingredients (basic structure)
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
  std::vector<Recipe> recipes;

public:
  void showAvilableRecipes() const;

  void showAllRecipes() const;

  void loadRecipesFromJson(const std::string &filename);
};

/* Methods are defined beneath this comment
 - showRecipes() => Shows available recipes based on loaded ingredients.
 - showAllRecipes() => Shows all recipes loaded in the program.
 - loadRecipesFromJson() => Loads recipes from json on program startup.
*/

void RecipeManager::loadRecipesFromJson(const std::string &filename)
{

  /*glosary:
  - recfile => Recipe file
  - recipes => Our collection of recipes seen above
  - j => json object to parse the file
  - loadRec => Each recipe loaded from the json file to the program
  - Qty, Qties => Quantity, Quantities => ingrQty means the quantity of the ingredient
  - recIngrMap => Map of ingredients for each recipeq
  - ingr (short for ingredient) =>
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
    int recipeId = loadRec["id"];
    std::string recipeName = loadRec["name"];
    std::string recipeSteps = loadRec["instructions"];

    std::map<std::string, double> recIngrMap;

    for (const auto &ingrJson : loadRec["ingredients"])
    {
      std::string ingrName = ingrJson["name"];
      double ingrQty = ingrJson["quantity"];
      std::string ingrUnit = ingrJson["unit"];

      recIngrMap[ingrName] = ingrQty;
    }

    recipes.emplace_back(recipeId, recipeName, recIngrMap, recipeSteps);
  }
};

void RecipeManager::showAvilableRecipes() const
{
  std::cout << "Available Recipes:" << std::endl;
};

void RecipeManager::showAllRecipes() const
{
  std::cout << "Available Recipes:" << std::endl;
  for (const auto &recipe : recipes)
  {
    std::cout << "ID: " << recipe.id << std::endl;
    std::cout << "Recipe: " << recipe.recipe_name << std::endl;
    std::cout << "Ingredients:" << std::endl;

    for (const auto &ingredient : recipe.ingredients)
    {
      const std::string &name = ingredient.first;
      const double &quantity = ingredient.second;

      std::cout << "- " << name << ": " << quantity << std::endl;
    }
    std::cout << "Steps: " << recipe.steps_preparation << std::endl;
    std::cout << "------------------------" << std::endl;
  }
};

int main()
{
  RecipeManager manager;
  std::string filename = "../data/recipes.json";
  manager.loadRecipesFromJson(filename);
  manager.showAllRecipes();
  return 0;
}