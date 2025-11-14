#include <stdio.h>
#include <string.h>


#define MAX_TERR 5  // Número máximo de territórios a serem cadastrados

// -------------------------------------------------------------
// Estrutura que representa um território no jogo
// Contém nome, cor do exército e quantidade de tropas
// -------------------------------------------------------------
typedef struct {
    char nome[30];   // Nome do território
    char cor[10];    // Cor do exército
    int tropas;      // Quantidade de tropas
} Territorio;

int main() {
    // Vetor que armazenará os 5 territórios
    Territorio mapa[MAX_TERR];

    printf("=============================================\n");
    printf("===  Sistema de Cadastro de Territorios  ===\n");
    printf("=============================================\n\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", MAX_TERR);

    // -------------------------------------------------------------
    // Entrada de dados dos territórios
    // O loop percorre de 0 a MAX_TERR - 1, cadastrando cada território
    // -------------------------------------------------------------
    for (int i = 0; i < MAX_TERR; i++) {
        printf("------ Cadastrando Territorio %d ------\n", i + 1);

        // Leitura do nome do território
        printf("Nome do Territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // Remove o '\n'

        // Leitura da cor do exército
        printf("Cor do Exercito (ex: Azul, Verde, Vermelho): ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0'; // Remove o '\n'

        // Leitura da quantidade de tropas
        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // Limpa o buffer do teclado (ENTER)

        printf("\n");
    }

    // -------------------------------------------------------------
    // Exibição dos dados cadastrados
    // -------------------------------------------------------------
    printf("\n=============================================\n");
    printf("===  Territorios cadastrados com sucesso! ===\n");
    printf("=============================================\n\n");

    for (int i = 0; i < MAX_TERR; i++) {
        printf("%d. Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    printf("\n=============================================\n");
    printf("Cadastro finalizado. Boa sorte na conquista!\n");
    printf("=============================================\n");

    return 0;
}
                