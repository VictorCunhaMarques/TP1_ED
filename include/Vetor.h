#ifndef VETOR_H
#define VETOR_H

// Estrutura de vetor dinâmico genérico (substitui std::vector sem violar as regras do trabalho)
template <typename T>
class Vetor {
private:
    T* dados;
    int tam;
    int cap;

    void redimensionar(int nova_capacidade) {
        T* novos_dados = new T[nova_capacidade];
        for (int i = 0; i < tam; i++) {
            novos_dados[i] = dados[i];
        }
        delete[] dados;
        dados = novos_dados;
        cap = nova_capacidade;
    }

public:
    Vetor(int cap_inicial = 8) : tam(0), cap(cap_inicial) {
        dados = new T[cap];
    }

    ~Vetor() {
        delete[] dados;
    }

    // Evitar cópias acidentais rasas
    Vetor(const Vetor&) = delete;
    Vetor& operator=(const Vetor&) = delete;

    void adicionar(const T& elemento) {
        if (tam == cap) {
            redimensionar(cap * 2);
        }
        dados[tam++] = elemento;
    }

    T& operator[](int indice) {
        return dados[indice];
    }

    const T& operator[](int indice) const {
        return dados[indice];
    }

    int tamanho() const {
        return tam;
    }

    bool vazio() const {
        return tam == 0;
    }

    void limpar() {
        tam = 0;
    }
};

#endif
