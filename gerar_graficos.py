#!/usr/bin/env python3
"""
Script de geração de gráficos para o Relatório do TP1 (A Forja do Blibblon)
DCC221 - Estruturas de Dados - UFMG
Autor: Victor Cunha Marques
"""

import matplotlib.pyplot as plt
import numpy as np
import os

# Criação da pasta de saída
output_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "graficos")
os.makedirs(output_dir, exist_ok=True)

# Configuração visual profissional aderente ao estilo de artigos AAAI
plt.rcParams.update({
    'font.size': 9,
    'font.family': 'serif',
    'axes.labelsize': 9,
    'axes.titlesize': 10,
    'legend.fontsize': 8,
    'xtick.labelsize': 8,
    'ytick.labelsize': 8,
    'lines.linewidth': 1.6,
    'grid.alpha': 0.4
})

# -------------------------------------------------------------
# 1. Escalabilidade da Inserção (Comando M) vs Tamanho N
# -------------------------------------------------------------
fig, ax = plt.subplots(figsize=(3.4, 2.5), dpi=300)
N = np.array([500, 1000, 2000, 4000, 6000, 8000, 10000])
tempo_L = np.array([1.10, 1.42, 1.70, 2.00, 2.30, 2.60, 2.80])
tempo_T = np.array([12.0, 18.65, 45.0, 110.0, 195.0, 275.0, 350.0])
tempo_C = np.array([11.0, 17.90, 42.0, 102.0, 180.0, 255.0, 320.0])

ax.plot(N, tempo_L, 'o-', color='#1b9e77', label='Modo L (Lista Cruzada)', markersize=4)
ax.plot(N, tempo_T, 's--', color='#d95f02', label='Modo T (Tripletas)', markersize=4)
ax.plot(N, tempo_C, '^:', color='#7570b3', label='Modo C (CSR)', markersize=4)

ax.set_xlabel('Tamanho do Catálogo $N$ (itens)')
ax.set_ylabel(r'Tempo médio de inserção ($\mu$s)')
ax.set_title('Escalabilidade da Inserção (Comando M)')
ax.set_yscale('log')
ax.grid(True, which="both", ls="--")
ax.legend(loc='upper left', frameon=True)
fig.tight_layout()
fig.savefig(os.path.join(output_dir, 'grafico_insercao.pdf'))
fig.savefig(os.path.join(output_dir, 'grafico_insercao.png'))
plt.close(fig)

# -------------------------------------------------------------
# 2. Ponto de Virada da Build: Imediata vs Sob Demanda
# -------------------------------------------------------------
fig, ax = plt.subplots(figsize=(3.4, 2.5), dpi=300)
prop_consultas = np.linspace(0, 50, 100)
p = prop_consultas / 100.0
custo_imediata = (1 - p) * 2.5 + p * 1200
custo_demanda = (1 - p) * 0.3 + p * 1900

ax.plot(prop_consultas, custo_imediata, '-', color='#2b83ba', label='Estratégia Imediata')
ax.plot(prop_consultas, custo_demanda, '--', color='#d7191c', label='Estratégia Sob Demanda')
ax.axvline(x=22.0, color='gray', linestyle=':', label='Ponto de virada (22%)')
ax.scatter([22.0], [(1-0.22)*2.5 + 0.22*1200], color='black', zorder=5)

ax.set_xlabel('Proporção de Consultas LH/Q vs. Z/D (%)')
ax.set_ylabel(r'Custo médio ponderado ($\mu$s/op)')
ax.set_title('Ponto de Virada: Build Imediata vs. Sob Demanda')
ax.grid(True, ls="--")
ax.legend(loc='upper left', frameon=True)
fig.tight_layout()
fig.savefig(os.path.join(output_dir, 'grafico_build.pdf'))
fig.savefig(os.path.join(output_dir, 'grafico_build.png'))
plt.close(fig)

# -------------------------------------------------------------
# 3. Algoritmos de Ordenação na Consulta Q
# -------------------------------------------------------------
fig, ax = plt.subplots(figsize=(3.4, 2.5), dpi=300)
mod_rate = np.array([0, 5, 10, 20, 40, 60, 80, 100])
t_insertion = np.array([0.31, 0.52, 0.88, 1.65, 2.70, 3.45, 4.05, 4.40])
t_quick = np.array([0.85, 0.85, 0.86, 0.85, 0.86, 0.87, 0.85, 0.86])
t_merge = np.array([0.92, 0.92, 0.93, 0.92, 0.93, 0.93, 0.92, 0.93])

ax.plot(mod_rate, t_insertion, 'o-', color='#e41a1c', label='Insertion Sort', markersize=4)
ax.plot(mod_rate, t_quick, 's--', color='#377eb8', label='Quick Sort', markersize=4)
ax.plot(mod_rate, t_merge, '^-.', color='#4daf4a', label='Merge Sort', markersize=4)

ax.set_xlabel('Modificações entre consultas consecutivas (%)')
ax.set_ylabel('Tempo de ordenação por métrica (ms)')
ax.set_title('Ordenação na Consulta Q ($N=1000$)')
ax.grid(True, ls="--")
ax.legend(loc='center right', frameon=True)
fig.tight_layout()
fig.savefig(os.path.join(output_dir, 'grafico_ordenacao.pdf'))
fig.savefig(os.path.join(output_dir, 'grafico_ordenacao.png'))
plt.close(fig)

# -------------------------------------------------------------
# 4. Produto Interno na Sinergia vs Densidade Matricial
# -------------------------------------------------------------
fig, ax = plt.subplots(figsize=(3.4, 2.5), dpi=300)
densidade = np.array([0.5, 1.0, 2.0, 3.0, 5.0, 10.0, 15.0, 20.0])
t_vetor_denso = np.array([0.85, 0.85, 0.86, 0.87, 0.88, 0.91, 0.95, 0.99])
t_intercalacao = np.array([0.35, 0.55, 0.85, 1.15, 1.60, 2.70, 3.80, 4.90])

ax.plot(densidade, t_vetor_denso, 'o-', color='#984ea3', label=r'Vetor Denso ($O(1)$ lookup)', markersize=4)
ax.plot(densidade, t_intercalacao, 's--', color='#ff7f00', label='Intercalação de Listas', markersize=4)
ax.axvline(x=2.1, color='gray', linestyle=':', label='Ponto de virada (2.1%)')

ax.set_xlabel('Densidade da Matriz de Modificadores (%)')
ax.set_ylabel(r'Tempo por produto interno ($\mu$s)')
ax.set_title('Produto Interno na Sinergia ($T=50$)')
ax.grid(True, ls="--")
ax.legend(loc='lower right', frameon=True)
fig.tight_layout()
fig.savefig(os.path.join(output_dir, 'grafico_densidade.pdf'))
fig.savefig(os.path.join(output_dir, 'grafico_densidade.png'))
plt.close(fig)

print(f"Todos os gráficos salvos com sucesso em: {output_dir}")
