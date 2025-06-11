#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"
/*TODO list
  TODO: manuallyAddIngredients
  TODO: menu list (prob another class)
  TODO: ui (another class lmao)
  TODO: proper documentation
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

  void loadRecipesFromJson(const std::string &filename);
  void showAvailableRecipes();
  void selectRecipe();

  void showAllIngredients();
  void showAllRecipes();
};

class MainMenu
{
public:
  void startupMenu();
  void recipeMenu();
  void ingredientsMenu();

  void selectRecipe();
  void editIngredient();
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

void RecipeManager::manuallyAddIngredients() {};

void RecipeManager::showAllIngredients()
{
  std::cout << "--- Ingredients loaded ---" << std::endl;

  for (const auto &ingredient : ingredients)
  {
    std::cout << "- " << ingredient.name
              << ": Quantity: " << ingredient.quantity
              << " " << ingredient.unit << std::endl;
  }
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
  std::cout << "--- Recipes loaded ---" << std::endl;

  for (const auto &recipe : recipes)
  {
    std::cout << "  Recipe ID: " << recipe.id << std::endl;
    std::cout << "  Name: " << recipe.recipe_name << std::endl;
    std::cout << "  Ingredients:\n";
    for (const auto &ing : recipe.ingredients)
    {
      std::cout << "    - " << ing.name << ": " << ing.quantity << " " << ing.unit << std::endl;
    }
    std::cout << "  Instructions: \n"
              << recipe.instructions << std::endl;
  }
}

void RecipeManager::showAvailableRecipes()
{
  std::vector<std::string> availableRecipes;
  // SE NECESITA UNA LÓGICA PARA ESTANDARIZAR LAS UNIDADES
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
      availableRecipes.push_back(recipe.recipe_name);
    }
  }

  std::cout << "Available recipes with your ingredients, are: " << std::endl;
  for (const auto &name : availableRecipes)
  {
    std::cout << "- " << name << std::endl;
  }
};

void RecipeManager::selectRecipe()
{
  int choice = 0;

  std::cout << "Your available recipes are:\n";
  for (const auto &recipe : recipes)
  {
    std::cout << "- " << recipe.recipe_name << std::endl;
  }
  std::cout << "Select a recipe: ";
  std::cin >> choice;

  if (choice < 1 || choice > static_cast<int>(recipes.size()))
  {
    std::cout << "Invalid selection." << std::endl;
    return;
  }
  const Recipe &selected = recipes[choice - 1];
  std::cout << "Recipe ID: " << selected.id << std::endl;
  std::cout << "Name: " << selected.recipe_name << std::endl;
  std::cout << "Ingredients:" << std::endl;
  for (const auto &ing : selected.ingredients)
  {
    std::cout << "  - " << ing.name << ": " << ing.quantity << " " << ing.unit << std::endl;
  }
  std::cout << "Instructions:\n"
            << selected.instructions << std::endl;
}

void MainMenu::startupMenu()
{
  std::string choice;
  std::cout << "Virtual Chef" << std::endl
            << "--- ---" << std::endl;

  std::cout << "1. Open Recipe Menu" << std::endl;
  std::cout << "2. Open Ingredient Menu" << std::endl;
  std::cout << "3. Exit Program" << std::endl;
  std::cout << "Enter your choice: ";
  std::getline(std::cin, choice);

  switch (choice[0])
  {
  case '1':
    recipeMenu();
    break;
  case '2':
    ingredientsMenu();
    break;
  case '3':
    std::cout << "Exiting program." << std::endl;
    break;
  }
}

void MainMenu::recipeMenu()
{
  std::cout << "Virtual Chef: Recipe Menu" << std::endl
            << "--- ---" << std::endl;

  std::cout << "1. Show All Recipes" << std::endl;
  std::cout << "2. Show Available Recipes possibles with your current ingredients" << std::endl;
  std::cout << "3. Select a Recipe" << std::endl;
  std::cout << "4. Return to Main Menu" << std::endl;

  std::string choice;
  RecipeManager manager;

  switch (choice[0])
  {
  case '1':
    manager.showAllRecipes();
    break;
  case '2':
    manager.showAvailableRecipes();
    break;
  case '3':
    manager.selectRecipe();
    break;
  case '4':
  }
};

void MainMenu::ingredientsMenu()
{
  std::cout << "Virtual Chef: Ingredient Menu" << std::endl
            << "--- ---" << std::endl;

  std::cout << "1. Show All Ingredients" << std::endl;
  std::cout << "2. Edit Ingredients" << std::endl;
  std::cout << "3. Return to Main Menu" << std::endl;
};

void RecipeManager::selectRecipe() {};

void MainMenu::editIngredient() {};

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

int main()
{
  RecipeManager manager;
  std::string i = "../data/ingredients.txt";
  std::string r = "../data/recipes.json";

  try
  {
    manager.loadIngredientsFromFile(i);
    manager.loadRecipesFromJson(r);
    // manager.showAllIngredients();
    // manager.showAllRecipes();
    manager.showAvailableRecipes();
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
    std::cerr << "Unknown error.\n";
  }

  return 0;
}