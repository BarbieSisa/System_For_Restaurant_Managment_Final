#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "orders.h"
#include "menuItems.h"

const int MAX_MENU_ITEMS = 100;
const int MAX_ORDERS = 100;
const int MAX_DATES = 100;
int ordersCount = 0;
int menuItemsCount = 0;

//int menuItemCount = 0;

MenuItem menuItems[MAX_MENU_ITEMS];
Order orders[MAX_ORDERS];
string dates[MAX_DATES];  // Array to hold unique dates
double priceSum[MAX_DATES];  // Array to hold total prices for each date
int dateCount = 0;  // To track the number of unique dates


void DeleteOrder() {
    ifstream inFile("orders.txt");
    ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error: Unable to open file.\n";
        return;
    }

    int orderIDToDelete;
    cout << "Enter Order ID to delete: ";
    cin >> orderIDToDelete;

    bool orderFound = false;

    while (true) {
        int orderID;
        string date, menuItemName;
        double menuItemPrice;
        int productCount;

        // Read order ID
        if (!(inFile >> orderID))
            break; // End of file

        // Read order details
        inFile.ignore(); // Skip newline
        getline(inFile, date);
        getline(inFile, menuItemName);
        inFile >> menuItemPrice >> productCount;

        // Read products for this order
        string productName;
        int quantity, price;

        string productsData = "";
        for (int i = 0; i < productCount; ++i) {
            inFile.ignore(); // Skip newline
            getline(inFile, productName);
            inFile >> quantity >> price;
            productsData += productName + "\n" + to_string(quantity) + "\n" + to_string(price) + "\n";
        }

        if (orderID == orderIDToDelete) {
            orderFound = true; // Skip writing this order
            continue;
        }

        // Write the current order to the temp file
        outFile << orderID << "\n"
            << date << "\n"
            << menuItemName << "\n"
            << menuItemPrice << "\n"
            << productCount << "\n"
            << productsData;
    }

    inFile.close();
    outFile.close();

    // Replace the original file if the order was found
    if (orderFound) {
        if (remove("orders.txt") != 0) {
            cout << "Error: Could not delete the original file.\n";
            return;
        }
        if (rename("temp.txt", "orders.txt") != 0) {
            cout << "Error: Could not rename temp file.\n";
            return;
        }
        cout << "Order deleted successfully.\n";
    }
    else {
        cout << "Order ID " << orderIDToDelete << " not found.\n";
        remove("temp.txt");
    }
}
void CreateOrder() {
    ifstream file("menuItems.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open menu items file.\n";
        return;
    }

    int index = 0;

    while (index < MAX_MENU_ITEMS && getline(file, menuItems[index].name)) {
        // Read the price of the menu item
        if (file >> menuItems[index].price) {
            file.ignore(); // Ignore newline character after the price

            // Read the number of products
            if (file >> menuItems[index].productCount) {
                file.ignore(); // Ignore newline character after product count

                // Load each product
                for (int i = 0; i < menuItems[index].productCount; ++i) {
                    string productName;
                    int quantity, quantityInMenuItem;

                    // Read product details
                    if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                        file.ignore(); // Ignore the newline character after product details

                        // Assign product details
                        menuItems[index].products[i].name = productName;
                        menuItems[index].products[i].quantity = quantity;
                        menuItems[index].products[i].QuantityInMenuItem = quantityInMenuItem;
                    }
                    else {
                        cout << "Error: File format incorrect for product at index "
                            << i << " in menu item " << menuItems[index].name << ".\n";
                        break;
                    }
                }
                index++; // Move to the next menu item
            }
            else {
                cout << "Error: File format incorrect for product count in menu item "
                    << menuItems[index].name << ".\n";
                break;
            }
        }
        else {
            cout << "Error: File format incorrect for price in menu item "
                << menuItems[index].name << ".\n";
            break;
        }
    }

    file.close();

    if (index == 0) {
        cout << "No menu items loaded.\n";
    }
    else {
      //  cout << "Loaded " << index << " menu items successfully.\n";
    }
    Order newOrder;

    // Input Order details
    cout << "Enter Order ID: ";
    cin >> newOrder.ID;
    cin.ignore(); // Clear input buffer

    cout << "Enter Order Date: ";
    getline(cin, newOrder.date);

    cout << "Enter Menu Item Name: ";
    getline(cin, newOrder.menuItem.name);

    // Check if the menu item exists in the loaded menu items
    bool menuItemExists = false;
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        if (menuItems[i].name == newOrder.menuItem.name) {
            // Menu item found, copy its details into the order
            newOrder.menuItem.price = menuItems[i].price;
            newOrder.menuItem.productCount = menuItems[i].productCount;
            for (int j = 0; j < menuItems[i].productCount; ++j) {
                newOrder.menuItem.products[j] = menuItems[i].products[j];
            }
            menuItemExists = true;
            break;
        }
    }
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        if (menuItems[i].name == newOrder.menuItem.name) {

            for (int j = 0; j < menuItems[i].productCount; ++j) {
                string newOrderMenuItemName = newOrder.menuItem.name;
                string newOrderMenuItemProductName = newOrder.menuItem.products[j].name;
                SubtractProductQuantityFromMenuItem(newOrderMenuItemName, newOrderMenuItemProductName);
            }
            menuItemExists = true;
            break;
        }
    }

    if (!menuItemExists) {
        cout << "Error: Menu Item \"" << newOrder.menuItem.name << "\" does not exist.\n";
        return;
    }

    // Write order to file
    ofstream outFile("orders.txt", ios::app);
    if (!outFile.is_open()) {
        cout << "Error: Could not open orders file for writing.\n";
        return;
    }

    // Save order details
    outFile << newOrder.ID << "\n"
        << newOrder.date << "\n"
        << newOrder.menuItem.name << "\n"
        << newOrder.menuItem.price << "\n";

    // Write product details for the order
    outFile << newOrder.menuItem.productCount << "\n";
    for (int i = 0; i < newOrder.menuItem.productCount; ++i) {
        outFile << newOrder.menuItem.products[i].name << "\n"
            << newOrder.menuItem.products[i].quantity << "\n"
            << newOrder.menuItem.products[i].QuantityInMenuItem << "\n";  // Include QuantityInMenuItem
    }

    outFile.close();
    cout << "Order created successfully.\n";

}

