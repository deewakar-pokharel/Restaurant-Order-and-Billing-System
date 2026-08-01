/*
    RESTAURANT ORDER & BILLING SYSTEM
    ----------------------------------
    OOP concepts used : Classes & Objects, Constructors & Destructors,
                         Hierarchical Inheritance, Operator Overloading
    File Handling used: ifstream (reading menu), ofstream (saving bills)

    ENCT 151 - Object Oriented Programming
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;

const int MAX_ITEMS = 20;

// ================= Base Class =================
class MenuItem {
protected:
    int code;
    string name;
    double price;

public:
    // Default constructor
    MenuItem() {
        code = 0;
        name = "";
        price = 0.0;
    }

    // Parameterized constructor
    MenuItem(int c, string n, double p) {
        code = c;
        name = n;
        price = p;
    }

    // Destructor
    ~MenuItem() {
        // Nothing to clean up manually, but shown for completeness
    }

    int getCode() const { return code; }
    string getName() const { return name; }
    double getPrice() const { return price; }

    // Overloaded << operator to print a MenuItem directly
    friend ostream& operator<<(ostream& out, const MenuItem& item);
};

ostream& operator<<(ostream& out, const MenuItem& item) {
    out << item.code << "\t" << item.name << "\tRs. " << item.price;
    return out;
}

// ================= Derived Class: FoodItem =================
class FoodItem : public MenuItem {
private:
    string spiceLevel;

public:
    FoodItem() : MenuItem() {
        spiceLevel = "Mild";
    }

    FoodItem(int c, string n, double p, string spice) : MenuItem(c, n, p) {
        spiceLevel = spice;
    }

    void display() const {
        cout << code << "\t" << name << "\t\tRs. " << price
             << "\t(" << spiceLevel << ")" << endl;
    }
};

// ================= Derived Class: DrinkItem =================
class DrinkItem : public MenuItem {
private:
    string size;

public:
    DrinkItem() : MenuItem() {
        size = "Medium";
    }

    DrinkItem(int c, string n, double p, string sz) : MenuItem(c, n, p) {
        size = sz;
    }

    void display() const {
        cout << code << "\t" << name << "\t\tRs. " << price
             << "\t(" << size << ")" << endl;
    }
};

// ================= Bill Class =================
class Bill {
private:
    string itemNames[50];
    double itemPrices[50];
    int lineCount;
    double total;
    int tableNumber;

public:
    Bill(int table) {
        tableNumber = table;
        lineCount = 0;
        total = 0.0;
    }

    // Overloaded += operator: adds one MenuItem (food or drink) to the bill
    Bill& operator+=(const MenuItem& item) {
        if (lineCount < 50) {
            itemNames[lineCount] = item.getName();
            itemPrices[lineCount] = item.getPrice();
            total += item.getPrice();
            lineCount++;
        }
        return *this;
    }

    double getTotal() const { return total; }
    int getTable() const { return tableNumber; }

    // Overloaded << operator: prints the whole receipt
    friend ostream& operator<<(ostream& out, const Bill& bill);

    // Save this bill to the sales log file
    void saveToFile() const;
};

ostream& operator<<(ostream& out, const Bill& bill) {
    out << "\n----- BILL: Table " << bill.tableNumber << " -----\n";
    for (int i = 0; i < bill.lineCount; i++) {
        out << bill.itemNames[i] << "\tRs. " << bill.itemPrices[i] << endl;
    }
    out << "-----------------------------\n";
    out << "TOTAL: Rs. " << bill.total << "\n";
    return out;
}

void Bill::saveToFile() const {
    ofstream outFile("sales_log.txt", ios::app);   // append mode
    if (!outFile) {
        cout << "Error: Could not open sales_log.txt for saving.\n";
        return;
    }
    outFile << "Table: " << tableNumber << endl;
    for (int i = 0; i < lineCount; i++) {
        outFile << "  " << itemNames[i] << " - Rs. " << itemPrices[i] << endl;
    }
    outFile << "TOTAL: " << total << endl;
    outFile << "----------------------\n";
    outFile.close();
}

// ================= Menu storage (global arrays of objects) =================
FoodItem foodMenu[MAX_ITEMS];
int foodCount = 0;
DrinkItem drinkMenu[MAX_ITEMS];
int drinkCount = 0;

// Read menu.txt and fill the food/drink arrays
void loadMenu() {
    ifstream inFile("menu.txt");
    if (!inFile) {
        cout << "Error: menu.txt not found!\n";
        return;
    }

    string line;
    string mode = "";

    while (getline(inFile, line)) {
        if (line == "FOOD" || line == "DRINK") {
            mode = line;
            continue;
        }
        if (line.empty()) continue;

        stringstream ss(line);
        string codeStr, name, priceStr, extra;
        getline(ss, codeStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, extra, ',');

        int code = atoi(codeStr.c_str());
        double price = atof(priceStr.c_str());

        if (mode == "FOOD" && foodCount < MAX_ITEMS) {
            foodMenu[foodCount++] = FoodItem(code, name, price, extra);
        } else if (mode == "DRINK" && drinkCount < MAX_ITEMS) {
            drinkMenu[drinkCount++] = DrinkItem(code, name, price, extra);
        }
    }
    inFile.close();
}

void displayMenu() {
    cout << "\n===== FOOD MENU =====\n";
    cout << "Code\tName\t\tPrice\t(Spice)\n";
    for (int i = 0; i < foodCount; i++) foodMenu[i].display();

    cout << "\n===== DRINKS MENU =====\n";
    cout << "Code\tName\t\tPrice\t(Size)\n";
    for (int i = 0; i < drinkCount; i++) drinkMenu[i].display();
}

// Search both menus for an item by its code
MenuItem* findItemByCode(int code) {
    for (int i = 0; i < foodCount; i++) {
        if (foodMenu[i].getCode() == code) return &foodMenu[i];
    }
    for (int i = 0; i < drinkCount; i++) {
        if (drinkMenu[i].getCode() == code) return &drinkMenu[i];
    }
    return NULL;
}

// Read sales_log.txt and add up every TOTAL: line
void viewTodaySales() {
    ifstream inFile("sales_log.txt");
    if (!inFile) {
        cout << "No sales recorded yet.\n";
        return;
    }

    string line;
    double grandTotal = 0.0;
    while (getline(inFile, line)) {
        if (line.substr(0, 6) == "TOTAL:") {
            double amt = atof(line.substr(7).c_str());
            grandTotal += amt;
        }
    }
    inFile.close();

    cout << "\nToday's Total Sales: Rs. " << grandTotal << endl;
}

// ================= Main Program =================
int main() {
    loadMenu();

    int choice;
    do {
        cout << "\n===== RESTAURANT BILLING SYSTEM =====\n";
        cout << "1. New Order\n";
        cout << "2. View Today's Total Sales\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int table;
            cout << "Enter table number: ";
            cin >> table;

            displayMenu();

            Bill bill(table);
            int code, qty;

            while (true) {
                cout << "\nEnter item code to add (0 to finish): ";
                cin >> code;
                if (code == 0) break;

                MenuItem* item = findItemByCode(code);
                if (item == NULL) {
                    cout << "Invalid code. Try again.\n";
                    continue;
                }

                cout << "Enter quantity: ";
                cin >> qty;

                for (int i = 0; i < qty; i++) {
                    bill += *item;   // uses our overloaded += operator
                }
            }

            cout << bill;            // uses our overloaded << operator
            bill.saveToFile();
            cout << "Bill saved successfully!\n";
        }
        else if (choice == 2) {
            viewTodaySales();
        }

    } while (choice != 3);

    cout << "Thank you! Program ended.\n";
    return 0;
}
