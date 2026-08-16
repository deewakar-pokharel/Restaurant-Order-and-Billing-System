#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Item
{
private:
    int id;
    string name;
    float price;

public:
    Item()
    {
        id = 0;
        name = "";
        price = 0;
    }

    Item(int i, string n, float p)
    {
        id = i;
        name = n;
        price = p;
    }

    int getId()
    {
        return id;
    }

    string getName()
    {
        return name;
    }

    float getPrice()
    {
        return price;
    }
};

class Order
{
private:
    int tableNo;
    int quantity[15];
    float total;

public:
    Order(int table)
    {
        tableNo = table;
        total = 0;

        for (int i = 0; i < 15; i++)
            quantity[i] = 0;
    }

    void addItem(Item item, int qty)
    {
        quantity[item.getId() - 1] += qty;
        total += item.getPrice() * qty;
    }

    int getTableNo()
    {
        return tableNo;
    }

    float getTotal()
    {
        return total;
    }

    void displayItems(Item menu[])
    {
        for (int i = 0; i < 15; i++)
        {
            if (quantity[i] > 0)
            {
                float amount = menu[i].getPrice() * quantity[i];

                cout << menu[i].getName()
                     << " x " << quantity[i]
                     << " = Rs. " << amount << endl;
            }
        }
    }

    void saveItems(ofstream &file, Item menu[])
    {
        for (int i = 0; i < 15; i++)
        {
            if (quantity[i] > 0)
            {
                float amount = menu[i].getPrice() * quantity[i];

                file << menu[i].getName()
                     << " x " << quantity[i]
                     << " = Rs. " << amount << endl;
            }
        }
    }
};

class Restaurant
{
private:
    Item menu[15];

public:
    Restaurant()
    {
        menu[0] = Item(1, "Momo", 120);
        menu[1] = Item(2, "Chowmein", 150);
        menu[2] = Item(3, "Thukpa", 180);
        menu[3] = Item(4, "Dal Bhat", 250);
        menu[4] = Item(5, "Sekuwa", 300);
        menu[5] = Item(6, "Sel Roti", 50);
        menu[6] = Item(7, "Samosa", 40);
        menu[7] = Item(8, "Aloo Tama", 180);
        menu[8] = Item(9, "Chatamari", 150);
        menu[9] = Item(10, "Yomari", 100);
        menu[10] = Item(11, "Thakali Khana", 350);
        menu[11] = Item(12, "Milk Tea", 40);
        menu[12] = Item(13, "Lassi", 100);
        menu[13] = Item(14, "Aloo Chop", 60);
        menu[14] = Item(15, "Fried Rice", 160);
    }

    void run()
    {
        int choice;

        do
        {
            cout << "\n========================================\n";
            cout << "        NEPALI RESTAURANT SYSTEM\n";
            cout << "========================================\n";
            cout << "1. Take Order\n";
            cout << "2. View Sales\n";
            cout << "3. Exit\n";
            cout << "========================================\n";

            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                takeOrder();
                break;

            case 2:
                viewSales();
                break;

            case 3:
                cout << "\nThank you for visiting!\n";
                cout << "Program exited successfully.\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
            }

        } while (choice != 3);
    }

private:
    void displayFoodMenu()
    {
        cout << "\n============= FOOD MENU =============\n";

        for (int i = 0; i < 15; i++)
        {
            cout << menu[i].getId()
                 << ". "
                 << menu[i].getName()
                 << " - Rs. "
                 << menu[i].getPrice()
                 << endl;
        }
    }

    void takeOrder()
    {
        int tableNo;
        int itemChoice;
        int quantity;
        int moreItems;

        cout << "\nEnter Table Number: ";
        cin >> tableNo;

        Order order(tableNo);

        displayFoodMenu();

        do
        {
            cout << "\nEnter item number: ";
            cin >> itemChoice;

            if (itemChoice < 1 || itemChoice > 15)
            {
                cout << "Invalid item number!\n";
                continue;
            }

            cout << "Enter quantity: ";
            cin >> quantity;

            if (quantity <= 0)
            {
                cout << "Invalid quantity!\n";
                continue;
            }

            order.addItem(menu[itemChoice - 1], quantity);

            cout << menu[itemChoice - 1].getName()
                 << " x " << quantity
                 << " added to order.\n";

            cout << "\nDo you want to order more items?\n";
            cout << "1. Yes\n";
            cout << "0. No\n";
            cout << "Enter choice: ";
            cin >> moreItems;

        } while (moreItems == 1);

        cout << "\n========================================\n";
        cout << "                 BILL\n";
        cout << "========================================\n";

        cout << "Table No: " << order.getTableNo() << endl;

        cout << "\nItems Sold:\n";
        cout << "----------------------------------------\n";

        order.displayItems(menu);

        cout << "----------------------------------------\n";
        cout << "TOTAL = Rs. " << order.getTotal() << endl;
        cout << "========================================\n";

        ofstream file("sales.txt", ios::app);

        if (!file)
        {
            cout << "\nError opening sales.txt!\n";
            return;
        }

        file << "\n========================================\n";
        file << "Table No: " << order.getTableNo() << endl;
        file << "\nItems Sold:\n";
        file << "----------------------------------------\n";

        order.saveItems(file, menu);

        file << "----------------------------------------\n";
        file << "TOTAL = Rs. " << order.getTotal() << endl;
        file << "========================================\n";

        cout << "\nSales saved successfully!\n";
    }

    void viewSales()
    {
        ifstream file("sales.txt");

        if (!file)
        {
            cout << "\nNo sales record found.\n";
            return;
        }

        string line;

        cout << "\n========================================\n";
        cout << "             SALES RECORD\n";
        cout << "========================================\n";

        while (getline(file, line))
            cout << line << endl;

        cout << "========================================\n";
    }
};

int main()
{
    Restaurant restaurant;
    restaurant.run();

    return 0;
}
