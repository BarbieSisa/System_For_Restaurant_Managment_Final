
#pragma once
#ifndef ORDERS_H
#define ORDERS_H

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "menuItems.h" 

struct Order {
    int ID = 0;
    string date;
    MenuItem menuItem;
};

void CreateOrder();
void DeleteOrder();
void LoadOrders();
void PrintOrders();
void SortOrdersByMenuItemNameDescending();
void SortOrdersByDate();
void PrintMenuItemOccurrences();
void SumPricesByDate();
double CalculateSumForDate(const string&);
void PrintPriceFromDate1();
bool ParseDate(const std::string&, int&, int&, int&);
string GetLatterDate(const string&, const string&);
string GetEarlierDate(const string&, const string&);
void PrintPriceFromDate();

#endif