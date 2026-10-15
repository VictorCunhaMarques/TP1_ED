#include "../include/Item.h"

Item::Item(int _id, const std::string& _nome) : id(_id), nome(_nome) {}
int Item::getId() const { return id; }
const std::string& Item::getNome() const { return nome; }