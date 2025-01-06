#include <iostream>
#include <fstream>
#include <string>

#include "menuItems.h"

using namespace std;
const int MAX_PRODUCTS = 100;
const int MAX_MENU_ITEMS1 = 100;
int menuItemCount = 0;

int productCount = 0;
Product products[MAX_PRODUCTS];

MenuItem menuItems1[MAX_MENU_ITEMS1];
void LoadProductsFromFile() {
    ifstream file("products.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open products file.\n";
        return;
    }

    int index = 0;
    string line;

    while (index < MAX_PRODUCTS && std::getline(file, line)) {
        // Read product name
        products[index].name = line;

        // Read quantity and QuantityInMenuItem for the product
        if (file >> products[index].quantity >> products[index].QuantityInMenuItem) {
            file.ignore(std::string::npos, '\n'); // Ignore the newline character after each product
            index++;
        }
        else {
            cout << "Error: File format incorrect for product at index " << index << ".\n";
            break;
        }
    }

    file.close();
    if (index == 0) {
        std::cout << "No products loaded.\n";
    }
    else {
        std::cout << "Loaded " << index << " products successfully.\n";
    }
}

void SaveMenuItems() {
    std::ofstream file("menuItems.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open menu items file for writing.\n";
        return;
    }

    // Write the menu items back to the file
    for (int i = 0; i < menuItemCount; ++i) {
        file << menuItems1[i].name << "\n";
        file << menuItems1[i].price << "\n";
        file << menuItems1[i].productCount << "\n";

        for (int j = 0; j < menuItems1[i].productCount; ++j) {
            file << menuItems1[i].products[j].name << "\n";
            file << menuItems1[i].products[j].quantity << " "
                << menuItems1[i].products[j].QuantityInMenuItem << "\n";
        }
    }

    file.close();
}
string ltrim(const string& s) {
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r')) {
        ++start;
    }
    return s.substr(start);
}

// Trim from the end (right)
string rtrim(const string& s) {
    size_t end = s.size();
    while (end > 0 && (s[end - 1] == ' ' || s[end - 1] == '\t' || s[end - 1] == '\n' || s[end - 1] == '\r')) {
        --end;
    }
    return s.substr(0, end);
}

// Trim from both ends
string trim1(const string& s) {
    return rtrim(ltrim(s));
}





