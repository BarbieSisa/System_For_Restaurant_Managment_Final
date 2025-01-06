#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <vector>
#include "products.h"  

struct MenuItem {
    string name;         // Name of the menu item
    double price = 0.0;       // Price of the menu item
    int productCount = 0;     // Number of products in this menu item
    Product products[100]; // Fixed-size array of products


};
//
//void LoadProductsFromFile();
string ltrim(const string&);
string rtrim(const string&);
string trim1(const string&);
void LoadProductsFromFile();
void LoadMenuItems();
void LoadProductsFromFileSub();
void CreateMenuItem();
void PrintMenuItems();
void UpdateProductQuantityInSpecificMenuItem();
void SaveProducts();
void SubtractProductQuantityFromMenuItem(std::string, std::string);
void SubtractProductQuantityFromMenuItemTest();
#endif // MENUITEM_H