void LoadOrders() {

    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    int index = 0;
    while (index < MAX_ORDERS && file >> orders[index].ID) {
        file.ignore(std::string::npos, '\n'); // Ignore the newline character after ID

        // Read order date
        getline(file, orders[index].date);

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName);

        // Match menu item
        bool found = false;
        for (int i = 0; i < menuItemsCount; ++i) {
            if (menuItems[i].name == menuItemName) {
                orders[index].menuItem = menuItems[i]; // Assign the menu item
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Error: Menu item '" << menuItemName << "' not found for order ID " << orders[index].ID << ". Skipping this order.\n";
            continue; // Skip to the next order
        }

        // Read product details for this order
        int productCount;
        file >> productCount;
        file.ignore(std::string::npos, '\n'); // Ignore the newline after product count
        orders[index].menuItem.productCount = productCount;

        // Read each product
        for (int i = 0; i < productCount; ++i) {
            string productName;
            double productQuantity;
            int productQuantityInMenuItem;

            getline(file, productName);  // Product name
            file >> productQuantity >> productQuantityInMenuItem;
            file.ignore(std::string::npos, '\n');  // Ignore the newline character after the product details

            // Assign product details to the order's menu item
            orders[index].menuItem.products[i].name = productName;
            orders[index].menuItem.products[i].quantity = productQuantity;
            orders[index].menuItem.products[i].QuantityInMenuItem = productQuantityInMenuItem;
        }

        index++;
    }

    ordersCount = index;
    file.close();

    if (ordersCount == 0) {
        cout << "No valid orders loaded.\n";
    }
    else {
        cout << "Loaded " << ordersCount << " orders successfully.\n";
    }
}

