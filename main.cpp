#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include "imports/nlohmann/json.hpp"

/* MAJOR CHANGES
  - Refactor: Deleted MainMenu class; it's not necessary to be a class or even a function.
  - Consistency: All code was reviewed so IN THEORY all names of everything are more accurate and "correct".
  - Minor corrections: THE MAIN MENU NOW RETURNS TO ITSELF WHEN YOU EXIT THE SUBMENUS, WOW!

  The structure is pretty much the same. Some references can be updated to make the program less heavy on load,
  because it is not built thinking you can load over 9000 recipes or 16,000 ingredients at a time.

  TODO list
  TODO: Save added ingredients to my ingredients.txt file.
  TODO: Unit tests.
  TODO: Bug fixes:

    - Inputting a string for "choice" breaks the program, causing an infinite loop to begin endlessly.

    - For some reason (and I don't know why), sometimes loading ingredients at startup doesn't work and
      you need to load them manually using showAllIngredients().

    - Due to the lack of standardization on ingredients files, the program can break or not work correctly
      if you add wrong parameters to ingredients.txt, for example: [onion (singular), -10, units]; [onions, , un it]; or things like that.

  TODO: Refactor displayMenu (removing it) and make better menu functions; maybe a loop to keep adding ingredients or something.
  TODO: Visual user interface.
  TODO: Proper documentation of the project.
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
  // !!! doesn't work on "Sa sa Le le"
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

  // !!! Dump checker
  std::ifstream ingredientsFile(filename);
  if (!ingredientsFile.is_open())
  {
    std::cerr << "File " << filename << "not found or cannot be opened." << std::endl;
    return;
  }

  // the loop reads the file line per line and storage every line on te var "line"
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
  // GENERADO CON [COPILOT], SUJETO A CAMBIOS
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

void displayMenu() // this fun. is so funny lolol
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
  // relative path from output (where code exes) to files.
  const std::string i = "../data/ingredients.txt";
  const std::string r = "../data/recipes.json";

  rm.loadIngredientsFromFile(i);
  rm.loadRecipesFromJson(r);

  int choice;
  do
  {
    displayMenu();
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