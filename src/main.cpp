#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include "../imports/nlohmann/json.hpp"

// in the documentation, we'll calling function as fun.
/* MAJOR CHANGES
  - Refactor: Deleted MainMenu class; it's not necessary to be a class or even a function.
  - Consistency: All code was reviewed so IN THEORY all names of everything are more accurate and "correct".
  - Minor corrections: THE MAIN MENU NOW RETURNS TO ITSELF WHEN YOU EXIT THE SUBMENUS, WOW!

  The structure is pretty much the same. Some references can be updated to make the program less heavy on load,
  because it is not built thinking you can load over 9000 recipes or 16,000 ingredients at a time.

  TODO list:acos
  TODO: Save added ingredients to my ingredients.txt file.
  TODO: Unit tests.
  TODO: Bug fixes:

    - Inputting a string for "choice" breaks the program, causing an infinite loop to begin endlessly.

    - For some reason (and I don't know why), sometimes loading ingredients at startup doesn't work and
      you need to load them manually using showAllIngredients().

    - Due to the lack of standardization on ingredients files, the program can break or not work correctly
      if you input wrong parameters to ingredients.txt, for example: [onion (singular), -10, units]; [onions, , un it]; or things like that.

  TODO: Refactor displayMenu (removing it) and make better menu functions; maybe a loop to keep adding ingredients or something.
  TODO: Visual user interface.
  TODO: Proper documentation of the project.
*/

/* json is an alias from nlohmann::json,
   it makes easier to use json by casting only as 'json'.
*/
using json = nlohmann::json;

/* trim is a fun. that erase blank spaces at start and end of a string
  trim text loaded from txt or csv
  => "   Sasa Lele    " => "Sasa Lele"
  !!! doesn't work on "Sa sa Le le"

  so we def our fun. 'trim' who gets a reference string '&s' and
  returns a new string.

  if ensures the string is not empty, so the fun. do not exec.

  size_t stores *index* on a string, so it works like this:
  => std::string myString= Sasa Lele
  => ['S'(0), 'a'(1), 's'(2), 'a'(3), ' '(4), 'L'(5), 'e'(6), 'l'(7), 'e'(8)]

  using param 'first', 'size_t' searchs the first non blank space
  => "   Sasa Lele(...)" => "S"
  using param 'last', 'size_t' searchs the last non blank space
  => "(...)Sasa Lele   " => "e"

  substr(first, last - first + 1) takes the first log char 'till
  the last index, in this case the last 'l' son "lele". We add +1
  to ensure substr takes all the string.

*/
std::string trim(const std::string &s)
{
  if (s.empty())
    return s;

  size_t first = s.find_first_not_of(' ');
  size_t last = s.find_last_not_of(' ');
  return s.substr(first, last - first + 1);
}