void PrintOrders() {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    int index = 0;
    while (file) {
        Order order;

        // Read order ID
        if (!(file >> order.ID)) break;
        file.ignore(); // Ignore newline after order ID

        // Read order date
        getline(file, order.date);

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName);
        order.menuItem.name = menuItemName;

        // Read price
        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        // Read number of products in menu item
        if (!(file >> order.menuItem.productCount)) break;
        file.ignore(); // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            // Read product details
            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore(); // Ignore the newline character after product details

                // Assign product details
                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        // Print order details
        cout << "Order ID: " << order.ID << "\n";
        cout << "Order Date: " << order.date << "\n";
        cout << "Menu Item: " << order.menuItem.name << "\n";
        cout << "Price: $" << order.menuItem.price << "\n";
        cout << "Products in Menu Item:\n";
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            const Product& product = order.menuItem.products[i];
            cout << "  - Product Name: " << product.name << "\n";
            cout << "    Quantity: " << product.quantity << ", ";
            cout << "Quantity in Menu Item: " << product.QuantityInMenuItem << "\n";
        }
        cout << "-------------------------------\n";

        index++; // Increment the index for orders
    }

    file.close();
}

void SortOrdersByMenuItemNameDescending() {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    // Read all orders into the array
    while (file) {
        if (ordersCount >= MAX_ORDERS) break;  // Stop if we exceed the max orders count

        Order order;

        // Read order ID
        if (!(file >> order.ID)) break;
        file.ignore(); // Ignore the newline after order ID

        // Read order date
        getline(file, order.date);  // This reads the full date line
        if (order.date.empty())  // If there is no date line, ignore
            continue;

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName); // Read the menu item name
        order.menuItem.name = menuItemName;

        // Read price
        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        // Read number of products in menu item
        if (!(file >> order.menuItem.productCount)) break;
        file.ignore(); // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            // Read product details
            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore(); // Ignore the newline character after product details

                // Assign product details
                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        orders[ordersCount++] = order;  // Store the order in the array and increment orders count
    }

    file.close();

    if (ordersCount == 0) {
        cout << "No orders to display.\n";
        return;
    }

    // Sorting the orders by MenuItem name using a simple bubble sort
    for (int i = 0; i < ordersCount; ++i) {
        for (int j = i + 1; j < ordersCount; ++j) {
            if (orders[i].menuItem.name > orders[j].menuItem.name) {
                swap(orders[i], orders[j]); // Swap the orders
            }
        }
    }

    // Print sorted orders
    for (int i = 0; i < ordersCount; ++i) {
        cout << "Order ID: " << orders[i].ID << "\n";
        cout << "Order Date: " << orders[i].date << "\n";
        cout << "Menu Item: " << orders[i].menuItem.name << "\n";
        cout << "Price: $" << orders[i].menuItem.price << "\n";
        cout << "Products in Menu Item:\n";
        for (int j = 0; j < orders[i].menuItem.productCount; ++j) {
            const Product& product = orders[i].menuItem.products[j];
            cout << "  - Product Name: " << product.name << "\n";
            cout << "    Quantity: " << product.quantity << ", ";
            cout << "Quantity in Menu Item: " << product.QuantityInMenuItem << "\n";
        }
        cout << "-------------------------------\n";
    }
}

