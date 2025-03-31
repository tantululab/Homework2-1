#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class Time {
private:
    int seconds;

public:
    Time(int sec = 0) : seconds(sec) {}

    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        int h = t.seconds / 3600;
        int m = (t.seconds % 3600) / 60;
        int s = t.seconds % 60;
        os << std::setfill('0') << std::setw(2) << h << "h:"
           << std::setw(2) << m << "m:" << std::setw(2) << s << "s";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Time& t) {
        int h, m, s;
        char ch;
        is >> h >> ch >> m >> ch >> s;
        t.seconds = h * 3600 + m * 60 + s;
        return is;
    }

    Time operator+(const Time& other) const { return Time(seconds + other.seconds); }
    Time operator-(const Time& other) const { return Time(seconds - other.seconds); }
};

class Item {
public:
    std::string name;
    double unit_price;
    char vat_type;
    int quantity;

    Item(std::string name, double price, char vat, int qty)
        : name(name), unit_price(price), vat_type(vat), quantity(qty) {}
};

class Invoice {
private:
    std::string sellerNIP, buyerNIP;
    std::vector<Item> items;

public:
    Invoice(const std::string& seller, const std::string& buyer) : sellerNIP(seller), buyerNIP(buyer) {}

    void add_item(const Item& item) { items.push_back(item); }

    friend std::ostream& operator<<(std::ostream& os, const Invoice& inv) {
        os << "----------------VAT Invoice----------------\n";
        os << "Seller: " << inv.sellerNIP << "\tBuyer: " << inv.buyerNIP << "\n";
        os << "--------------------------------------------\n";
        os << "C.J. VAT   U.P.  QTY   Net    Total\n";
        double total_net = 0, total_gross = 0;
        for (const auto& item : inv.items) {
            double vat_rate = (item.vat_type == 'A') ? 0.23 : (item.vat_type == 'B') ? 0.08 : 0.0;
            double net_price = item.unit_price * item.quantity;
            double gross_price = net_price * (1 + vat_rate);
            os << item.name << "\t" << item.vat_type << "\t"
               << std::fixed << std::setprecision(2) << item.unit_price
               << "\t" << item.quantity << "\t"
               << net_price << "\t" << gross_price << "\n";
            total_net += net_price;
            total_gross += gross_price;
        }
        os << "--------------------------------------------\n";
        os << "TOTAL: " << total_net << " | " << total_gross << "\n";
        return os;
    }
};
