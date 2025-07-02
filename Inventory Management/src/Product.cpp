#include "Product.h"

Product::Product(int id, const std::string& name, double price, int quantity){
    this->id =id;
    this->name=name;
    this->price=price;
    this->quantity= quantity;
}
    // : id(id), name(name), price(price), quantity(quantity) {}

int Product::getId() const {
    return id;
}

const std::string& Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

int Product::getQuantity() const {
    return quantity;
}

void Product::setPrice(double price) {
    this->price = price;
}

void Product::setQuantity(int quantity) {
    this->quantity = quantity;
}
