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
        products[index].name = line;
        if (file >> products[index].quantity >> products[index].QuantityInMenuItem) {
            file.ignore(std::string::npos, '\n'); 
            index++;
        }
        else {
          //  cout << "Error: File format incorrect for product at index " << index << ".\n";
            break;
        }
    }

    file.close();
    if (index == 0) {
        std::cout << "No products loaded.\n";
    }
    else {
       // std::cout << "Loaded " << index << " products successfully.\n";
    }
}

void SaveMenuItems() {
    std::ofstream file("menuItems.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open menu items file for writing.\n";
        return;
    }

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

string rtrim(const string& s) {
    size_t end = s.size();
    while (end > 0 && (s[end - 1] == ' ' || s[end - 1] == '\t' || s[end - 1] == '\n' || s[end - 1] == '\r')) {
        --end;
    }
    return s.substr(0, end);
}

string trim1(const string& s) {
    return rtrim(ltrim(s));
}

void PrintMenuItems() {
    ifstream file("menuItems.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open menu items file.\n";
        return;
    }

    int index = 0;
    while (file) {
        MenuItem menuItem;

        // Read menu item name
        string line;
        if (!getline(file, line)) break;
        menuItem.name = trim1(line); // Trim whitespace

        // Read price
        if (getline(file, line)) {
            line = trim1(line); // Trim whitespace
            try {
                menuItem.price = std::stod(line); // Convert string to double
            }
            catch (const std::exception&) {
                cout << "Error: Incorrect format for price in menu item '" << menuItem.name << "'.\n";
                break;
            }
        }
        else {
            cout << "Error: Missing price for menu item '" << menuItem.name << "'.\n";
            break;
        }

        // Read product count
        if (getline(file, line)) {
            line = trim1(line); // Trim whitespace
            try {
                menuItem.productCount = std::stoi(line); // Convert string to integer
            }
            catch (const std::exception&) {
                cout << "Error: Incorrect format for product count in menu item '" << menuItem.name << "'.\n";
                break;
            }
        }
        else {
            cout << "Error: Missing product count for menu item '" << menuItem.name << "'.\n";
            break;
        }

        // Debug: Print product count
        cout << "DEBUG: Menu Item '" << menuItem.name << "' has " << menuItem.productCount << " products.\n";

        // Read product details
        for (int i = 0; i < menuItem.productCount; ++i) {
            string productName;
            int quantity = 0, quantityInMenuItem = 0;

            // Read product name
            if (!getline(file, productName)) {
                cout << "Error: Missing product name for product at index " << i
                    << " in menu item '" << menuItem.name << "'.\n";
                break;
            }
            productName = trim1(productName); // Trim whitespace

            // Read quantity
            if (getline(file, line)) {
                line = trim1(line); // Trim whitespace
                try {
                    quantity = std::stoi(line); // Convert string to integer
                }
                catch (const std::exception&) {
                    cout << "Error: Incorrect format for quantity at index " << i
                        << " in menu item '" << menuItem.name << "'.\n";
                    break;
                }
            }
            else {
                cout << "Error: Missing quantity for product at index " << i
                    << " in menu item '" << menuItem.name << "'.\n";
                break;
            }

            // Read QuantityInMenuItem
            if (getline(file, line)) {
                line = trim1(line); // Trim whitespace
                try {
                    quantityInMenuItem = std::stoi(line); // Convert string to integer
                }
                catch (const std::exception&) {
                    cout << "Error: Incorrect format for QuantityInMenuItem at index " << i
                        << " in menu item '" << menuItem.name << "'.\n";
                    break;
                }
            }
            else {
                cout << "Error: Missing QuantityInMenuItem for product at index " << i
                    << " in menu item '" << menuItem.name << "'.\n";
                break;
            }

            // Assign product details
            menuItem.products[i].name = productName;
            menuItem.products[i].quantity = quantity;
            menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;

            // Debug: Print product details
            cout << "DEBUG: Product[" << i << "] - Name: " << productName
                << ", Quantity: " << quantity
                << ", QuantityInMenuItem: " << quantityInMenuItem << "\n";
        }

        // Print menu item details
        cout << "Menu Item: " << menuItem.name << "\n";
        cout << "Price: $" << menuItem.price << "\n";
        cout << "Products:\n";
        for (int i = 0; i < menuItem.productCount; ++i) {
            const Product& product = menuItem.products[i];
            cout << "  - Product Name: " << product.name << "\n";
            cout << "    Quantity: " << product.quantity << ", ";
            cout << "Quantity in Menu Item: " << product.QuantityInMenuItem << "\n";
        }
        cout << "-------------------------------\n";

        index++; // Increment the index for menu items
    }

    file.close();

    if (index == 0) {
        cout << "No menu items loaded.\n";
    }
    else {
        cout << "Loaded " << index << " menu items successfully.\n";
    }
}
void CreateMenuItem() {
    LoadProductsFromFile();
    MenuItem menuItem;
    cout << "Enter Menu Item Name: ";
    cin.ignore();  // Clears the leftover newline from previous input
    getline(cin, menuItem.name);

    cout << "Enter Menu Item Price: ";
    while (!(cin >> menuItem.price)) { // Validate price input
        cout << "Invalid price. Please enter a numeric value: ";
        cin.clear();
        while (cin.get() != '\n'); // Manually clear input buffer
    }
    cin.ignore(); // Ensure no extra newline remains

    cout << "Enter the number of products used in this Menu Item (Max 100): ";
    while (!(cin >> menuItem.productCount) || menuItem.productCount <= 0 || menuItem.productCount > 100) {
        cout << "Invalid number. Please enter a positive integer (1-100): ";
        cin.clear();
        while (cin.get() != '\n'); // Manually clear input buffer
    }
    cin.ignore(); // Ensure no extra newline remains

    for (int i = 0; i < menuItem.productCount; ++i) {
        cout << "Enter Product " << i + 1 << " Name: ";
        getline(cin, menuItem.products[i].name);

        cout << "Enter Quantity of " << menuItem.products[i].name << ": ";
        while (!(cin >> menuItem.products[i].quantity)) {
            cout << "Invalid input. Enter a numeric quantity: ";
            cin.clear();
            while (cin.get() != '\n'); // Manually clear input buffer
        }
        cin.ignore(); // Ensure no extra newline remains

        cout << "Enter Quantity In Menu Item for " << menuItem.products[i].name << ": ";
        while (!(cin >> menuItem.products[i].QuantityInMenuItem)) {
            cout << "Invalid input. Enter a numeric quantity: ";
            cin.clear();
            while (cin.get() != '\n'); // Manually clear input buffer
        }
        cin.ignore(); // Ensure no extra newline remains
    }

    ofstream outFile("menuItems.txt", ios::app);
    if (!outFile) {
        cerr << "Error: Unable to open menuItems.txt for writing.\n";
        return;
    }

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
    std::cout << "Enter Menu Item Name: ";
    std::getline(std::cin, menuItemName);

    std::cout << "Enter Product Name: ";
    std::getline(std::cin, productName);

    std::cout << "Enter new QuantityInMenuItem value: ";
    std::cin >> newQuantityInMenuItem;
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
        name = trim1(name);
        if (name.empty()) {
           // std::cout << "Error: Found empty product name at index " << i << ". Skipping...\n";
            continue;
        }
        if (!(file >> quantity)) {
         //   std::cout << "Error: Incorrect format for quantity at index " << i << ".\n";
            file.clear(); 
            file.ignore(std::string::npos, '\n');  
            continue;
        }
        if (!(file >> quantityInMenuItem)) {
          //  std::cout << "Error: Incorrect format for QuantityInMenuItem at index " << i << ".\n";
            file.clear(); 
            file.ignore(std::string::npos, '\n');  
            continue;
        }

        file.ignore(std::string::npos, '\n');
        products[i].name = name;
        products[i].quantity = quantity;
        products[i].QuantityInMenuItem = quantityInMenuItem;
       /*std::cout << "Loaded Product: " << products[i].name
            << ", Quantity: " << products[i].quantity
            << ", QuantityInMenuItem: " << products[i].QuantityInMenuItem << "\n";
*/
        i++; 
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

    menuItemCount = 0;  

    while (menuItemCount < MAX_MENU_ITEMS1 && !file.eof()) {
        std::string name;
        std::getline(file, name);
        if (name.empty()) continue;
        double price;
        if (!(file >> price)) {
            std::cout << "Error: Incorrect format for price in menu item '" << name << "'.\n";
            break;
        }
        int productCount;
        if (!(file >> productCount)) {
            std::cout << "Error: Incorrect format for product count in menu item '" << name << "'.\n";
            break;
        }
        file.ignore();
        MenuItem& menuItem = menuItems1[menuItemCount];
        menuItem.name = name;
        menuItem.price = price;
        menuItem.productCount = productCount;
        for (int i = 0; i < productCount; ++i) {
            std::string productName;
            int quantity;
            int quantityInMenuItem;
            std::getline(file, productName);
            if (!(file >> quantity >> quantityInMenuItem)) {
                std::cout << "Error: Incorrect format for product details in menu item '" << name << "'.\n";
                break;
            }
            file.ignore();  
            menuItem.products[i].name = productName;
            menuItem.products[i].quantity = quantity;
            menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;

          /*  std::cout << "Loaded product " << productName << " with quantity " << quantity
                << " and QuantityInMenuItem " << quantityInMenuItem << "\n";
     */   }
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
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        if (products[i].name.empty()) continue;
        file << products[i].name << "\n"
            << products[i].quantity << "\n"
            << products[i].QuantityInMenuItem << "\n";
    }

    file.close();  
    std::cout << "Products saved successfully.\n";
}
void SubtractProductQuantityFromMenuItem(std::string menuItemName, std::string productName) {
    LoadMenuItems();
    LoadProductsFromFileSub();
    menuItemName = trim1(menuItemName);
    productName = trim1(productName);
    LoadMenuItems();
    LoadProductsFromFileSub();

    bool menuItemFound = false;
    bool productFoundInMenuItem = false;
    bool productFoundInProducts = false;
    for (int i = 0; i < menuItemCount; ++i) {
        std::string menuItemNameTrimmed = trim1(menuItems1[i].name);
        if (menuItemNameTrimmed == menuItemName) {
            menuItemFound = true;
            std::cout << "Menu Item '" << menuItemName << "' found.\n";

            for (int j = 0; j < menuItems1[i].productCount; ++j) {
                std::string productInMenuItemTrimmed = trim1(menuItems1[i].products[j].name);
                if (productInMenuItemTrimmed == productName) {
                    productFoundInMenuItem = true;
                    std::cout << "Product '" << productName << "' found in Menu Item '" << menuItemName << "'.\n";
                    for (int k = 0; k < MAX_PRODUCTS; ++k) {
                        std::string globalProductTrimmed = trim1(products[k].name);

                        if (globalProductTrimmed == productName) {
                            productFoundInProducts = true;
                            products[k].quantity -= menuItems1[i].products[j].QuantityInMenuItem;
                            std::cout << "Updated quantity of product '" << productName << "': "
                                << products[k].quantity << "\n";
                            if (products[k].quantity <= 0) {
                                std::cout << "Error: Product '" << productName << "' has insufficient stock. "
                                    << "Quantity is now " << products[k].quantity << ". Reload required.\n";
                            }

                            break; 
                        }
                    }

                    if (!productFoundInProducts) {
                        std::cout << "Error: Product '" << productName << "' not found in the products list.\n";
                    }

                    break; 
                }
            }

            if (!productFoundInMenuItem) {
                std::cout << "Error: Product " << productName << "' not found in Menu Item '" << menuItemName << "'.\n";
            }

            break; 
        }
    }

    if (!menuItemFound) {
        std::cout << "Error: Menu Item '" << menuItemName << "' not found.\n";
    }
    if (menuItemFound && productFoundInMenuItem && productFoundInProducts) {
        SaveProducts();  
    }
}
void SubtractProductQuantityFromMenuItemTest() {
    std::string menuItemName, productName;

    std::cout << "Enter the Menu Item Name: ";
    std::getline(std::cin, menuItemName);

    std::cout << "Enter the Product Name: ";
    std::getline(std::cin, productName);
    menuItemName = trim1(menuItemName);
    productName = trim1(productName);
    LoadMenuItems();
    LoadProductsFromFileSub();

    bool menuItemFound = false;
    bool productFoundInMenuItem = false;
    bool productFoundInProducts = false;
    for (int i = 0; i < menuItemCount; ++i) {
        std::string menuItemNameTrimmed = trim1(menuItems1[i].name);
        if (menuItemNameTrimmed == menuItemName) {
            menuItemFound = true;
            std::cout << "Menu Item '" << menuItemName << "' found.\n";
            for (int j = 0; j < menuItems1[i].productCount; ++j) {
                std::string productInMenuItemTrimmed = trim1(menuItems1[i].products[j].name);
                if (productInMenuItemTrimmed == productName) {
                    productFoundInMenuItem = true;
                    std::cout << "Product '" << productName << "' found in Menu Item '" << menuItemName << "'.\n";
                    for (int k = 0; k < MAX_PRODUCTS; ++k) {
                        std::string globalProductTrimmed = trim1(products[k].name);

                        if (globalProductTrimmed == productName) {
                            productFoundInProducts = true;

                            products[k].quantity -= menuItems1[i].products[j].QuantityInMenuItem;
                            std::cout << "Updated quantity of product '" << productName << "': "
                                << products[k].quantity << "\n";
                            if (products[k].quantity <= 0) {
                                std::cout << "Error: Product '" << productName << "' has insufficient stock. "
                                    << "Quantity is now " << products[k].quantity << ". Reload required.\n";
                            }

                            break; 
                        }
                    }

                    if (!productFoundInProducts) {
                        std::cout << "Error: Product '" << productName << "' not found in the products list.\n";
                    }

                    break; 
                }
            }

            if (!productFoundInMenuItem) {
                std::cout << "Error: Product '" << productName << "' not found in Menu Item '" << menuItemName << "'.\n";
            }

            break; 
        }
    }

    if (!menuItemFound) {
        std::cout << "Error: Menu Item '" << menuItemName << "' not found.\n";
    }

    if (menuItemFound && productFoundInMenuItem && productFoundInProducts) {
        SaveProducts();  
    }
}
void DeleteMenuItem() {
    ifstream inFile("menuItems.txt");
    ofstream outFile("temp.txt");

    if (!inFile || !outFile) {
        cout << "Error: Unable to open file.\n";
        return;
    }

    string menuItemName;
    MenuItem menuItem;

    cout << "Enter the menu item name to delete: ";
    cin.ignore();
    getline(cin, menuItemName);

    bool menuItemFound = false;

    while (!inFile.eof()) {
        // Read menu item name
        getline(inFile, menuItem.name);
        if (menuItem.name.empty()) continue; // Skip empty lines

        // Read price
        inFile >> menuItem.price;
        if (inFile.fail()) {
            cout << "Error: Invalid price format for menu item '" << menuItem.name << "'. Skipping...\n";
            inFile.clear();
            inFile.ignore(1000, '\n');
            continue;
        }

        // Read product count
        inFile >> menuItem.productCount;
        if (inFile.fail() || menuItem.productCount < 0 || menuItem.productCount > 100) {
            cout << "Error: Incorrect product count for menu item '" << menuItem.name << "'. Skipping...\n";
            inFile.clear();
            inFile.ignore(1000, '\n');
            continue;
        }

        inFile.ignore(); // Clear newline before reading products

        // Read product details
        for (int i = 0; i < menuItem.productCount; i++) {
            getline(inFile, menuItem.products[i].name);
            inFile >> menuItem.products[i].quantity >> menuItem.products[i].QuantityInMenuItem;
            inFile.ignore(); // Clear newline
        }

        // If menu item matches the one to delete, skip writing it
        if (menuItem.name == menuItemName) {
            menuItemFound = true;
            continue;
        }

        // Write the menu item to temp file
        outFile << menuItem.name << "\n";
        outFile << menuItem.price << "\n";
        outFile << menuItem.productCount << "\n";

        for (int i = 0; i < menuItem.productCount; ++i) {
            outFile << menuItem.products[i].name << "\n";
            outFile << menuItem.products[i].quantity << "\n";
            outFile << menuItem.products[i].QuantityInMenuItem << "\n";
        }
    }

    inFile.close();
    outFile.close();

    if (menuItemFound) {
        if (remove("menuItems.txt") != 0) {
            cout << "Error: Unable to delete the original file.\n";
            return;
        }
        if (rename("temp.txt", "menuItems.txt") != 0) {
            cout << "Error: Unable to rename the temp file.\n";
            return;
        }
        cout << "Menu item \"" << menuItemName << "\" deleted successfully.\n";
    }
    else {
        cout << "Menu item \"" << menuItemName << "\" not found.\n";
        if (remove("temp.txt") != 0) {
            cout << "Error: Unable to delete the temp file.\n";
        }
    }
}











