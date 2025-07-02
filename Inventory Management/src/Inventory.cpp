#include "Inventory.h"
#include <iostream>

void Inventory::addProduct(const Product& p) {
    products.push_back(p);
}

bool Inventory::removeProduct(int productId) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getId() == productId) {
            products.erase(it);
            return true;
        }
    }
    return false;
}

Product* Inventory::findProduct(int productId) {
    for (auto& p : products) {
        if (p.getId() == productId)
            return &p;
    }
    return nullptr;
}

void Inventory::listAllProducts() const {
    std::cout << "ID\tName\tPrice\tQty\n";
    for (const auto& p : products) {
        std::cout 
            << p.getId() << "\t" 
            << p.getName() << "\t" 
            << p.getPrice() << "\t" 
            << p.getQuantity() << "\n";
    }
}

bool Inventory::updateQuantity(int productId, int change) {
    Product* p = findProduct(productId);
    if (!p) return false;
    int newQty = p->getQuantity() + change;
    if (newQty < 0) return false;
    p->setQuantity(newQty);
    return true;
}
