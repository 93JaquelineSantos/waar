
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---------------------- Configurações ---------------------- */
#define MAX_NOME 30
#define MAX_COR  10
#define MISSao_TAMANHO 100

/* ---------------------- Estruturas ------------------------ */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* ---------------------- Protótipos ------------------------ */
void limparBuffer(void);
void limparTela(void);
void exibirBanner(void);
void lerString(char *dest, int tamanho, const char *mensagem);
Territorio* criarMapa(int total);
void cadastrarTerritorios(Territorio *mapa, int total);
void exibirMapa(Territorio *mapa, int total);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
void exibirMissao(const char *missao);
void atacar(Territorio *atacante, Territorio *defensor);
int verificarMissao(char *missao, Territorio *mapa, int tamanho, const char *corJogador, char *missoes[], int totalMissoes);
void liberarMemoria(Territorio *mapa, char *missao1, char *missao2);

/* ---------------------- Implementações ------------------- */

/* Limpa o buffer de entrada (stdin) */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Limpa a tela de forma portátil */
void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    /* tenta usar clear; se falhar, imprime linhas */
    if (system("clear") != 0) {
        for (int i = 0; i < 60; i++) putchar('\n');
    }
#endif
}

/* Exibe cabeçalho/banner do jogo */
void exibirBanner(void) {
    printf("=============================================================\n");
    printf("                  WAR ---- JOGO                    \n");
    printf("=============================================================\n");
}

/* Leitura segura de strings com remoção do newline */
void lerString(char *dest, int tamanho, const char *mensagem) {
    printf("%s", mensagem);
    if (fgets(dest, tamanho, stdin) != NULL) {
        dest[strcspn(dest, "\n")] = '\0';
    } else {
        dest[0] = '\0';
        limparBuffer();
    }
}

/* Cria mapa dinamicamente com calloc e verifica alocação */
Territorio* criarMapa(int total) {
    Territorio *m = (Territorio*) calloc((size_t)total, sizeof(Territorio));
    if (!m) {
        fprintf(stderr, "Erro: falha na alocacao de memoria para o mapa.\n");
        exit(EXIT_FAILURE);
    }
    return m;
}

/* Cadastro interativo de territórios */
void cadastrarTerritorios(Territorio *mapa, int total) {
    for (int i = 0; i < total; i++) {
        printf("\n------ Cadastrando Territorio %d de %d ------\n", i + 1, total);
        lerString(mapa[i].nome, sizeof(mapa[i].nome), "Nome do Territorio: ");
        lerString(mapa[i].cor, sizeof(mapa[i].cor), "Cor do Exercito (ex: Azul, Verde, Vermelho): ");

        /* validação do número de tropas */
        int tropas = -1;
        char buf[64];
        do {
            printf("Numero de Tropas (inteiro >= 0): ");
            if (!fgets(buf, sizeof(buf), stdin)) { buf[0] = '\0'; }
            tropas = atoi(buf); /* atoi retorna 0 para strings não-numéricas */
            if (tropas < 0) {
                printf("Valor invalido. Digite um numero inteiro maior ou igual a zero.\n");
            }
        } while (tropas < 0);
        mapa[i].tropas = tropas;
    }
}

