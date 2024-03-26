#include "loja.h"

#include <string.h>

#define PRODUTOS_SZ 6
#define CLIENTES_SZ 64
#define STRING_SZ 64

Cliente Clientes[CLIENTES_SZ];
int ClientesSz = 0;
Produto Produtos[PRODUTOS_SZ];

double DescontoPorcentagem = 0.9;
int DescontoQuantidadeMinima = 3;

int main()
{
	Produtos[0] = criarProduto(0, "Bermuda", "Nike", 100, 10);
	Produtos[1] = criarProduto(1, "Camisa Social", "Nike", 80, 10);
	Produtos[2] = criarProduto(2, "Tenis", "Nike", 250, 10);
	Produtos[3] = criarProduto(3, "Calca Moletom", "Adidas", 150, 10);
	Produtos[4] = criarProduto(4, "Camisa Dryfit", "Adidas", 60, 10);
	Produtos[5] = criarProduto(5, "Sapatenis", "Adidas", 200, 10);

	while (true)
	{
		char continueKey;
		int option;

		system("cls");
		printf("Escolha uma das opcoes abaixo \n");
		printf("  [0] Gerar relatorio\n");
		printf("  [1] Entrar na loja\n");
		printf(">: ");
		scanf_s("%d", &option);
		system("cls");
		if (option == 0)
		{
			qsort((void*)Clientes, sizeof(Clientes) / sizeof(Clientes[0]), sizeof(Clientes[0]), compararClientesDecrescente);
			printf("> Total gasto por cliente: \n");
			for (int i = 0; i < ClientesSz; i++)
			{
				printf("  (%d) %s -> R$%.2f\n", Clientes[i].id, Clientes[i].nome, Clientes[i].totalGasto);
			}
		}
		else if (option == 1)
		{
			int resultado = compra();
			if (resultado)
			{
				printf("Obrigado por comprar com a gente!\n\n");
			}
			else
			{
				printf("Que pena que nao comprou nada, volte sempre!\n\n");
			}
		}
		else
		{
			printf("Opcao invalida.\n\n");
		}

		printf("Aperte qualquer tecla para reiniciar...");
		scanf_s(" %c", &continueKey, 1);
	}
}

int compararClientesDecrescente(const void* v1, const void* v2)
{
	const Cliente* p1 = (Cliente*)v1;
	const Cliente* p2 = (Cliente*)v2;
	if (p1->totalGasto > p2->totalGasto)
		return -1;
	else if (p1->totalGasto < p2->totalGasto)
		return +1;
	else
		return 0;
}

time_t gerarIdCliente()
{
	return time(NULL);
}

void inicializarCarrinho(ProdutoNoCarrinho* carrinho, int size)
{	
	for (int i = 0; i < size; i++)
	{
		carrinho[i].quantidade = 0;
	}
}

