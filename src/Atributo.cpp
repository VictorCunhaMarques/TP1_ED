#include "../include/Atributo.h"

//Inicialização variável estática
int Atributo::contador_atributos = 0;

Atributo::Atributo(const std::string& _nome) : id(contador_atributos++), nome(_nome) {}
int Atributo::getId() const { return id; }
const std::string& Atributo::getNome() const { return nome; }
int Atributo::getTotalAtributos() { return contador_atributos; }