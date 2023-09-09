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
 * Função auxiliar que obtém a posição de um cliente c no grafo g.
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
        nome[strcspn(nome, "\n")] = 0;
        printf("Digite o bairro do cliente: ");
        fgets(bairro, 10, stdin);
        bairro[strcspn(bairro, "\n")] = 0;

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
    nome[strcspn(nome, "\n")] = 0;
    printf("Digite o bairro do cliente: ");
    fgets(bairro, 10, stdin);
    bairro[strcspn(bairro, "\n")] = 0;

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
    nome[strcspn(nome, "\n")] = 0;
    printf("Digite o bairro do cliente: ");
    fgets(bairro, 10, stdin);
    bairro[strcspn(bairro, "\n")] = 0;

    int posicao = obterPosicaoDoCliente(nome, bairro, g);

    if (posicao == -1) {
        printf("\nErro: não há cliente com este nome e bairro");
    } else {
        free(g->clientes[posicao]);
        g->clientes[posicao] = (Cliente*) malloc(sizeof(Cliente));
        g->clientes[posicao] = NULL;

        g->n_vert -= 1;
    }
}

/*
 * Função auxiliar que adiciona um caminho entre um vértice e outro de acordo com suas posições no vetor de clientes.
 * Se for via de mão dupla, cria a aresta de mesmo tamanho pra as duas direções. Só cria o caminho se já não houver
 * outro caminho existente.
 */
void adicionarCaminhoEntre(int indexInicio, int indexDestino, int viaMaoDupla, int distancia, Grafo *g) {
    if (indexDestino == -1 || indexInicio == -1) {
        printf("\nErro: o vértice inicial e o destino devem existir.\n");
    } else if (g->m_adj[indexInicio][indexDestino] != 0) {
        printf("\nErro: o caminho entre estes vértices na direção informada já existe");
    } else {
        g->m_adj[indexInicio][indexDestino] = distancia;

        if (viaMaoDupla == 1) {
            g->m_adj[indexDestino][indexInicio] = distancia;
        }
    }
}

/*
 * Função que cria um caminho direcionado entre dois vértices. Solicita ao usuário os dois vértices, se não existirem,
 * apresenta um erro e nada acontece. Se o usuário fornecer dois clientes iguais, também apresenta um erro. Caso contrá-
 * rio, solicita o peso (distância) do caminho e valida se o peso fornecido é maior que 0. Além disso, valida se o cami-
 * nho é uma via de mão dupla, ou seja, se age em ambas as direções. Se sim, cria o caminho do destino para a origem.
 */
void criarCaminhoEntreVertices(Grafo *g) {
    int vertices[2] = {-1, -1};

    for (int i = 0; i < 2; i++) {
        char nome[10];
        char bairro[10];

        printf("Digite o nome do cliente %d: ", i + 1);
        fgets(nome, 10, stdin);
        nome[strcspn(nome, "\n")] = 0;
        printf("Digite o bairro do cliente %d: ", i + 1);
        fgets(bairro, 10, stdin);
        bairro[strcspn(bairro, "\n")] = 0;

        if (obterPosicaoDoCliente(nome, bairro, g) == -1) {
            printf("Erro: o cliente não existe");
            return;
        }

        if (obterPosicaoDoCliente(nome, bairro, g) == vertices[0]) {
            printf("Erro: os clientes não podem ser iguais.");
            return;
        }

        vertices[i] = obterPosicaoDoCliente(nome, bairro, g);
    }

    int distancia;
    printf("Qual a distância percorrida no caminho? ");
    scanf("%d", &distancia);
    getchar();

    if (distancia > 0) {
        adicionarCaminhoEntre(vertices[0], vertices[1], 0, distancia, g);

        int choice;
        printf("O caminho é via de mão dupla? (1 para sim, 0 para não) ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            adicionarCaminhoEntre(vertices[1], vertices[0], choice, distancia, g);
        }
    } else {
        printf("\nErro: a distância deve ser maior que zero\n");
    }
}

/*
 * Função que remove um caminho entre dois vértices. Solicita o cliente de origem e o cliente de destino, se não houver
 * caminho entre eles, apresenta um erro; se os clientes fornecidos forem o mesmo cliente, apresenta um erro; se não,
 * remove o caminho entre os vértices na direção informada.
 */
void removerCaminhoEntreVertices(Grafo *g) {
    int vertices[2] = {-1, -1};

    for (int i = 0; i < 2; i++) {
        char nome[10];
        char bairro[10];

        printf("Digite o nome do cliente %d: ", i + 1);
        fgets(nome, 10, stdin);
        nome[strcspn(nome, "\n")] = 0;
        printf("Digite o bairro do cliente %d: ", i + 1);
        fgets(bairro, 10, stdin);
        bairro[strcspn(bairro, "\n")] = 0;

        if (obterPosicaoDoCliente(nome, bairro, g) == -1) {
            printf("Erro: o cliente não existe");
            return;
        }

        if (obterPosicaoDoCliente(nome, bairro, g) == vertices[0]) {
            printf("Erro: os clientes não podem ser iguais.");
            return;
        }

        vertices[i] = obterPosicaoDoCliente(nome, bairro, g);
    }

    if (g->m_adj[vertices[0]][vertices[1]] > 0) {
        g->m_adj[vertices[0]][vertices[1]] = 0;
    } else {
        printf("\nErro: não há caminho nesta direção entre os vértices citados.");
    }
}

/*
 * Função que imprime a matriz de adjacência de um grafo g.
 */
void imprimirMatrizDeAdjacenciaDe(Grafo *g) {
    printf("\t\t");
    for (int i = 0; i < MAX; i++) {
        if (g->clientes[i] == NULL) {
            printf("(0, 0) ");
        } else {
            printf("(%s, %s) ", g->clientes[i]->nome, g->clientes[i]->bairro);
        }
    }

    printf("\n");

    for (int i = 0; i < MAX; i++) {
        if (g->clientes[i] == NULL) {
            printf("(0, 0) ");
        } else {
            printf("(%s, %s) ", g->clientes[i]->nome, g->clientes[i]->bairro);
        }

        for (int j = 0; j < MAX; j++) {
            printf("%5d  ", g->m_adj[i][j]);
        }
        printf("\n");
    }
}

/*
 * Função principal que exibe um menu de operações e apresenta o resultado das funções.
 */
void exibirMenuDeOperacoes(Grafo *g) {
    int choice = 0;

    do {
        printf("Digite 1 para: adicionar cliente\n");
        printf("Digite 2 para: buscar um cliente por nome e bairro\n");
        printf("Digite 3 para: remover um cliente\n");
        printf("Digite 4 para: adicionar um caminho entre dois clientes\n");
        printf("Digite 5 para: remover um caminho entre dois clientes\n");
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
            case 4:
                criarCaminhoEntreVertices(g);
                break;
            case 5:
                removerCaminhoEntreVertices(g);
                break;
            case 6:
                imprimirMatrizDeAdjacenciaDe(g);
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


