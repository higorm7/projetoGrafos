#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

typedef struct {
    char *nome;
    char *bairro;
} Cliente;

typedef struct {
    Cliente **clientes;
    int **m_adj;
    int n_vert;
} Grafo;

/*
 * Função que cria o grafo: inicializa o vetor de clientes, com o número máximo definido; inicializa a matriz
 * de adjacência, do tipo [MAX][MAX].
 */
Grafo *criarGrafo() {
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));

    g->clientes = (Cliente**) malloc(MAX * sizeof(Cliente*));
    for (int i = 0; i < MAX; i++) {
        g->clientes[i] = (Cliente*) malloc(sizeof(Cliente));
        g->clientes[i] = NULL;
    }

    g->n_vert = 0;
    g->m_adj = (int**) malloc(MAX * sizeof(int*));
    for (int i = 0; i < MAX; i++) {
        g->m_adj[i] = (int*) malloc(MAX * sizeof(int));
    }

    return g;
}

/*
 * Função auxiliar que busca uma posição vazia no vetor de clientes de um grafo g. Retorna o índice da primeira posição
 * disponível encontrada.
 */
int buscarPosicaoVaziaEm(Grafo *g) {
    for (int i = 0; i < MAX; i++) {
        if (g->clientes[i] == NULL) {
            return i;
        }
    }

    return -1;
}

/*
 * Função auxiliar que obtém a posição de um cliente c no grafo g
 */
int obterPosicaoDoCliente(char *nome, char* bairro, Grafo *g) {
    for (int i = 0; i < MAX; i++) {
        if (g->clientes[i] == NULL) {
            continue;
        }

        if (!strcmp(nome, g->clientes[i]->nome) &&
            !strcmp(bairro, g->clientes[i]->bairro)) {
            return i;
        }
    }

    return -1;
}

/*
 * Função que adiciona cliente a um grafo. Verifica se há posições livres no vetor de clientes. Se houver, solicita nome
 * e bairro do novo cliente; se nome e bairro iguais já existirem, nada é criado. Se não existirem, o cliente é criado e
 * a quantidade de vértices aumenta.
 */
void adicionarClienteAo(Grafo *g) {
    if (buscarPosicaoVaziaEm(g) != -1) {
        char nome[10];
        char bairro[10];

        printf("Digite o nome do cliente: ");
        fgets(nome, 10, stdin);
        printf("Digite o bairro do cliente: ");
        fgets(bairro, 10, stdin);

        if (obterPosicaoDoCliente(nome, bairro, g) == -1) {
            int posicao = buscarPosicaoVaziaEm(g);
            Cliente *c = (Cliente*) malloc(sizeof(Cliente));
            c->nome = malloc(MAX * sizeof(char));
            c->bairro = malloc(MAX * sizeof(char));
            strncpy(c->nome, nome, 10);
            strncpy(c->bairro, bairro, 10);

            g->clientes[posicao] = c;
            g->n_vert += 1;
            printf("\nCliente criado com sucesso\n\n");
        } else {
            printf("Já existe um cliente igual cadastrado.\n\n");
        }
    } else {
        printf("Erro: número máximo de clientes alcançado.\n\n");
    }
}

/*
 * Função que imprime as informações do cliente informado pelo usuário.
 */
void mostrarClienteDe(Grafo *g) {
    char nome[10];
    char bairro[10];

    printf("Digite o nome do cliente: ");
    fgets(nome, 10, stdin);
    printf("Digite o bairro do cliente: ");
    fgets(bairro, 10, stdin);

    int posicao = obterPosicaoDoCliente(nome, bairro, g);

    if (posicao != -1) {
        printf("\nNome do cliente: %s", g->clientes[posicao]->nome);
        printf("Bairro do cliente: %s\n\n", g->clientes[posicao]->bairro);
    } else {
        printf("\nCliente informado não existe.");
    }
}

/*
 * Função que remove um cliente do grafo. O cliente é encontrado pelo nome e bairro, se não existir, nada acontece. Caso
 * contrário, o espaço na memória que continha o cliente é liberado e, em seguida, é alocado novo espaço de memória.
 */
void removerClienteDe(Grafo *g) {
    char nome[10];
    char bairro[10];

    printf("Digite o nome do cliente: ");
    fgets(nome, 10, stdin);
    printf("Digite o bairro do cliente: ");
    fgets(bairro, 10, stdin);

    int posicao = obterPosicaoDoCliente(nome, bairro, g);

    if (posicao == -1) {
        printf("\nErro: não há cliente com este nome e bairro");
    } else {
        printf("\n%d\n", posicao);
        free(g->clientes[posicao]);
        g->clientes[posicao] = (Cliente*) malloc(sizeof(Cliente));
        g->clientes[posicao] = NULL;

        g->n_vert -= 1;
    }
}

void exibirMenuDeOperacoes(Grafo *g) {
    int choice = 0;

    do {
        printf("Digite 1 para: adicionar cliente\n");
        printf("Digite 2 para: buscar um cliente por nome e bairro\n");
        printf("Digite 3 para: remover um cliente\n");
        printf("Digite 0 para: sair\n");
        printf("Escolha: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("%d",buscarPosicaoVaziaEm(g));
                adicionarClienteAo(g);
                system("clear");
                break;
            case 2:
                mostrarClienteDe(g);
                system("clear");
                break;
            case 3:
                removerClienteDe(g);
                break;
            case 0:
                break;
        }

        printf("\n\n");
        for (int i = 0; i < MAX; i++) {
            if (g->clientes[i] != NULL) {
                printf ("%s\n", g->clientes[i]->nome);
            }
        }
    } while (choice != 0);
}

int main()
{
    Grafo *g = criarGrafo();
    exibirMenuDeOperacoes(g);


    return 0;
}


