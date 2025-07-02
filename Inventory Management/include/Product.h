#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    int id;
    std::string name;
    double price;
    int quantity;

public:
    Product(int id, const std::string& name, double price, int quantity);
    int getId() const;
    const std::string& getName() const;
    double getPrice() const;
    int getQuantity() const;
    void setPrice(double price);
    void setQuantity(int quantity);
};

#endif // PRODUCT_H
