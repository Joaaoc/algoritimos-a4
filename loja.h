#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct _Cliente
{
	int id;
	char nome[64];
	int vendas;
	double totalGasto;
} Cliente;

typedef struct _Produto
{
	unsigned int codigo;
	char nome[64];
	char marca[64];
	double preco;
	int quantidade;
} Produto;

typedef struct _ProdutoNoCarrinho
{
	Produto* produto;
	int quantidade;
	double preco;
} ProdutoNoCarrinho;

int main();
bool compra();
Produto criarProduto(unsigned int codigo, const char* nome, const char* marca, double preco, int quantidade);
int obterId(Produto* produtos, int produtosSz);
int obterQuantidade(Produto* produto);
void printarCarrinho(ProdutoNoCarrinho* carrinho, int produtosSz);
time_t gerarIdCliente();
int compararClientesDecrescente(const void* v1, const void* v2);
