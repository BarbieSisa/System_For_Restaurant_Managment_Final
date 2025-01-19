#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "menuItems.h"
#include "products.h"
#include "orders.h"
using namespace std;
    void WaiterUserRole()
    {
        int choice;
        do {
            std::cout << "Menu:\n";
            std::cout << "Opt for 1: If you want to see the MenuItems." << endl;
            std::cout << "Opt for 2: If you want to make an Order." << endl;
            std::cout << "Opt for 3: If you want to delete an Order." << endl;
            std::cout << "Opt for 4: If you want to see the previous Orders." << endl;
            std::cout << "Opt for 5: If you want to see the previous Orders, sorted by name." << endl;
            std::cout << "Opt for 6: If you want to see the number of MenuItems ordered." << endl;
            std::cout << "Opt for 7: If you want to see the turnover for the particular day." << endl;
            std::cout << "Opt for 0: Quit the program." << endl;

            std::cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                PrintMenuItems();
                break;
            case 2:
                CreateOrder();
                break;
            case 3:
                DeleteOrder();
                break;
            case 4:
                PrintOrders();
                break;
            case 5:
                SortOrdersByMenuItemNameDescending();
                break;
            case 6:
                PrintMenuItemOccurrences();
                break;
            case 7:
                SumPricesByDate();
                break;
            case 0:
                std::cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please choose a valid number between 0 and 7.\n";
                break;
            }

            std::cout << endl; 
        } while (choice != 0);


    }
    void ManagerUserRole()
    {
        int choice;
        do {
            std::cout << "Menu:\n";
            std::cout << "Opt for 1: If you want to see the MenuItems." << endl;
            std::cout << "Opt for 2: If you want to make an Order." << endl;
            std::cout << "Opt for 3: If you want to delete an Order." << endl;
            std::cout << "Opt for 4: If you want to see the previous Orders." << endl;
            std::cout << "Opt for 5: If you want to see the previous Orders, sorted by name." << endl;
            std::cout << "Opt for 6: If you want to see the Products in the Warehouse." << endl;
            std::cout << "Opt for 7: If you want to delete a paticular Product in the Warehouse ." << endl;
            std::cout << "Opt for 8: If you want to add a new Product into the Warehouse ." << endl;
            std::cout << "Opt for 9: If you want to see the turnover for the particular day." << endl;
            std::cout << "Opt for 10: If you want to see the turnover for a particular Date up to Now ." << endl;
            std::cout << "Opt for 11: If you want to add new MenuItem into the Restaurant Menu ." << endl;
            std::cout << "Opt for 12: If you want to delete a paticular MenuItem in the Restaurant Menu ." << endl;
            std::cout << "Opt for 0: Quit the program." << endl;

            std::cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                PrintMenuItems();
                break;
            case 2:
                CreateOrder();
                break;
            case 3:
                DeleteOrder();
                break;
            case 4:
                PrintOrders();
                break;
            case 5:
                SortOrdersByMenuItemNameDescending();
                break;
            case 6:
                PrintProducts();
            case 7:
                DeleteProduct();
            case 8:
                CreateProduct();
                break;
            case 9:
                SumPricesByDate();
            case 10:
                SortOrdersByDate();
            case 11:
                CreateMenuItem();
                break;
            case 12:
                DeleteMenuItem();
            case 0:
                std::cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please choose a valid number between 0 and 7.\n";
                break;
            }

            std::cout << endl; 
        } while (choice != 0);


    }
int main() {
    std::cout << "There are two types of user roles in 'System for Restaurant Management App'" << endl;
    std::cout << "If the user want to Log in as a waiter, type : Waiter" << endl;
    std::cout << "Else, if the user want to Log in as a manager : Manager" << endl;
    string UserRole;
    cin >> UserRole;
    if (UserRole == "Waiter")
        WaiterUserRole();
   else if (UserRole == "Manager")
        ManagerUserRole(); 
    return 0;
}
