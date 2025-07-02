#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <vector>

class Inventory {
private:
    std::vector<Product> products;

public:
    void addProduct(const Product& p);
    bool removeProduct(int productId);
    Product* findProduct(int productId);
    void listAllProducts() const;
    bool updateQuantity(int productId, int change);
};

#endif // INVENTORY_H
