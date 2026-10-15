#include "../include/Item.h"

int Item::contador_itens = 0;

Item::Item(const std::string& _nome) : id(contador_itens++), nome(_nome) {}
int Item::getId() const { return id; }
const std::string& Item::getNome() const { return nome; }