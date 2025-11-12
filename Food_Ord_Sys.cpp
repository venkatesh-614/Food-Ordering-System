#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <utility>

using namespace std;

const int MAX_MENU_ITEMS = 15;  // Maximum number of items on the menu

// Class representing a single menu item
class MenuItem {
public:
    string name;
    double price;
    int preparationTime;
    int orderCount;

    MenuItem(string name = "", double price = 0.0, int prepTime = 0)
        : name(name), price(price), preparationTime(prepTime), orderCount(0) {}

    void display() const {
        cout << left << setw(15) << name << " $" << fixed << setprecision(2) << price
             << " Prep Time: " << preparationTime << " min" << endl;
    }
};

// Class representing the entire menu
class Menu {
private:
    MenuItem items[MAX_MENU_ITEMS];  // Array to store menu items
    int itemCount;                   // Current number of items on the menu

public:
    Menu() : itemCount(0) {}

    void addMenuItem(const MenuItem& item) {
        if (itemCount < MAX_MENU_ITEMS) {
            items[itemCount++] = item;
        } else {
            cout << "Menu is full. Cannot add more items.\n";
        }
    }

    void viewMenu() const {
        cout << "Menu:\n";
        for (int i = 0; i < itemCount; ++i) {
            cout << i + 1 << ". ";
            items[i].display();
        }
    }

    void updateMenuItem(int index, const string& newName, double newPrice, int newPrepTime) {
        if (index >= 0 && index < itemCount) {                 // Changed index < 15  itemCount
            items[index].name = newName;
            items[index].price = newPrice;
            items[index].preparationTime = newPrepTime;
            cout << "Menu item updated successfully.\n";
        } else {
            cout << "Invalid menu item index.\n";
        }
    }

    MenuItem& getMenuItem(int index) {
        return items[index];
    }

    int getItemCount() const {
        return itemCount;
    }
};

// Base class for Users
class User {
protected:
    string name;

public:
    User(string name) : name(name) {}
    virtual void viewMenu(const Menu& menu) const = 0;
};

// Manager class derived from User
class Manager : public User {
private:
    static double totalSales;  // Static variable to track total sales
    static int totalOrders;    // Static variable to track total orders

public:
    Manager(string name) : User(name) {}

    void viewMenu(const Menu& menu) const{
        menu.viewMenu();                //line 45
    }

    void updateMenu(Menu& menu, int index, const string& newName, double newPrice, int newPrepTime) {
        menu.updateMenuItem(index, newName, newPrice, newPrepTime);         //line 53
    }

    void updatePrice(Menu& menu, int index, double newPrice) {
        if (index >= 0 && index < menu.getItemCount()) {
            menu.getMenuItem(index).price = newPrice;               //line 64
            cout << "Price updated successfully.\n";
        } else {
            cout << "Invalid menu item index.\n";
        }
    }

    void acceptOrder(Menu& menu, int itemIndex, int quantity) {
        if (itemIndex >= 0 && itemIndex < menu.getItemCount()) {
            double orderAmount = menu.getMenuItem(itemIndex).price * quantity;
            menu.getMenuItem(itemIndex).orderCount += quantity;
            totalSales += orderAmount;
            totalOrders += quantity;
            cout << "Order accepted for " << quantity << " of " << menu.getMenuItem(itemIndex).name << endl;
            cout << "Total Order Amount: $" << orderAmount << endl<<endl;
        } else {
            cout << "Invalid menu item index.\n";
        }
    }

    void viewTotalSales() const {
        cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
        cout << "Total number of items ordered: " << totalOrders << endl;
    }

    void popularDish( Menu& menu) const {
        int maxOrders = 0;
        string popularItem;

        for (int i = 0; i < menu.getItemCount(); ++i) {
            if (menu.getMenuItem(i).orderCount > maxOrders) {           //line 64
                maxOrders = menu.getMenuItem(i).orderCount;
                popularItem = menu.getMenuItem(i).name;
            }
        }

        if (maxOrders > 0) {
            cout << "Popular Dish: " << popularItem << " with " << maxOrders << " orders.\n";
        } else {
            cout << "No orders have been placed yet.\n";
        }
    }
};

// Initializing static variables
double Manager::totalSales = 0;
int Manager::totalOrders = 0;

// Customer class derived from User
class Customer : public User {
private:
    double balance;
    vector<pair<string, int> > orderedItems; // To track ordered items and their quantities

public:
    Customer(string name) : User(name) {}

    void viewMenu(const Menu& menu) const {
        menu.viewMenu();
    }

    bool placeOrder(Menu& menu, Manager& manager) {
        int itemIndex, quantity;
        double totalCost = 0.0;
        vector<pair<int, int> > orderDetails;

        cout << "Enter the number of different items you want to order: ";
        int itemCount;
        cin >> itemCount;

        for (int i = 0; i < itemCount; ++i) {
            cout << "Enter item index and quantity for item " << (i + 1) << ": ";
            cin >> itemIndex >> quantity;

            if (itemIndex < 1 || itemIndex > menu.getItemCount() || quantity <= 0) {
                cout << "Invalid input for item " << (i + 1) << ". Please try again.\n";
                --i;
                continue;
            }

            --itemIndex; // Adjust for 0-based index
            MenuItem& item = menu.getMenuItem(itemIndex);
            double cost = item.price * quantity;
            totalCost += cost;
            orderDetails.push_back(make_pair(itemIndex, quantity));
        }

        // if (totalCost > balance) {
        //     cout << "Insufficient balance to place the entire order. Order not placed.\n";
        //     return false;
        // }

       // deductBalance(totalCost);
        for (size_t i = 0; i < orderDetails.size(); ++i) {
            int idx = orderDetails[i].first;
            int qty = orderDetails[i].second;
            manager.acceptOrder(menu, idx, qty);
            orderedItems.push_back(make_pair(menu.getMenuItem(idx).name, qty));
        }

        cout << "Total Cost for your order: $" << totalCost << endl;
        cout<<"\n\nPlease pay the amount to the delivery executive\n";
        return true;
    }