/* Exibe o mapa com índices para seleção pelo usuário */
void exibirMapa(Territorio *mapa, int total) {
    printf("\n-------------------- MAPA ATUAL --------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%2d) %-25s | Cor: %-10s | Tropas: %3d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("----------------------------------------------------\n");
}

/* Sorteia uma missão e copia para destino (destino deve ter espaço) */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

/* Exibe missão (passagem por valor/const) */
void exibirMissao(const char *missao) {
    printf(">>> Sua missão: %s\n", missao);
}

/* Função que realiza um ataque entre dois territórios (ponteiros) */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (!atacante || !defensor) return;

    printf("\n--- Ataque: %s (%s, %d tropas) --> %s (%s, %d tropas) ---\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;
    printf("Rolagem: %s (atk) = %d | %s (def) = %d\n", atacante->nome, dadoAtk, defensor->nome, dadoDef);

    if (dadoAtk > dadoDef) {
        /* Atacante vence: transfere metade das tropas (arredondadas para baixo), mínimo 1 */
        int movimento = atacante->tropas / 2;
        if (movimento < 1) movimento = 1;
        printf("Resultado: ATACANTE VENCE! %d tropas serão transferidas para %s\n", movimento, defensor->nome);

        /* transferir tropas e mudar cor do defensor */
        if (movimento > atacante->tropas) movimento = atacante->tropas;
        atacante->tropas -= movimento;
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas += movimento;
    } else {
        /* Atacante perde 1 tropa (se possuir) */
        printf("Resultado: DEFENSOR RESISTE! Atacante perde 1 tropa.\n");
        if (atacante->tropas > 0) atacante->tropas -= 1;
    }

    printf("----- Pos-ataque -----\n");
    printf("%s: Cor=%s Tropas=%d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("%s: Cor=%s Tropas=%d\n", defensor->nome, defensor->cor, defensor->tropas);
    printf("----------------------\n");
}

/* Verificação simples de missão.
   Retorna 1 se cumprida, 0 caso contrário.
   missao: string da missão
   mapa/tamanho: estado atual
   corJogador: cor do jogador para avaliar condição
   missoes[]/totalMissoes: para identificar qual missão foi sorteada
*/
int verificarMissao(char *missao, Territorio *mapa, int tamanho, const char *corJogador, char *missoes[], int totalMissoes) {
    if (!missao || !mapa || !corJogador) return 0;

    int id = -1;
    for (int i = 0; i < totalMissoes; i++) {
        if (strcmp(missao, missoes[i]) == 0) {
            id = i;
            break;
        }
    }
    if (id == -1) return 0;

    switch (id) {
        case 0: { /* "Conquistar 2 territorios" */
            int cont = 0;
            for (int i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
            return (cont >= 2);
        }
        case 1: { /* "Conquistar 3 territorios" */
            int cont = 0;
            for (int i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
            return (cont >= 3);
        }
        case 2: { /* "Possuir 10 tropas no total" */
            int soma = 0;
            for (int i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) soma += mapa[i].tropas;
            return (soma >= 10);
        }
        case 3: { /* "Eliminar todas as tropas da cor Vermelho" */
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) return 0;
            }
            return 1;
        }
        case 4: { /* "Conquistar 1 territorio adversario" */
            int cont = 0;
            for (int i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
            return (cont >= 1);
        }
        default:
            return 0;
    }
}

/* Libera memória alocada dinamicamente */
void liberarMemoria(Territorio *mapa, char *missao1, char *missao2) {
    if (mapa) free(mapa);
    if (missao1) free(missao1);
    if (missao2) free(missao2);
}

/* ---------------------- Fluxo principal ------------------- */
int main(void) {
    srand((unsigned) time(NULL));

    limparTela();
    exibirBanner();

    /* Número de territórios (dinâmico) */
    int totalTerritorios = 0;
    char linha[64];
    do {
        printf("Digite o numero total de territorios (min 2): ");
        if (!fgets(linha, sizeof(linha), stdin)) linha[0] = '\0';
        totalTerritorios = atoi(linha);
        if (totalTerritorios < 2) printf("Informe um valor inteiro >= 2.\n");
    } while (totalTerritorios < 2);

    /* Cria e cadastra territórios */
    Territorio *mapa = criarMapa(totalTerritorios);
    cadastrarTerritorios(mapa, totalTerritorios);

    /* Cores dos jogadores */
    char corJog1[MAX_COR], corJog2[MAX_COR];
    lerString(corJog1, sizeof(corJog1), "Cor do Jogador 1 (ex: Azul): ");
    lerString(corJog2, sizeof(corJog2), "Cor do Jogador 2 (ex: Vermelho): ");

    /* Vetor de missões disponíveis */
    char *missoesDisponiveis[] = {
        "Conquistar 2 territorios",
        "Conquistar 3 territorios",
        "Possuir 10 tropas no total",
        "Eliminar todas as tropas da cor Vermelho",
        "Conquistar 1 territorio adversario"
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    /* Aloca missões dinamicamente para cada jogador */
    char *missaoJog1 = (char*) malloc(MISSao_TAMANHO);
    char *missaoJog2 = (char*) malloc(MISSao_TAMANHO);
    if (!missaoJog1 || !missaoJog2) {
        fprintf(stderr, "Erro: falha na alocaçao de memoria para missoes.\n");
        liberarMemoria(mapa, missaoJog1, missaoJog2);
        return EXIT_FAILURE;
    }

    atribuirMissao(missaoJog1, missoesDisponiveis, totalMissoes);
    atribuirMissao(missaoJog2, missoesDisponiveis, totalMissoes);

    /* Exibe missões apenas uma vez no início */

    printf("\n---------------------- MISSOES ----------------------\n");
    printf("Jogador 1 (%s): ", corJog1); exibirMissao(missaoJog1);
    printf("Jogador 2 (%s): ", corJog2); exibirMissao(missaoJog2);
    printf("-----------------------------------------------------\n\n");
    printf("Pressione ENTER para iniciar o jogo...");
    limparBuffer();

    /* Loop de turnos */
    int turno = 1;
    int vencedor = 0;
    while (!vencedor) {
        limparTela();
        exibirBanner();
        printf("\nTurno %d\n", turno);
        exibirMapa(mapa, totalTerritorios);

        int jogadorAtual = (turno % 2 == 1) ? 1 : 2;
        const char *corAtual = (jogadorAtual == 1) ? corJog1 : corJog2;
        const char *missaoAtual = (jogadorAtual == 1) ? missaoJog1 : missaoJog2;

        printf("\nVez do Jogador %d (cor %s)\n", jogadorAtual, corAtual);
        printf("Missao: %s\n", missaoAtual);
        printf("\nAçoes disponiveis:\n");
        printf(" 1) Atacar\n");
        printf(" 2) Passar turno\n");
        printf(" 3) Mostrar missao (novamente)\n");
        printf(" 0) Sair do jogo\n");
        printf("Escolha a ação: ");

        int acao = -1;
        if (!fgets(linha, sizeof(linha), stdin)) linha[0] = '\0';
        acao = atoi(linha);

        if (acao == 0) {
            printf("Jogo encerrado pelo jogador.\n");
            break;
        } else if (acao == 1) {
            /* Escolha do território atacante */

            int idxAtk = -1, idxDef = -1;
            do {
                printf("Informe o numero do territorio ATACANTE (ou 0 para cancelar): ");
                if (!fgets(linha, sizeof(linha), stdin)) linha[0] = '\0';
                idxAtk = atoi(linha) - 1;
                if (idxAtk == -1) { printf("Ataque cancelado.\n"); break; }
                if (idxAtk < 0 || idxAtk >= totalTerritorios) {
                    printf("Indice invalido. Tente novamente.\n");
                    idxAtk = -2; continue;
                }
                if (strcmp(mapa[idxAtk].cor, corAtual) != 0) {
                    printf("Voce deve escolher um territorio de sua cor (%s).\n", corAtual);
                    idxAtk = -2; continue;
                }
                if (mapa[idxAtk].tropas < 2) {
                    printf("Territorio precisa ter pelo menos 2 tropas para atacar (possui %d).\n", mapa[idxAtk].tropas);
                    idxAtk = -2; continue;
                }
                break;
            } while (1);

            if (idxAtk >= 0) {
                /* Escolha do defensor */
                do {
                    printf("Informe o numero do territorio DEFENSOR (ou 0 para cancelar): ");
                    if (!fgets(linha, sizeof(linha), stdin)) linha[0] = '\0';
                    idxDef = atoi(linha) - 1;
                    if (idxDef == -1) { printf("Ataque cancelado.\n"); break; }
                    if (idxDef < 0 || idxDef >= totalTerritorios) {
                        printf("Indice invalido. Tente novamente.\n");
                        idxDef = -2; continue;
                    }
                    if (idxDef == idxAtk) {
                        printf("Atacante e defensor nao podem ser o mesmo.\n");
                        idxDef = -2; continue;
                    }
                    if (strcmp(mapa[idxDef].cor, corAtual) == 0) {
                        printf("Voce nao pode atacar um territorio da propria cor.\n");
                        idxDef = -2; continue;
                    }
                    break;
                } while (1);

                if (idxDef >= 0) {
                    atacar(&mapa[idxAtk], &mapa[idxDef]);
                }
            }

        } else if (acao == 2) {
            printf("Turno passado.\n");
        } else if (acao == 3) {
            printf("\nReexibindo missao: %s\n", missaoAtual);
            printf("Pressione ENTER para continuar...");
            limparBuffer();
        } else {
            printf("Opçao invalida. Pulando turno.\n");
        }

        /* Verificação de missão do jogador atual (após ação) */

        if (verificarMissao((char*)missaoAtual, mapa, totalTerritorios, corAtual, missoesDisponiveis, totalMissoes)) {
            limparTela();
            exibirBanner();
            printf("\n*** PARABENS! Jogador %d (cor %s) cumpriu sua missao: %s ***\n", jogadorAtual, corAtual, missaoAtual);
            vencedor = jogadorAtual;
            break;
        }

        /* Também verificar se o adversário já cumpriu (pode ocorrer por reação anterior) */

        const char *corAdversario = (jogadorAtual == 1) ? corJog2 : corJog1;
        const char *missaoAdv = (jogadorAtual == 1) ? missaoJog2 : missaoJog1;
        if (verificarMissao((char*)missaoAdv, mapa, totalTerritorios, corAdversario, missoesDisponiveis, totalMissoes)) {
            limparTela();
            exibirBanner();
            printf("\n*** ATENÇAO! Jogador adversário já cumpriu a missao: %s ***\n", missaoAdv);
            vencedor = (jogadorAtual == 1) ? 2 : 1;
            break;
        }

        printf("Pressione ENTER para próximo turno...");
        limparBuffer();
        turno++;
    }

    /* Resultado final */

    if (vencedor == 1) {
        printf("\n>>> Vencedor: Jogador 1 (cor %s)\n", corJog1);
    } else if (vencedor == 2) {
        printf("\n>>> Vencedor: Jogador 2 (cor %s)\n", corJog2);
    } else {
        printf("\nNenhum vencedor declarado. Encerrando o jogo.\n");
    }

    /* Libera memória */


    liberarMemoria(mapa, missaoJog1, missaoJog2);

    printf("\nMemória liberada. Obrigado por jogar!\n");

    return 0;
}