void PrintMenuItemOccurrences() {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    string menuItems[MAX_MENU_ITEMS];
    int itemCount[MAX_MENU_ITEMS] = { 0 };  // Array to store the counts of each menu item
    int uniqueMenuItemCount = 0;

    while (file) {
        if (ordersCount >= MAX_ORDERS) break;

        Order order;

        // Read order ID
        if (!(file >> order.ID)) break;
        file.ignore();  // Ignore the newline after order ID

        // Read order date
        getline(file, order.date);  // Read the full date line
        if (order.date.empty()) continue;

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName);  // Read the menu item name
        order.menuItem.name = menuItemName;

        // Read price
        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        // Read number of products in menu item
        if (!(file >> order.menuItem.productCount)) break;
        file.ignore();  // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            // Read product details
            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore();  // Ignore the newline character after product details

                // Assign product details
                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        // Count occurrences of menu items
        bool found = false;
        for (int i = 0; i < uniqueMenuItemCount; ++i) {
            if (menuItems[i] == order.menuItem.name) {
                itemCount[i]++;  // Increment count if menu item is found
                found = true;
                break;
            }
        }

        // If menu item not found, add it to the list
        if (!found && uniqueMenuItemCount < MAX_MENU_ITEMS) {
            menuItems[uniqueMenuItemCount] = order.menuItem.name;
            itemCount[uniqueMenuItemCount] = 1;
            uniqueMenuItemCount++;
        }

        orders[ordersCount++] = order;  // Store the order and increment the orders count
    }

    file.close();

    // Print the counts of each menu item
    if (uniqueMenuItemCount == 0) {
        cout << "No menu items found in the orders.\n";
    }
    else {
        cout << "Menu items occurrences in the orders:\n";
        for (int i = 0; i < uniqueMenuItemCount; ++i) {
            cout << "Menu Item: " << menuItems[i] << " - " << itemCount[i] << " time(s)\n";
        }
    }
}


