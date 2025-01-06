
#pragma once
#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
struct Product {
    string name;
    double quantity = 0.0;       // Initialize to 0.0
    int QuantityInMenuItem = 0;
};
void CreateProduct();
void DeleteProduct();
void PrintProducts();
void UpdateProductQuantity();
void UpdateProductQuantityInMenuItem();
//void SaveProducts();
#endif