# Virtual Chef v0.20.1

## What's New

- Improved code logic: fixed redundant logic and enhanced reference passing in `manuallyAddIngredients` and `selectRecipe`.
- Refactored `Ingredients`: now implemented as a struct for simplicity.
- Code reorganization:
  - Added header files for better modularity.
  - Moved method implementations to corresponding `.cpp` files.
  - Introduced a build manager with `CMakeLists.txt` for easier compilation.
- Enhanced input validation and error handling in user prompts.
- Updated documentation and code comments for clarity.
- Improved menu navigation.
- General code cleanup and formatting.

Virtual Chef is a terminal application that lets you manage recipes and check which dishes you can prepare based on the ingredients you have available.

## 📚 Table of Contents
- [What's New](#-whats-new)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Code Structure](#code-structure)
- [License](#license)
- [Known Issues](#known-issues)
- [Future Improvements](#future-improvements)
- [Acknowledgements](#acknowledgements)

## Installation

To get started with Virtual Chef, follow these steps:

```bash
# Clone the repository
git clone https://github.com/iKebb/VirtualChef-keb2025.git

# Enter the project directory
cd VirtualChef-keb2025

# Make sure you have C++ compiler to build and run the application.
```

## Usage

Compile and run the program. The main menu allows you to:

- View all recipes.
- Check available recipes based on your ingredients.
- Select and view recipe details.
- List all loaded ingredients.
- Add ingredients manually during execution.

Recipe data is loaded from JSON files and ingredients from CSV files located in the `data/` directory.

## Features

- **Browse Recipes:** View all stored recipes.
- **Filter Available Recipes:** Show only recipes you can prepare with your ingredients.
- **Recipe Details:** Access instructions and ingredients for each recipe.
- **Ingredient Management:** View and add ingredients manually.
- **Data Loading:** Ingredients from `.txt`, recipes from `.json` using the `nlohmann/json` library.

## Code Structure

- `main.cpp`: Entry point, initializes and displays the menu.
- `recipe.hpp/cpp`: `Recipe` class with metadata, ingredients, and instructions.
- `recipeManager.hpp/cpp`: Logic for loading, filtering, and displaying recipes and ingredients.
- `trim.hpp/cpp`: Utility for string trimming.
- `readme.md`: Project documentation.

**Dependencies:**
- `nlohmann/json` for JSON handling.
- Standard C++ libraries (`<fstream>`, `<sstream>`, `<algorithm>`, etc.).

## License

This project is licensed under the MIT License. You can modify and distribute it under its terms.

## Known Issues

- Infinite loop when entering spaces in ingredient names.
- Crashes when entering non-numeric values for quantities.
- Insufficient validation in the main menu and recipe selection.
- 'Yes/No' confirmation does not break the loop correctly when entering "n".
- Compiler error tolower() returns an int but it is asigned to a char.

## Future Improvements

- Improve input validation.
- Case-insensitive ingredient comparison.
- Standardize data and unit parsing.
- Expand the recipe database.
- Refactor code for clarity and better object orientation.
- Add search by name or ingredient.
- Save manually added ingredients.

## Acknowledgements

Thanks to the developers of `nlohmann/json` for their solution for handling JSON in modern C++.