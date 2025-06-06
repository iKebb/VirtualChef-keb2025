#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

using json = nlohmann::json;

/// @brief Class representing a recipe with its name, ingredients, and preparation steps. Its atributes are:
/// - `recipe_name`: The name of the recipe.
/// - `ingredients`: A map containing ingredient names and their respective quantities.
/// - `ingredients`: Quantities are represented in spanish units, such as: [unidad/es], [mililitros], [cucharadas], [tazas], [onzas].
/// - `steps_preparation`: A string containing the steps to prepare the recipe.
class Recipe
{
public:
  std::string recipe_name;
  std::map<std::string, double> ingredients;
  std::string steps_preparation;

  // Constructor
  Recipe(const std::string &recipe_name,
         const std::map<std::string, double> &ingredients,
         const std::string &steps_preparation)
      : recipe_name(recipe_name),
        ingredients(ingredients),
        steps_preparation(steps_preparation) {}
};

/// @brief Class to manage recipes, including loading from a file, showing recipes, and managing ingredients.
class recipeManager
{
private:
  std::vector<Recipe> recipes;

public:
  void showRecipes();
  void showAllRecipes();
  void loadRecipesFromFile();
  void mainMenu();

  void loadRecipesFromJson(const std::string &filename)
  {
    std::ifstream recfile("data/recipes.json");
    if (!recfile.is_open())
    {
      std::cerr << "Could not open the file: " << filename << std::endl;
      return;
    }

    json j;
    recfile >> j;
  };

  class ingredientManager
  {
  };
};
class Ingredient
{
public:
  std::string name;
  double quantity;
  std::string unit;

  // class constructor
  Ingredient(const std::string &name,
             double quantity,
             const std::string &unit)
      : name(name),
        quantity(quantity),
        unit(unit) {}
};

int main()
{
  // You can now create Recipe and Ingredient objects here
  return 0;
}