/* struct Ingredient (was a class b4)
ensures ALL the ingredients have the same structure.
the structure is the same described beneath: a name, its qty and its unit.
*/
struct Ingredient
{
  std::string name;
  int quantity;
  std::string unit;
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

// Methods beneath this comment

void RecipeManager::loadIngredientsFromFile(const std::string &filename)
{

  // !!! Dump checker
  /*ingredientsFile is a ifstream variable; it works as a file reader
    that allows the program manipulate the content it has AS A REFERENCE
    but not changing the content directly, instead, it manages teh state
    of the file. ingredientsFile is not a string variable, so if print it
    this won't shows the file content nor a memory dir, but a bool state.
  */
  std::ifstream ingredientsFile(filename);
  if (!ingredientsFile.is_open())
  {
    std::cerr << "File " << filename << "not found or cannot be opened." << std::endl;
    return;
  }

  std::string line;
  /*getlien() is a fun. who reads an entire line of text from a string,
    in this case, it scans the content the var 'ingredientsFile' grants to it
    so it can store THAT LINE onto the var 'line'. So, _while_ the file can
    be redable or is not at the end point of the file, this loop keelps looping;
    (we can use a for-each loop but a while one makes thing easier [for me])
    for every line our 'line' var reads, it asigns to a istringstream var type
    called 'ss'. We set 3 vars: name, qtyString, unit, and using an if we
    divide line on three parts: the first one starts at start of line 'till the
    first comma; the seccond one thus the first comma (at the very end of first part)
    'till the second comma, and the third one the same; from seccond comma to last comma or
    end of the text.

    At the end, we trim the variables and change atyString to int quantity using 'stoi',
    then, we trim it as well. Lastly, the program adds in order: name, quantity and unit
    to the vector ingredients.
    */
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
};

void RecipeManager::manuallyAddIngredients()
{
  bool s = true;
  while (s)
  {
    /*simple logic that set vars name, unit and qty for adding onto ingredients vector,
    input in order ingredient, quantity and unit and push it back to ingredients.
    */
    std::string name, unit;
    int quantity;

    std::cout << "Ingredient Name: ";
    std::cin >> name;
    std::cout << "Quantity: ";
    std::cin >> quantity;
    std::cout << "Unit: ";
    std::cin >> unit;

    ingredients.push_back({name, quantity, unit});

    std::cout << "Ingredient " << name << " added succesfully!\n";
    std::cout << "Want to add another ingredient? (say n to exit): ";
    std::string c;
    std::cin >> c;

    /*trims choice to avoid dumb inputs and transform the string 'c'
    from start to end to lowercase.
    => "   SaD oNiOn   " => "sad onion"
    */
    c = trim(c);
    std::transform(c.begin(), c.end(), c.begin(), ::tolower);
    if (c == "n" || c == "no")
    {
      s = false;
    }
  }
}

void RecipeManager::showAllIngredients()
{
  /*
  simple for each loop that shows every ingredient loaded on program.*/
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
  // !!! Dump checker
  std::ifstream recipesFile(filename);
  if (!recipesFile.is_open())
  {
    std::cerr << "File " << filename << "not found or cannot be opened." << std::endl;
    return;
  }

  // creates an empty jason var called j and stores the data from recipesFile
  json j;
  recipesFile >> j;

  /* for-each loop that reads and iterates over the array [j]. On every iteraiton,
    recipeJson being a const reference of the current obj (our recipe), using .value()
    emplace in our vars ther values.
    => id takes the value called "id", if there's nothing, set a 0 as df.
    => recipe_name takes the value "name", if there's nothing, set "" as df.

    for the ingredients, we use a nested for-each loop: who reads every ingredient
    of the array "ingredients" from the JSON file AND makes the same as with
    teh oters:
    => name(from ingredient) takes the value "name" or set "" if there's nothing.
    => quantity(from ingredient) takes the value "name" or set 0 if (...)
    => unit(from ingredient) takes the value "unit" or set "" if (...)

    lastly, it pushses back the vars to the vector recipe_ingredients. Then,
    outside the loop, the vector recipes stores the data in order.
  */
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
};

void RecipeManager::showAllRecipes()
{
  std::cout << "\nShowing ALL recipes on database\n"
            << std::endl;
  // for-each logic to read every recipe from vector recipes and shows in order.

  for (const auto &recipe : recipes)
  {
    std::cout << recipe.id << ". " << recipe.recipe_name << std::endl;
    std::cout << std::endl;
  }
};

void RecipeManager::showAvailableRecipes()
{
  std::cout << "Available recipes with your ingredients are:" << std::endl;

  /* the first for-each reads every Recipe on reipes vector. "recipe" is a
  constant reference of the ACTUAL recipe 'for' is reading.

  inside the [first]for, creates a bool 'isPossible' in true state;
  on the [second]for, loop through every <ingredient> on the recipe vector and
  stores onto a reference reqIngr. Then declares a false bool var called found,
  then, another nested for appears looping through every ingredient on ingredients vector,
  then, compares:

  => if the name coincides from what the user has to the program have
  => if the unit is the same
  => if has enough of that ingredient

  if every condition is true, found == true and the loops breaks and continues,
  but if found == false, isPossible's set to false and exit the main loop so
  that recipe cannot be mdde. Otherwise, if isPossible == true,
  prints the recipes name.
  */
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
      std::cout << "- " << recipe.recipe_name << std::endl;
    }
  }

  std::cout << std::endl;
};

void RecipeManager::selectRecipe()
{
  // !!! Dump checker
  if (recipes.empty())
  {
    std::cout << "DUMNbo" << std::endl;
    return;
  }

  // GENERADO CON [COPILOT], SUJETO A CAMBIOS
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
};

void displayMenu() // this fun. is so funny lolol
{
  std::cout << "--- Virtual Chef ---" << std::endl;
  std::cout << "1. Show all recipes" << std::endl;
  std::cout << "2. Show available recipes" << std::endl;
  std::cout << "3. Select a recipe" << std::endl;
  std::cout << "4. Show all ingredients" << std::endl;
  std::cout << "5. Add ingredients manually" << std::endl;
  std::cout << "6. Exit" << std::endl;
  std::cout << "Choose an option: ";
}

int main()
{
  RecipeManager rm;
  // relative path from output (where code exes) to files.
  const std::string i = "../../data/ingredients.txt";
  const std::string r = "../../data/recipes.json";

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
      rm.showAllIngredients();
      break;
    case 5:
      rm.manuallyAddIngredients();
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