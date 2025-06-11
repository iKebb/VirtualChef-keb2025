#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

/*MAJOR CHANGES
  - Code refraction: deleted MainMenu class; not necessary to be a class neither a fun.
  - Consistence: all code was reviewed so IN THEORY all names of everyting is more accurate and "correct"
  - Minor corrections: THE MAIN MENU NOW RETURN ITSELF TO THE MAIN MENU WHEN YOU EXIT THE SUBMENUS WOW!

  Structure is pretty much the same, some references can be updated in order to make the program less heavy on load
  bc it is not build thinking you can load over 9000 recipes or 16000 ingredients at a time.

  TODO: save added ingredients onto my ingredientd.txt file
*/

using json = nlohmann::json;

std::string trim(const std::string &s)
{
  if (s.empty())
    return s;

  size_t first = s.find_first_not_of(' ');
  size_t last = s.find_last_not_of(' ');
  return s.substr(first, last - first + 1);
  // trim text loaded from txt or csv
  // => "   Sasa Lele    " => "Sasa Lele"
}

class Ingredient
{
public:
  std::string name;
  int quantity;
  std::string unit;
  // Ingredient class.
  // myIngredient => milk, 180, ml
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
  // Recipe class.
  // myRecipe => 658, Spanish Omelette, [{eggs 4 units}, {potatoes 2 units}, {...}], Peel potatoes and blah blah blah blah.
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
  // vector of recipes is needed to storage, load and manage the recipes on program.
  // vector of ingredients is also needed by the same but also for knowing what recipes we can do.
  std::vector<Recipe> recipes;
  std::vector<Ingredient> ingredients;

public:
  void loadIngredientsFromFile(const std::string &filename);
  void manuallyAddIngredients();
  void showAllIngredients();

  void loadRecipesFromJson(const std::string &filename);
  void showAllRecipes();
  void showAvailableRecipes();
  void selectRecipe();
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
};

void RecipeManager::manuallyAddIngredients()
{
  std::string name, unit;
  int quantity;

  std::cout << "Ingredient Name: ";
  std::cin >> name;
  std::cout << "Quantity: ";
  std::cin >> quantity;
  std::cout << "Unit (make sure to use plural; mililiters, grams or spoons): ";
  std::cin >> unit;

  ingredients.emplace_back(trim(name), quantity, trim(unit));
  std::cout << "Ingredient" << name << " added succesfully!\n";
};

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
};

void RecipeManager::showAllRecipes()
{
  std::cout << "\nShowing ALL recipes on database\n"
            << std::endl;

  for (const auto &recipe : recipes)
  {
    std::cout << "  Recipe ID: " << recipe.id << std::endl;
    std::cout << "  Name: " << recipe.recipe_name << std::endl;
    /*std::cout << "  Ingredients:\n";
    for (const auto &ing : recipe.ingredients)
    {
      std::cout << "    - " << ing.name << ": " << ing.quantity << " " << ing.unit << std::endl;
    }
    std::cout << "  Instructions: \n"
              << recipe.instructions << std::endl;
    std::cout << std::endl;*/
    std::cout << std::endl;
  }
};

void RecipeManager::showAvailableRecipes()
{
  std::vector<std::string> availableRecipes;
  // la estandarización de unidades no existe, es un invento del gobierno para hacerme pensar de más
  // GENERADO CON [COPILOT], SUJETO A CAMBIOS

  std::cout << "\nShowing AVAILABLE Recipes\n"
            << std::endl;
  std::cout << "Available recipes with your ingredients, are: " << std::endl;

  for (const auto &recipe : recipes)
  {
    bool canMake = true;
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
        canMake = false;
        break;
      }
    }
    if (canMake)
    {
      std::cout << "- " << recipe.recipe_name << std::endl;
    }
  }

  for (const auto &name : availableRecipes)
  {
    std::cout << "- " << name << std::endl;
  }
  std::cout << std::endl;
};

void RecipeManager::selectRecipe()
{

  if (recipes.empty())
  {
    std::cout << "DUMNbo" << std::endl;
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

/*Execution seq:

  => loadIngredientsFromFile
  loads the ingredients from the .txt or .csv file
  and storage'em onto a ingredient vector in RecipeManager.

  => loadRecipesFromJson
  load sthe entire recipe file and storage them on the program,
  then, they're procesed onto a Recipe object.

  => showAvailableRecipes
  print the recipes only available by the ingredients
  loaded in the program.

  => manuallyAddIngredients
  adds new ingredients inputed by the user.

  => mergeIngredients
  merge the inputed ingredients onto the ingredients vector.


  THIS IS ALL IN THEORY. FOR NOW, JUST THE FIRST 2 STEPS WORKS
*/

void displayMenu()
{
  std::cout << "--- Virtual Chef ---" << std::endl;
  std::cout << "1. Show all recipes" << std::endl;
  std::cout << "2. Show available recipes" << std::endl;
  std::cout << "3. Select a recipe" << std::endl;
  std::cout << "4. Add ingredients manually" << std::endl;
  std::cout << "5. Show all ingredients" << std::endl;
  std::cout << "6. Exit" << std::endl;
  std::cout << "Choose an option: ";
}

int main()
{
  RecipeManager rm;
  const std::string i = "../data/ingredients.txt";
  const std::string r = "../data/recipes.json";

  rm.loadIngredientsFromFile(i);
  rm.loadRecipesFromJson(r);

  int choice;
  do
  {
    displayMenu(); // Displays full menu including prompt
    std::cin >> choice;

    switch (choice)
    {
    case 1:
      rm.showAllRecipes();
      break;
    case 2:
      rm.showAvailableRecipes();
      break;
    case 3:
      rm.selectRecipe();
      break;
    case 4:
      rm.manuallyAddIngredients();
      break;
    case 5:
      rm.showAllIngredients();
      break;
    case 6:
      std::cout << "Exiting program..." << std::endl;
      break;
    default:
      std::cout << "Input a number between 1 and 6." << std::endl;
    }
  } while (choice != 6);
  return 0;
}