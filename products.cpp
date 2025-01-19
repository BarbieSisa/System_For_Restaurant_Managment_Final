#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "products.h"
#include <stdexcept> // For std::runtime_error
using namespace std;
const string filename = "products.txt";
const int MAX_PRODUCTS1 = 100;
vector<Product> productList;
Product products1[100];  // Array of products, adjust size as needed
int productCount1 = 0;

void CreateProduct() {
    Product newProduct;

    // Input product name
    std::cin.clear(); // Clear error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the buffer completely
    std::cout << "Enter Product Name: ";
    std::getline(std::cin, newProduct.name);

    // Check if the product name already exists
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string name, quantityStr, quantityInMenuItemStr;

        while (std::getline(file, name)) {
            if (!std::getline(file, quantityStr) || !std::getline(file, quantityInMenuItemStr)) {
                std::cout << "Error: File is corrupted or improperly formatted.\n";
                file.close();
                return;
            }

            if (name == newProduct.name) {
                file.close();
                std::cout << "Product with this name already exists.\n";
                return;
            }
        }
        file.close();
    }
    else {
        std::cout << "Error: Could not open file for reading.\n";
        return;
    }

    // Input validation for product quantity
    while (true) {
        std::cout << "Enter Product Quantity: ";
        if (std::cin >> newProduct.quantity && newProduct.quantity >= 0) {
            break;
        }
        std::cout << "Invalid input. Please enter a valid positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Input validation for QuantityInMenuItem
    while (true) {
        std::cout << "Enter Quantity In Menu Item: ";
        if (std::cin >> newProduct.QuantityInMenuItem && newProduct.QuantityInMenuItem >= 0) {
            break;
        }
        std::cout << "Invalid input. Please enter a valid positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Save the new product to the file
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open()) {
        std::cout << "Error: Could not open file for writing.\n";
        return;
    }

    outFile << newProduct.name << '\n'
        << newProduct.quantity << '\n'
        << newProduct.QuantityInMenuItem << '\n';

    outFile.close();
    std::cout << "Product added successfully.\n";
}

void DeleteProduct() {
    ifstream inFile("products.txt");
    ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error: Unable to open file.\n";
        return;
    }

    string productName;
    Product product;

    cout << "Enter the product name to delete: ";
    cin.ignore();
    getline(cin, productName);

    bool productFound = false;

    // Read from the original file and write to a temp file except the product to delete
    while (inFile >> ws && getline(inFile, product.name) &&
        inFile >> product.quantity >> product.QuantityInMenuItem) {
        if (product.name == productName) {
            productFound = true; // Mark the product as found and skip writing it
            continue;
        }
        outFile << product.name << "\n"
            << product.quantity << "\n"
            << product.QuantityInMenuItem << "\n";
    }

    inFile.close();
    outFile.close();

    if (productFound) {
        // Safely handle file replacement
        if (remove("products.txt") != 0) {
            cout << "Error: Unable to delete the original file.\n";
            return;
        }
        if (rename("temp.txt", "products.txt") != 0) {
            cout << "Error: Unable to rename the temp file.\n";
            return;
        }
        cout << "Product \"" << productName << "\" deleted successfully.\n";
    }
    else {
        cout << "Product \"" << productName << "\" not found.\n";
        if (remove("temp.txt") != 0) {
            cout << "Error: Unable to delete the temp file.\n";
        }
    }
}




void PrintProducts() {
    std::ifstream file("products.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open file.\n";
        return;
    }

    Product product;

    // Read and print products from file
    std::cout << "Product List:\n";
    while (file >> std::ws && std::getline(file, product.name) &&
        file >> product.quantity >> product.QuantityInMenuItem) {
        std::cout << "Name: " << product.name << ", "
            << "Quantity: " << product.quantity << ", "
            << "QuantityInMenuItem: " << product.QuantityInMenuItem << "\n";
    }

    file.close();
}

void UpdateProductQuantity() {
    std::ifstream inFile("products.txt");
    std::ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error: Unable to open file.\n";
        return;
    }

    std::string productName;
    Product product;
    double newQuantity;

    std::cout << "Enter the product name to update: ";
    std::cin.ignore();
    std::getline(std::cin, productName);

    // Find and update the product
    bool productFound = false;
    while (inFile >> std::ws && std::getline(inFile, product.name) &&
        inFile >> product.quantity >> product.QuantityInMenuItem) {
        if (product.name == productName) {
            productFound = true;
            std::cout << "Enter new quantity for " << productName << ": ";
            std::cin >> newQuantity;

            // Update the quantity
            product.quantity = newQuantity;
        }

        // Write to temp file
        outFile << product.name << "\n"
            << product.quantity << "\n"
            << product.QuantityInMenuItem << "\n";
    }

    inFile.close();
    outFile.close();

    if (productFound) {
        if (std::remove("products.txt") != 0) {
            cout << "Error: Unable to delete original file.\n";
            return;
        }
        if (std::rename("temp.txt", "products.txt") != 0) {
            cout << "Error: Unable to rename temp file.\n";
            return;
        }
        std::cout << "Product updated successfully.\n";
    }
    else {
        cout << "Product \"" << productName << "\" not found.\n";
        std::remove("temp.txt");
    }
}



void UpdateProductQuantityInMenuItem() {
    ifstream inFile("products.txt");
    ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error: Unable to open file.\n";
        return;
    }

    string productName;
    Product product;
    bool productFound = false;
    bool newUsedInDish;

    cout << "Enter the product name to update: ";
    cin.ignore();
    getline(cin, productName);

    // Read each line from the file
    while (getline(inFile, product.name)) {
        inFile >> product.quantity >> product.QuantityInMenuItem;  // Read quantity and current QuantityInMenuItem
        inFile.ignore();  // Clear the newline character from the input stream after reading int values

        // Check if the current line corresponds to the product to be updated
        if (product.name == productName) {
            productFound = true;

            cout << "Enter new UsedInDish status for " << productName << " (0 for No, 1 for Yes): ";
            cin >> newUsedInDish;

            // Update the QuantityInMenuItem status
            product.QuantityInMenuItem = newUsedInDish;

            // Skip the next line to avoid writing it twice
            inFile.ignore(std::string::npos, '\n');
        }

        // Write to temp file
        outFile << product.name << "\n"
            << product.quantity << "\n"
            << product.QuantityInMenuItem << "\n";
    }

    inFile.close();
    outFile.close();

    if (productFound) {
        // Replace original file with updated temp file
        remove("products.txt");
        rename("temp.txt", "products.txt");
        cout << "Product UsedInDish status updated successfully.\n";
    }
    else {
        cout << "Product \"" << productName << "\" not found.\n";
        remove("temp.txt");
    }
}
// Global vector to hold the products

// Function to load products from file into the global products list