// Function to sum the menu item prices grouped by the same order date
void SumPricesByDate() {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    while (file) {
        if (ordersCount >= MAX_ORDERS) break;

        Order order;

        // Read order ID
        if (!(file >> order.ID)) break;
        file.ignore();  // Ignore the newline after order ID

        // Read order date
        getline(file, order.date);  // Read the full date line
        if (order.date.empty()) continue;

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName);  // Read the menu item name
        order.menuItem.name = menuItemName;

        // Read price
        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        // Read number of products in menu item
        if (!(file >> order.menuItem.productCount)) break;
        file.ignore();  // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            // Read product details
            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore();  // Ignore the newline character after product details

                // Assign product details
                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        // Find the index of the date in the dates array
        bool found = false;
        for (int i = 0; i < dateCount; ++i) {
            if (dates[i] == order.date) {
                // Add the price to the corresponding date
                priceSum[i] += order.menuItem.price;
                found = true;
                break;
            }
        }

        // If the date is not found, add it to the dates array
        if (!found && dateCount < MAX_DATES) {
            dates[dateCount] = order.date;
            priceSum[dateCount] = order.menuItem.price;
            dateCount++;
        }

        orders[ordersCount++] = order;  // Store the order and increment the orders count
    }

    file.close();
    string inputDate;
    cout << "Enter the date (in format DD-MM-YYYY): ";
    cin >> inputDate;
    bool found = false;
    // Print the total price sum for each date
    if (dateCount == 0) {
        cout << "No orders found.\n";
    }
    else {
        cout << "Total prices by date:\n";
        for (int i = 0; i < dateCount; ++i) {
            if (dates[i] == inputDate) {
                cout << "Date: " << dates[i] << " - Total Price: $" << priceSum[i] << "\n";
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "No orders found for the date " << inputDate << ".\n";
        }
    }
}
double CalculateSumForDate(const string& inputDate) {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return -1.0; // Return -1 to indicate an error
    }

    double totalSum = 0.0; // Initialize total sum for the input date

    while (file) {
        Order order;

        // Read order ID
        if (!(file >> order.ID)) break;
        file.ignore();  // Ignore the newline after order ID

        // Read order date
        getline(file, order.date); // Read the full date line
        if (order.date.empty()) continue;

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName); // Read the menu item name
        order.menuItem.name = menuItemName;

        // Read price
        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        // Read number of products in menu item
        if (!(file >> order.menuItem.productCount)) break;
        file.ignore(); // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            // Read product details
            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore(); // Ignore the newline character after product details

                // Assign product details
                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        // Check if the order's date matches the input date
        if (order.date == inputDate) {
            totalSum += order.menuItem.price;
        }
    }

    file.close();
    return totalSum; // Return the computed total sum
}
// Helper function to trim leading and trailing whitespaces from a string
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Function to sum the menu item prices grouped by the same order date
bool ParseDate(const string& date, int& day, int& month, int& year) {
    char delimiter;

    if (date.find('-') != string::npos) {
        delimiter = '-';
    }
    else if (date.find('.') != string::npos) {
        delimiter = '.';
    }
    else {
        return false; // Invalid format
    }

    // Format the parsing string dynamically based on the delimiter
    const string format = "%d%c%d%c%d";

    int scanned = sscanf_s(date.c_str(), format.c_str(), &day, &delimiter, sizeof(char), &month, &delimiter, sizeof(char), &year);
    if (scanned == 5) {
        return true;
    }

    return false;
}
string GetEarlierDate(const string& date1, const string& date2) {
    int day1, month1, year1;
    int day2, month2, year2;

    if (!ParseDate(date1, day1, month1, year1) || !ParseDate(date2, day2, month2, year2)) {
        return ""; // Return empty string for invalid date formats
    }

    // Compare years
    if (year1 > year2) return date2;
    if (year1 < year2) return date1;

    // Compare months
    if (month1 > month2) return date2;
    if (month1 == 11 && month2 != 12) return date2;
    if (month2 == 11 && month1 != 12) return date1;
    if (month1 < month2) return date1;
    if (month1 == 12) return date2;
    if (month2 == 12) return date1;
    // Compare days
    if (day1 > day2) return date2;
    if (day1 < day2) return date1;

    // Both dates are equal
    return date1;
}
string GetLatterDate(const string& date1, const string& date2) {
    int day1, month1, year1;
    int day2, month2, year2;

    if (!ParseDate(date1, day1, month1, year1) || !ParseDate(date2, day2, month2, year2)) {
        return ""; // Return empty string for invalid date formats
    }

    // Compare years
    if (year1 > year2) return date1;
    if (year1 < year2) return date2;

    // Compare months
    if (month1 > month2) return date1;
    if (month1 == 11 && month2 != 12) return date1;
    if (month2 == 11 && month1 != 12) return date2;
    if (month1 < month2) return date2;
    if (month1 == 12) return date1;
    if (month2 == 12) return date2;
    // Compare days
    if (day1 > day2) return date1;
    if (day1 < day2) return date2;

    // Both dates are equal
    return date1;
}
void SortOrdersByDate() {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    // Read all orders into the array
    while (file) {
        if (ordersCount >= MAX_ORDERS) break; // Stop if we exceed the max orders count

        Order order;

        // Read order ID
        if (!(file >> order.ID)) break;
        file.ignore(); // Ignore the newline after order ID

        // Read order date
        getline(file, order.date); // This reads the full date line
        if (order.date.empty()) continue; // If there is no date line, ignore

        // Read menu item name
        string menuItemName;
        getline(file, menuItemName); // Read the menu item name
        order.menuItem.name = menuItemName;

        // Read price
        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        // Read number of products in menu item
        if (!(file >> order.menuItem.productCount)) break;
        file.ignore(); // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            // Read product details
            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore(); // Ignore the newline character after product details

                // Assign product details
                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        orders[ordersCount++] = order; // Store the order in the array and increment orders count
    }

    file.close();

    if (ordersCount == 0) {
        cout << "No orders to sort.\n";
        return;
    }

    // Sorting the orders by date using a simple bubble sort
    for (int i = 0; i < ordersCount; ++i) {
        for (int j = i + 1; j < ordersCount; ++j) {
            // Use GetLatterDate to compare dates
            string laterDate = GetEarlierDate(orders[i].date, orders[j].date);
            if (laterDate == orders[j].date) {
                swap(orders[i], orders[j]); // Swap the orders if j's date is later
            }
        }
    }

    // Print sorted orders (optional)
 /*cout << "Orders sorted by date:\n";
    for (int i = 0; i < ordersCount; ++i) {
        cout << "Order ID: " << orders[i].ID << ", Date: " << orders[i].date
            << ", Menu Item: " << orders[i].menuItem.name << ", Price: "
            << orders[i].menuItem.price << "\n";
    }*/
}
void PrintPriceFromDate1() {
    ifstream file("orders.txt");
    if (!file.is_open()) {
        cout << "Error: Unable to open orders file.\n";
        return;
    }

    dateCount = 0;

    while (file) {
        Order order;

        if (!(file >> order.ID)) break;
        file.ignore();  // Ignore newline after order ID

        // Read order date and trim any surrounding whitespace
        getline(file, order.date);
        order.date = trim(order.date);

        if (order.date.empty()) continue;

        string menuItemName;
        getline(file, menuItemName);
        order.menuItem.name = menuItemName;

        if (!(file >> order.menuItem.price)) break;
        file.ignore(); // Ignore newline after price

        if (!(file >> order.menuItem.productCount)) break;
        file.ignore(); // Ignore newline after product count

        // Read product details
        for (int i = 0; i < order.menuItem.productCount; ++i) {
            string productName;
            int quantity, quantityInMenuItem;

            if (getline(file, productName) && file >> quantity >> quantityInMenuItem) {
                file.ignore();  // Ignore the newline character after product details

                order.menuItem.products[i].name = productName;
                order.menuItem.products[i].quantity = quantity;
                order.menuItem.products[i].QuantityInMenuItem = quantityInMenuItem;
            }
            else {
                cout << "Error: File format incorrect for product at index "
                    << i << " in order ID " << order.ID << ".\n";
                break;
            }
        }

        bool found = false;
        for (int i = 0; i < dateCount; ++i) {
            if (dates[i] == order.date) {
                priceSum[i] += order.menuItem.price;
                found = true;
                break;
            }
        }

        if (!found && dateCount < MAX_DATES) {
            dates[dateCount] = order.date;
            priceSum[dateCount] = order.menuItem.price;
            dateCount++;
        }

        orders[ordersCount++] = order;
    }

    file.close();
    SortOrdersByDate();

    if (ordersCount == 0) {
        cout << "No orders available to process.\n";
        return;
    }

    // Prompt the user to input a date
    string inputDate;
    cout << "Enter the date (in format DD-MM-YYYY): ";
    cin >> inputDate;

    inputDate = trim(inputDate); // Trim any extra spaces from the input date

    bool foundDate = false;

    // Process sorted orders
    cout << "Sums from " << inputDate << " onwards:\n";
    double cumulativeSum = 0.0;

    std::string printedDates[MAX_DATES]; // Array to track printed dates
    int printedDateCount = 0;            // Counter for printed dates

    for (int i = 0; i < ordersCount; ++i) {
        // Check if the order's date is the same or later than the input date
        if (GetLatterDate(orders[i].date, inputDate) == orders[i].date) {
            foundDate = true;

            // Check if this date has already been printed
            bool alreadyPrinted = false;
            for (int j = 0; j < printedDateCount; ++j) {
                if (printedDates[j] == orders[i].date) {
                    alreadyPrinted = true;
                    break;
                }
            }

            if (!alreadyPrinted) {
                // Add the date to the printedDates array
                if (printedDateCount < MAX_DATES) {
                    printedDates[printedDateCount++] = orders[i].date;
                }

                // Print the date and total price for it
                cout << "Date: " << orders[i].date
                    << " - Total Price: $" << CalculateSumForDate(orders[i].date) << "\n";
            }

            // Add the order's price to the cumulative sum
            cumulativeSum += orders[i].menuItem.price;
        }
    }

}
// Function to print the sum of prices starting from a particular date (inclusive)
void PrintPriceFromDate() {
    string inputDate;
    cout << "Enter the date (in format DD.MM.YYYY): ";
    cin >> inputDate;

    inputDate = trim(inputDate);  // Trim any extra spaces from the input date

    bool foundDate = false;

    // Loop through all the dates to check if the input date exists
    for (int i = 0; i < dateCount; ++i) {
        if (dates[i] == inputDate) {
            foundDate = true;

            // Print sums starting from the input date (inclusive)
            cout << "Sums from " << inputDate << " onwards:\n";
            for (int j = i; j < dateCount; ++j) {
                cout << "Date: " << dates[j] << " - Total Price: $" << priceSum[j] << "\n";
            }
            break;
        }
    }

    if (!foundDate) {
        cout << "No orders found for the date " << inputDate << ".\n";
    }
}
