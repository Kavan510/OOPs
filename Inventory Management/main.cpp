#include "include/Inventory.h"
#include "include/Product.h"
#include <iostream>
using namespace std;

void showMenu()
{
    cout << "\n1. Add Product\n"
         << "2. Remove Product\n"
         << "3. Update Quantity\n"
         << "4. List Products\n"
         << "5. Exit\n"
         << "Choose option: ";
}

int main()
{
    Inventory inv;
    int choice;
    do
    {
        showMenu();
        cin >> choice;
        cout << "Instruction: Include space between each attributes\n";
        switch (choice)
        {
        case 1:
        {
            int id, qty;
            string name;
            double price;
            cout << "Enter ID, Name, Price, Quantity: ";
            cin >> id >> name >> price >> qty;
            inv.addProduct(Product(id, name, price, qty));
            break;
        }
        case 2:
        {
            int id;
            cout << "Enter Product ID to remove: ";
            cin >> id;
            inv.removeProduct(id)
                ? cout << "Removed\n"
                : cout << "Not found\n";
            break;
        }
        case 3:
        {
            int id, change;
            cout << "Enter Product ID and Qty Change (+/-): ";
            cin >> id >> change;
            inv.updateQuantity(id, change)
                ? cout << "Updated\n"
                : cout << "Failed\n";
            break;
        }
        case 4:
            inv.listAllProducts();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option\n";
        }
    } while (choice != 5);
    return 0;
}