    // void makePayment(double amount) {
    //     //cout<<amount;
    //     balance += amount;
    //     if (balance < 0) {
    //         cout << "Your wallet has been emptied. Recharge it now to enjoy the delicious food. Hurry up!!!" << endl;
    //     } else {
    //         cout << "Payment of $" << fixed << setprecision(2) << amount << " successful. Updated balance: $" << balance << endl;
    //     }
    // }

    // void deductBalance(double totalCost) {
    //     balance -= totalCost;
    // }

    // double getBalance() const {
    //     return balance;
    // }

    void displayOrderedItems() const {
        if (orderedItems.empty()) {
            cout << "No items ordered yet.\n";
            return;
        }
        cout << "Ordered Items:\n";
        for (size_t i = 0; i < orderedItems.size(); ++i) {
            cout << orderedItems[i].first << " x" << orderedItems[i].second << endl;
        }
    }
};

// Main function
int main() {
    Menu menu;
    menu.addMenuItem(MenuItem("Burger", 5.99, 10));                 //line 37
    menu.addMenuItem(MenuItem("Pizza", 8.99, 15));
    menu.addMenuItem(MenuItem("Pasta", 7.99, 12));
    menu.addMenuItem(MenuItem("French Fries", 3.99, 10));
    menu.addMenuItem(MenuItem("Mojito", 9.99, 10));
    menu.addMenuItem(MenuItem("Milkshake", 11.99, 15));
    menu.addMenuItem(MenuItem("Coffee", 3.99, 5));
    menu.addMenuItem(MenuItem("Juice", 8.99, 7));
    menu.addMenuItem(MenuItem("Pastries", 7.99, 5));

    Manager manager("Manager1");                            //line 90
    Customer customer("Customer1");               //line 157

    int choice;
    cout << "\nWelcome to the Restaurant Management System!\n";
    cout << "\t   __      __        __\n";
    cout << "\t  /  \\    /  \\ ____ |  | ____  ____    _____   ____ \n";           // Greet the user
    cout << "\t  \\   \\/\\/   // __ \\|  |/ ___\\/  _ \\  /     \\ / __ \\\n";
    cout << "\t   \\        /|  ___/|  |  \\__(  <_> )|  Y Y  | ___ /  \n";
    cout << "\t    \\__/\\__/  \\____\\|__|\\_____>____/ |__|_|__|\\____\\\n";
    cout << " ----------------------------------------------------------------- " << endl;
    while (true) {
        cout << "\nSelect User:\n1. Manager\n2. Customer\n3. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            int managerChoice;
            do {
                cout << "\nManager View:\n1. View Menu\n2. Update Menu Item\n3. Update Price\n4. View Total Sales\n5. View Popular Dish\n6. Exit\nChoice: ";
                cin >> managerChoice;

                if (managerChoice == 1) {
                    manager.viewMenu(menu);             //line 92   
                } else if (managerChoice == 2) {
                    int index;
                    string newName;
                    double newPrice;
                    int newPrepTime;
                    cout << "Enter item index, new name, price, and preparation time: ";
                    cin >> index >> newName >> newPrice >> newPrepTime;
                    manager.updateMenu(menu, index - 1, newName, newPrice, newPrepTime);   //line 96          
                } else if (managerChoice == 3) {
                    int index;
                    double newPrice;
                    cout << "Enter item index and new price: ";
                    cin >> index >> newPrice;
                    manager.updatePrice(menu, index - 1, newPrice);             //line 100
                } else if (managerChoice == 4) {
                    manager.viewTotalSales();               //line 122
                } else if (managerChoice == 5) {
                    manager.popularDish(menu); // Call popular dish function            //line 127
                }
            } while (managerChoice != 6);
        } else if (choice == 2) {
            int customerChoice;
            do {
                cout << "\nCustomer View:\n1. View Menu\n2. Place Order\n3. Display Ordered Items\n4. Exit\nChoice: ";
                cin >> customerChoice;

                if (customerChoice == 1) {
                    cout << "--------------------------------------------------" << endl; // Display the food menu
                    cout << "          _____\n";
                    cout << "         /     \\   ____   ____    __ __\n";
                    cout << "        /  \\ /  \\_/ __ \\ /     \\ |  |  |\n";
                    cout << "       /    Y    \\  ___/|   |   \\|  |  |\n";
                    cout << "       \\____|____/\\____>____|____|_____|\n\n";
                    cout << "--------------------------------------------------" << endl;
                    customer.viewMenu(menu);
                } else if (customerChoice == 2) {
                    if (customer.placeOrder(menu, manager)) {  // Pass manager reference here           //line 163
                        // Process payment after placing the order
                        double payment;
                    }       // Added this after removing 3 lines from choice=2 and choice=3
                //         cout << "Enter total amount for payment: ";
                //         cin >> payment;
                //         customer.deductBalance(payment); // Deduct the total cost from balance
                //     }
                // } else if (customerChoice == 3) {
                //     double payment;
                //     cout << "Enter payment amount: ";
                //     cin >> payment;
                //    // cout << "Payment of $" << fixed << setprecision(2) << payment << " successful.";
                //     customer.makePayment(payment);
                } else if (customerChoice == 3) {
                    customer.displayOrderedItems(); // Display ordered items
                }
            } while (customerChoice != 4);
        // } else if (choice == 3) {
        //     cout << "Exiting the program.\n";
        //     break;
        // } else {
        //     cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}