// Function to print all menu items and their products
void PrintMenuItems() {
    ifstream file("menuItems.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open menu items file.\n";
        return;
    }

    int index = 0;

    while (index < MAX_MENU_ITEMS1 && std::getline(file, menuItems1[index].name)) {
        // Read the price of the menu item
        if (file >> menuItems1[index].price) {
            file.ignore(); // Ignore newline character after the price

            // Read the number of products
            if (file >> menuItems1[index].productCount) {
                file.ignore(); // Ignore newline character after product count

                // Load each product
                for (int i = 0; i < menuItems1[index].productCount; ++i) {
                    string productName;
                    int quantity, quantityInMenuItem;

                    // Read product details
                    if (std::getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                        file.ignore(); // Ignore the newline character after product details

                        // Assign product details
                        menuItems1[index].products[i].name = productName;
                        menuItems1[index].products[i].quantity = quantity;
                        menuItems1[index].products[i].QuantityInMenuItem = quantityInMenuItem;
                    }
                    else {
                        cout << "Error: File format incorrect for product at index "
                            << i << " in menu item " << menuItems1[index].name << ".\n";
                        break;
                    }
                }
                index++; // Move to the next menu item
            }
            else {
                cout << "Error: File format incorrect for product count in menu item "
                    << menuItems1[index].name << ".\n";
                break;
            }
        }
        else {
            cout << "Error: File format incorrect for price in menu item "
                << menuItems1[index].name << ".\n";
            break;
        }
    }

    file.close();

    if (index == 0) {
        std::cout << "No menu items loaded.\n";
    }
    else {
        std::cout << "Loaded " << index << " menu items successfully.\n";
    }

    // Print the loaded menu items
    for (int i = 0; i < index; ++i) {
        std::cout << "Menu Item: " << menuItems1[i].name << "\n";
        std::cout << "Price: $" << menuItems1[i].price << "\n";
        std::cout << "Products:\n";
        for (int j = 0; j < menuItems1[i].productCount; ++j) {
            std::cout << "  - " << menuItems1[i].products[j].name
                << " (Quantity: " << menuItems1[i].products[j].quantity
                << ", QuantityInMenuItem: " << menuItems1[i].products[j].QuantityInMenuItem << ")\n";
        }
        std::cout << "-----------------------------\n";
    }
}
void CreateMenuItem() {
    LoadProductsFromFile();
    MenuItem menuItem;

    // Input MenuItem details
    cout << "Enter Menu Item Name: ";
    getline(cin, menuItem.name);

    cout << "Enter Menu Item Price: ";
    cin >> menuItem.price;

    cout << "Enter the number of products used in this Menu Item: ";
    cin >> menuItem.productCount;
    cin.ignore(); // Clear the input buffer

    // Input details for each product
    for (int i = 0; i < menuItem.productCount; ++i) {
        cout << "Enter Product " << i + 1 << " Name: ";
        getline(cin, menuItem.products[i].name);

        cout << "Enter Quantity of " << menuItem.products[i].name << ": ";
        cin >> menuItem.products[i].quantity;

        cout << "Enter Quantity In Menu Item for " << menuItem.products[i].name << ": ";
        cin >> menuItem.products[i].QuantityInMenuItem;
        cin.ignore(); // Clear the input buffer
    }

    // Save the new MenuItem to the file
    ofstream outFile("menuItems.txt", ios::app); // Append to the file
    if (!outFile.is_open()) {
        cout << "Error: Unable to open menuItems.txt for writing.\n";
        return;
    }

    // Write MenuItem details to the file in the specified format
    outFile << menuItem.name << "\n";
    outFile << menuItem.price << "\n";
    outFile << menuItem.productCount << "\n";

    for (int i = 0; i < menuItem.productCount; ++i) {
        outFile << menuItem.products[i].name << "\n";
        outFile << menuItem.products[i].quantity << "\n";
        outFile << menuItem.products[i].QuantityInMenuItem << "\n";
    }

    outFile.close();
    cout << "Menu Item created successfully.\n";
}
void UpdateProductQuantityInSpecificMenuItem() {


    std::string menuItemName, productName;
    int newQuantityInMenuItem;

    // Input the MenuItem name, Product name, and the new QuantityInMenuItem value
    std::cout << "Enter Menu Item Name: ";
    std::getline(std::cin, menuItemName);

    std::cout << "Enter Product Name: ";
    std::getline(std::cin, productName);

    std::cout << "Enter new QuantityInMenuItem value: ";
    std::cin >> newQuantityInMenuItem;

    // Find the MenuItem and the Product within that MenuItem
    bool menuItemFound = false;
    bool productFound = false;

    for (int i = 0; i < menuItemCount; ++i) {
        if (menuItems1[i].name == menuItemName) {
            menuItemFound = true;

            for (int j = 0; j < menuItems1[i].productCount; ++j) {
                if (menuItems1[i].products[j].name == productName) {
                    productFound = true;
                    menuItems1[i].products[j].QuantityInMenuItem = newQuantityInMenuItem;
                    std::cout << "Product '" << productName << "' in MenuItem '" << menuItemName
                        << "' updated with new QuantityInMenuItem: " << newQuantityInMenuItem << "\n";
                    break;
                }
            }

            if (!productFound) {
                std::cout << "Error: Product '" << productName << "' not found in MenuItem '" << menuItemName << "'.\n";
            }

            break;
        }
    }

    if (!menuItemFound) {
        std::cout << "Error: MenuItem '" << menuItemName << "' not found.\n";
    }
}
void LoadProductsFromFileSub() {
    std::ifstream file("products.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open products file.\n";
        return;
    }

    int i = 0;
    std::string name;
    double quantity;
    int quantityInMenuItem;

    while (i < MAX_PRODUCTS && std::getline(file, name)) {
        // Clean up any extra spaces or newlines from the name
        name = trim1(name);
        if (name.empty()) {
            std::cout << "Error: Found empty product name at index " << i << ". Skipping...\n";
            continue;
        }

        // Read the quantity for the product
        if (!(file >> quantity)) {
            std::cout << "Error: Incorrect format for quantity at index " << i << ".\n";
            file.clear(); // Clear the error state
            file.ignore(std::string::npos, '\n');  // Skip to the next line
            continue;
        }

        // Read the QuantityInMenuItem for the product
        if (!(file >> quantityInMenuItem)) {
            std::cout << "Error: Incorrect format for QuantityInMenuItem at index " << i << ".\n";
            file.clear(); // Clear the error state
            file.ignore(std::string::npos, '\n');  // Skip to the next line
            continue;
        }

        file.ignore(std::string::npos, '\n');  // Ignore the newline character

        // Assign values to the product array
        products[i].name = name;
        products[i].quantity = quantity;
        products[i].QuantityInMenuItem = quantityInMenuItem;

        // Debugging output to verify that the product is loaded
        std::cout << "Loaded Product: " << products[i].name
            << ", Quantity: " << products[i].quantity
            << ", QuantityInMenuItem: " << products[i].QuantityInMenuItem << "\n";

        i++; // Move to the next product
    }

    file.close();
    if (i == 0) {
        std::cout << "No products loaded from the file.\n";
    }
    else {
        std::cout << "Loaded " << i << " products successfully.\n";
    }
}
void LoadMenuItems() {
    std::ifstream file("menuItems.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open menu items file.\n";
        return;
    }

    menuItemCount = 0;  // Reset the menu item count

    while (menuItemCount < MAX_MENU_ITEMS1 && !file.eof()) {
        // Read the menu item name
        std::string name;
        std::getline(file, name);
        if (name.empty()) continue;

        // Read the price
        double price;
        if (!(file >> price)) {
            std::cout << "Error: Incorrect format for price in menu item '" << name << "'.\n";
            break;
        }

        // Read the number of products in this menu item
        int productCount;
        if (!(file >> productCount)) {
            std::cout << "Error: Incorrect format for product count in menu item '" << name << "'.\n";
            break;
        }
        file.ignore();  // Ignore the newline after the product count

        // Initialize the menu item
        MenuItem& menuItem = menuItems1[menuItemCount];
        menuItem.name = name;
        menuItem.price = price;
        menuItem.productCount = productCount;

        // Read the products for this menu item
        for (int i = 0; i < productCount; ++i) {
            std::string productName;
            int quantity;
            int quantityInMenuItem;

            // Read the product name
            std::getline(file, productName);

            // Read the quantity and QuantityInMenuItem
            if (!(file >> quantity >> quantityInMenuItem)) {
                std::cout << "Error: Incorrect format for product details in menu item '" << name << "'.\n";
                break;
            }
            file.ignore();  // Ignore the newline after reading quantities

            // Assign values to the product in the menu item
            menuItem.products[i].name = productName;
            menuItem.products[i].quantity = quantity;
            menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;

            // Debug output to verify the loaded product
            std::cout << "Loaded product " << productName << " with quantity " << quantity
                << " and QuantityInMenuItem " << quantityInMenuItem << "\n";
        }

        // Increment the menu item count
        menuItemCount++;
    }

    file.close();
    std::cout << "Loaded " << menuItemCount << " menu items successfully.\n";
}
void SaveProducts() {
    std::ofstream file("products.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open products file for saving.\n";
        return;
    }

    // Write all products to the file
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        if (products[i].name.empty()) continue;  // Skip empty entries (if any)

        // Make sure we are writing the product's name, quantity, and QuantityInMenuItem
        file << products[i].name << "\n"
            << products[i].quantity << "\n"
            << products[i].QuantityInMenuItem << "\n";
    }

    file.close();  // Ensure the file is properly closed after writing
    std::cout << "Products saved successfully.\n";
}
void SubtractProductQuantityFromMenuItem(std::string menuItemName, std::string productName) {
    // Load menu items and products from files
    LoadMenuItems();
    LoadProductsFromFileSub();

    // Trim leading/trailing whitespaces
    menuItemName = trim1(menuItemName);
    productName = trim1(productName);

    // Load menu items and products from files
    LoadMenuItems();
    LoadProductsFromFileSub();

    bool menuItemFound = false;
    bool productFoundInMenuItem = false;
    bool productFoundInProducts = false;

    // Loop through the menu items to find the menu item
    for (int i = 0; i < menuItemCount; ++i) {
        // Trim the product name in the menu item for comparison
        std::string menuItemNameTrimmed = trim1(menuItems1[i].name);

        // Check if the menu item matches (after trimming)
        if (menuItemNameTrimmed == menuItemName) {
            menuItemFound = true;
            std::cout << "Menu Item '" << menuItemName << "' found.\n";

            // Loop through the products in the menu item to find the product
            for (int j = 0; j < menuItems1[i].productCount; ++j) {
                // Trim the product name in the menu item for comparison
                std::string productInMenuItemTrimmed = trim1(menuItems1[i].products[j].name);

                // Check if the product exists in the menu item
                if (productInMenuItemTrimmed == productName) {
                    productFoundInMenuItem = true;
                    std::cout << "Product '" << productName << "' found in Menu Item '" << menuItemName << "'.\n";

                    // Now look for the product in the global product list
                    for (int k = 0; k < MAX_PRODUCTS; ++k) {
                        // Trim the product name in the global product list for comparison
                        std::string globalProductTrimmed = trim1(products[k].name);

                        if (globalProductTrimmed == productName) {
                            productFoundInProducts = true;

                            // Subtract the quantity in the menu item from the global product quantity
                            products[k].quantity -= menuItems1[i].products[j].QuantityInMenuItem;
                            std::cout << "Updated quantity of product '" << productName << "': "
                                << products[k].quantity << "\n";

                            // Check if the quantity is 0 or below
                            if (products[k].quantity <= 0) {
                                std::cout << "Error: Product '" << productName << "' has insufficient stock. "
                                    << "Quantity is now " << products[k].quantity << ". Reload required.\n";
                            }

                            break; // Exit the product loop as we've found the product in the global list
                        }
                    }

                    if (!productFoundInProducts) {
                        std::cout << "Error: Product '" << productName << "' not found in the products list.\n";
                    }

                    break; // Exit the product loop as we've found the product in the menu item
                }
            }

            if (!productFoundInMenuItem) {
                std::cout << "Error: Product '" << productName << "' not found in Menu Item '" << menuItemName << "'.\n";
            }

            break; // Exit the menu item loop as we've found the menu item
        }
    }

    if (!menuItemFound) {
        std::cout << "Error: Menu Item '" << menuItemName << "' not found.\n";
    }

    // If everything is fine, save the updated product quantities
    if (menuItemFound && productFoundInMenuItem && productFoundInProducts) {
        SaveProducts();  // Save the updated products list
    }
}
void SubtractProductQuantityFromMenuItemTest() {
    // Input the menu item name and product name
    std::string menuItemName, productName;

    std::cout << "Enter the Menu Item Name: ";
    std::getline(std::cin, menuItemName);

    std::cout << "Enter the Product Name: ";
    std::getline(std::cin, productName);

    // Trim leading/trailing whitespaces
    menuItemName = trim1(menuItemName);
    productName = trim1(productName);

    // Load menu items and products from files
    LoadMenuItems();
    LoadProductsFromFileSub();

    bool menuItemFound = false;
    bool productFoundInMenuItem = false;
    bool productFoundInProducts = false;

    // Loop through the menu items to find the menu item
    for (int i = 0; i < menuItemCount; ++i) {
        // Trim the product name in the menu item for comparison
        std::string menuItemNameTrimmed = trim1(menuItems1[i].name);

        // Check if the menu item matches (after trimming)
        if (menuItemNameTrimmed == menuItemName) {
            menuItemFound = true;
            std::cout << "Menu Item '" << menuItemName << "' found.\n";

            // Loop through the products in the menu item to find the product
            for (int j = 0; j < menuItems1[i].productCount; ++j) {
                // Trim the product name in the menu item for comparison
                std::string productInMenuItemTrimmed = trim1(menuItems1[i].products[j].name);

                // Check if the product exists in the menu item
                if (productInMenuItemTrimmed == productName) {
                    productFoundInMenuItem = true;
                    std::cout << "Product '" << productName << "' found in Menu Item '" << menuItemName << "'.\n";

                    // Now look for the product in the global product list
                    for (int k = 0; k < MAX_PRODUCTS; ++k) {
                        // Trim the product name in the global product list for comparison
                        std::string globalProductTrimmed = trim1(products[k].name);

                        if (globalProductTrimmed == productName) {
                            productFoundInProducts = true;

                            // Subtract the quantity in the menu item from the global product quantity
                            products[k].quantity -= menuItems1[i].products[j].QuantityInMenuItem;
                            std::cout << "Updated quantity of product '" << productName << "': "
                                << products[k].quantity << "\n";

                            // Check if the quantity is 0 or below
                            if (products[k].quantity <= 0) {
                                std::cout << "Error: Product '" << productName << "' has insufficient stock. "
                                    << "Quantity is now " << products[k].quantity << ". Reload required.\n";
                            }

                            break; // Exit the product loop as we've found the product in the global list
                        }
                    }

                    if (!productFoundInProducts) {
                        std::cout << "Error: Product '" << productName << "' not found in the products list.\n";
                    }

                    break; // Exit the product loop as we've found the product in the menu item
                }
            }

            if (!productFoundInMenuItem) {
                std::cout << "Error: Product '" << productName << "' not found in Menu Item '" << menuItemName << "'.\n";
            }

            break; // Exit the menu item loop as we've found the menu item
        }
    }

    if (!menuItemFound) {
        std::cout << "Error: Menu Item '" << menuItemName << "' not found.\n";
    }

    // If everything is fine, save the updated product quantities
    if (menuItemFound && productFoundInMenuItem && productFoundInProducts) {
        SaveProducts();  // Save the updated products list
    }
}










