#ifndef CATALOGO_H
#define CATALOGO_H

#include <string>
#include "Excecoes.h"

// Estrutura para célula da Lista Cruzada (Modo L)
struct CelulaCruzada {
    int item;
    int atributo;
    int valor;
    CelulaCruzada* prox_coluna; // Próximo na mesma linha (coluna crescente)
    CelulaCruzada* prox_linha;  // Próximo na mesma coluna (linha crescente)

    CelulaCruzada(int _item, int _atributo, int _valor)
        : item(_item), atributo(_atributo), valor(_valor),
          prox_coluna(nullptr), prox_linha(nullptr) {}
};

// Estrutura para elemento do Vetor de Tripletas (Modo T)
struct Tripleta {
    int item;
    int atributo;
    int valor;
};

// Estrutura para busca por faixa de valores (Consulta LF - Ponto Extra)
struct EntradaAtributoValor {
    int item;
    int valor;
};

// TAD Catálogo: representa a matriz esparsa de modificadores de atributos por item.
// Suporta três representações internas:
//   - Modo L: Lista Cruzada
//   - Modo T: Vetor de Tripletas Ordenado
//   - Modo C: Compressed Sparse Row (CSR) - Ponto Extra
class Catalogo {
private:
    char modo; // 'L', 'T' ou 'C'
    int total_celulas;

    // --- Representação Modo L (Lista Cruzada) ---
    CelulaCruzada** cabecas_linhas;
    CelulaCruzada** cabecas_colunas;
    int cap_linhas;
    int cap_colunas;

    void redimensionarLinhasL(int novo_tamanho);
    void redimensionarColunasL(int novo_tamanho);
    void liberarMemoriaL();

    // --- Representação Modo T (Vetor de Tripletas Ordenado) ---
    Tripleta* tripletas;
    int cap_tripletas;

    void redimensionarT(int nova_capacidade);
    int buscarIndiceT(int item, int atributo, bool& encontrado) const;
    void liberarMemoriaT();

    // --- Representação Modo C (CSR - Compressed Sparse Row, Ponto Extra) ---
    int* csr_valores;
    int* csr_colunas;
    int* csr_linha_ptr;
    int csr_num_linhas;
    int csr_cap_celulas;

    void redimensionarCSRLinhas(int novo_num_linhas);
    void redimensionarCSRCelulas(int nova_cap);
    void liberarMemoriaCSR();

    // --- Estrutura Auxiliar para Consulta por Faixa LF (Ponto Extra) ---
    // Para cada atributo, vetor ordenado por valor
    EntradaAtributoValor** valores_atributos;
    int* qtd_valores_atributos;
    int* cap_valores_atributos;
    int cap_estrutura_atributos;

    void redimensionarEstruturaAtributos(int novo_tam);
    void atualizarIndiceValor(int item, int atributo, int novo_valor, int valor_antigo);
    void liberarEstruturaAtributos();

public:
    Catalogo(char modo_inicial = 'L');
    ~Catalogo();

    // Migração de representação interna ('L', 'T' ou 'C')
    void setModo(char novo_modo);
    char getModo() const;

    // Modificadores
    void alterarModificador(int id_item, int id_atributo, int valor);
    int consultarModificador(int id_item, int id_atributo) const;
    void removerModificador(int id_item, int id_atributo);

    // Consultas de percurso (alocam arrays que devem ser liberados pelo chamador com delete[])
    int obterModificadoresItem(int id_item, int*& atributos, int*& valores) const;
    int obterItensAtributo(int id_atributo, int*& itens, int*& valores) const;

    // Métricas para a consulta de recomendação Q
    double calcularPoder(int id_item) const;
    double calcularEquilibrio(int id_item, int total_atributos) const;
    double calcularSinergia(int id_item, const int* build_aventureiro, int total_atributos) const;

    // Produto interno entre uma linha do catálogo e um vetor denso
    double produtoInterno(int id_item, const int* vetor, int tamanho_vetor) const;

    int getTotalCelulas() const;

    // Ponto Extra: Consulta LF por faixa de valores
    // Retorna a quantidade de itens encontrados
    int consultarFaixa(int id_atributo, int valor_min, int valor_max, int total_itens,
                       int*& itens_saida, int*& valores_saida) const;
};

#endif
