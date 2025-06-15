#pragma once

#include <vector>
#include <string>
#include "recipe.hpp"
#include "ingredient.hpp"

class RecipeManager
{
private:
  std::vector<Recipe> recipes;
  std::vector<Ingredient> ingredients;

public:
  void loadIngredientsFromFile(const std::string &filename);
  void manuallyAddIngredients();
  void showAllIngredients();
  void loadRecipesFromJson(const std::string &filename);
  void showAllRecipes() const;
  void showAvailableRecipes() const;
  void selectRecipe();
};