bool compra()
{
	Cliente cliente;
	Produto* produto;
	ProdutoNoCarrinho carrinho[PRODUTOS_SZ];
	int carrinhoSz = 0;

	inicializarCarrinho(carrinho, PRODUTOS_SZ);

	cliente.id = gerarIdCliente();
	printf("Ola, para comecarmos digite seu nome para realizarmos seu cadastro: ");
	scanf_s("%s", &cliente.nome, STRING_SZ);
	system("cls");

	printf("> %s, bem vindo a loja 'BomDiaPessoal' <\n", cliente.nome);
	printf("> Aproveite que estamos com uma queima de estoque, adicionando 3 itens do mesmo produto no carrinho obtenha 10%% de desconto! <\n\n");

	while (true)
	{
		ProdutoNoCarrinho produto2;

		int idProduto = obterId(Produtos, PRODUTOS_SZ);
		if (idProduto == 8)
		{
			return false;
		}
		if (idProduto == 9)
		{
			if (carrinhoSz > 0)
				break;
			system("cls");
			printf("Adicione algum item ao carrinho antes de prosseguir para o checkout.\n\n");
			continue;
		}
		else if (idProduto < 0 || idProduto >= PRODUTOS_SZ)
		{
			printf("Produto invalido\n\n");
			continue;
		}
		produto2.produto = &Produtos[idProduto];

		int quantidade = 0;
		while (true)
		{
			quantidade = obterQuantidade(produto2.produto);
			if (quantidade < 0)
			{
				printf("Quantidade invalida.\n");
				continue;
			}
			else if (quantidade > produto2.produto->quantidade)
			{
				printf("Estoque insuficiente.\n\n");
				continue;
			}
			break;
		}

		produto2.quantidade = quantidade;
		if (produto2.quantidade >= DescontoQuantidadeMinima)
			produto2.preco = (double)(produto2.produto->preco * (double)produto2.quantidade) * DescontoPorcentagem;
		else
			produto2.preco = (double)(produto2.produto->preco * (double)produto2.quantidade);
		carrinho[produto2.produto->codigo] = produto2;

		system("cls");
		if (produto2.quantidade > 0)
		{
			carrinhoSz++;
			printf("%d produto(s) '%s (%s)' adicionando(s) ao carrinho por R$%.2f.\n\n", produto2.quantidade, produto2.produto->nome, produto2.produto->marca, produto2.preco);
		}
		else
		{
			carrinhoSz--;
			printf("O produto(s) '%s (%s)' foi removido do carrinho.\n\n", produto2.produto->nome, produto2.produto->marca);
		}
		printarCarrinho(carrinho, PRODUTOS_SZ);
	}

	system("cls");
	printf("> Checkout\n\n");
	printarCarrinho(carrinho, PRODUTOS_SZ);

	cliente.totalGasto = 0;
	for (int i = 0; i < PRODUTOS_SZ; i++)
	{
		if (carrinho[i].quantidade <= 0) continue;
		cliente.totalGasto += carrinho[i].preco;
	}
	Clientes[ClientesSz] = cliente;
	ClientesSz++;

	return true;
}

Produto criarProduto(unsigned int codigo, const char* nome, const char* marca, double preco, int quantidade)
{
	Produto produto;
	produto.quantidade = quantidade;
	produto.codigo = codigo;
	strcpy_s(produto.nome, sizeof(produto.nome), nome);
	strcpy_s(produto.marca, sizeof(produto.marca), marca);
	produto.preco = preco;
	return produto;
}

int obterId(Produto* produtos, int produtosSz)
{
	printf("Produtos disponiveis:\n");
	for (int i = 0; i < produtosSz; i++)
	{
		Produto* produto = &produtos[i];
		printf("  [%d] -> %s (%s) = R$%.2f\n", produto->codigo, produto->nome, produto->marca, produto->preco);
	}
	printf("\n");
	printf("  [8] -> Desistir da compra.\n");
	printf("  [9] -> Ir para o checkout.\n");
	int idProduto;
	printf("Digite o numero do produto desejado: ");
	scanf_s("%d", &idProduto);
	printf("\n");
	return idProduto;
}

int obterQuantidade(Produto* produto)
{
	printf("Produto selecioando '%s' (estoque: %d) \n", produto->nome, produto->quantidade);
	printf("Lembrando que adicionando 3 ou mais itens do mesmo produto voce ganha 10%% de desconto!\n");
	printf("Quantidade de itens do produto (0 para remover o item): ");
	int quantidade;
	scanf_s("%d", &quantidade);
	printf("\n");
	return quantidade;
}

void printarCarrinho(ProdutoNoCarrinho* carrinho, int produtosSz)
{
	printf("> Carrinho\n");
	for (int i = 0; i < produtosSz; i++)
	{
		ProdutoNoCarrinho* produto2 = &carrinho[i];
		if (!produto2 || !produto2->produto) { continue; }
		if (produto2->quantidade <= 0) { continue; }
		printf("  '%s (%s)'-> R$%.2f (%dx %.2f)", produto2->produto->nome, produto2->produto->marca, produto2->preco, produto2->quantidade, produto2->produto->preco);
		if (produto2->quantidade >= 3)
		{
			printf(" [10%% OFF] ");
		}
		printf("\n");
	}
	printf("\n");
}
