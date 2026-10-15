#include "../include/Atributo.h"

Atributo::Atributo(int _id, const std::string& _nome) : id(_id), nome(_nome) {}
int Atributo::getId() const { return id; }
const std::string& Atributo::getNome() const { return nome; }