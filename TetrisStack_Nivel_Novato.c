/* ============================================================= */
/*  TETRIS STACK – Fila de Peças Futuras (Next Queue)           */

/* ============================================================= */

#include <stdio.h>      // Permite usar printf, scanf, etc. (entrada e saída)
#include <stdlib.h>     // Permite usar malloc, free, rand, srand, exit...
#include <time.h>       // Permite usar time() para gerar números aleatórios diferentes a cada execução

/* ------------------- DEFINIÇÃO DA PEÇA ----------------------- */
// Aqui criamos um "molde" chamado Peca, como se fosse uma ficha de identificação
typedef struct {
    char nome;   // Guarda a letra da peça: 'I', 'O', 'T', etc.
    int  id;     // Número único para cada peça criada (como um RG)
} Peca;          // Agora podemos criar variáveis do tipo "Peca"

/* ------------------- CONFIGURAÇÕES DA FILA ------------------- */
#define TAMANHO_FILA 5                    // A fila vai ter exatamente 5 posições (como no Tetris real)
Peca fila[TAMANHO_FILA];                  // O "armário" com 5 gavetas onde guardamos as peças
int frente = 0;                           // Aponta para a peça que vai sair agora (primeira da fila)
int tras   = 0;                           // Aponta para onde vamos colocar a próxima peça (final da fila)
int quantidade = 0;                       // Conta quantas peças estão na fila no momento
int proximoId = 0;                        // Contador que aumenta toda vez que criamos uma peça nova

/* ------------------- TIPOS DE PEÇAS DISPONÍVEIS ------------- */
// Lista com as 7 peças clássicas do Tetris
const char tiposPeca[7] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

/* ============================================================= */
/*  Função: gerarPeca()                                          */
/*  Cria uma peça nova, aleatória, com ID único                  */
/* ============================================================= */
Peca gerarPeca() {
    Peca nova;                              // Cria uma peça temporária
    int indiceAleatorio = rand() % 7;       // Gera número entre 0 e 6 (aleatório)
    nova.nome = tiposPeca[indiceAleatorio]; // Pega uma letra aleatória da lista
    nova.id   = proximoId++;                // Dá um número único e já aumenta o contador
    return nova;                            // Devolve a peça pronta
}

/* ============================================================= */
/*  Função: enqueue() – coloca uma peça no final da fila         */
/*  É como "enfileirar" alguém no final da fila do pão           */
/* ============================================================= */
int enqueue() {
    if (quantidade == TAMANHO_FILA) {           // Verifica se a fila já está cheia (5 peças)
        printf("  ERRO: Fila cheia! Não é possível adicionar mais peças.\n");
        return 0;                               // Falhou (não adicionou)
    }

    Peca nova = gerarPeca();                    // Gera uma peça nova automaticamente
    fila[tras] = nova;                          // Coloca a peça na posição "trás" (final)
    tras = (tras + 1) % TAMANHO_FILA;           // Avança o "tras". O % faz virar circular!
                                                // Ex: 4 + 1 = 5 → 5 % 5 = 0 (volta pro início)
    quantidade++;                               // Aumenta o contador de peças na fila
    printf("  Adicionada peça [%c %d]\n", nova.nome, nova.id);
    return 1;                                   // Sucesso!
}

/* ============================================================= */
/*  Função: dequeue() – remove a peça da frente da fila          */
/*  É como "atender" a primeira pessoa da fila                   */
/* ============================================================= */
int dequeue() {
    if (quantidade == 0) {                      // Verifica se a fila está vazia
        printf("  ERRO: Fila vazia! Não há peça para jogar.\n");
        return 0;                               // Falhou
    }

    Peca removida = fila[frente];               // Pega a peça que está na frente
    printf("  Jogou peça [%c %d]\n", removida.nome, removida.id);

    frente = (frente + 1) % TAMANHO_FILA;       // Avança o ponteiro da frente (circular)
    quantidade--;                               // Reduz o número de peças na fila
    return 1;                                   // Sucesso!
}

/* ============================================================= */
/*  Função: exibirFila() – mostra todas as peças na ordem        */
/* ============================================================= */
void exibirFila() {
    printf("Fila de peças futuras: ");
    if (quantidade == 0) {                      // Se não tem nenhuma peça
        printf("<vazia>\n");
        return;                                 // Sai da função
    }

    // Percorre a fila começando da "frente" e vai até ter mostrado todas
    int i = frente;                             // Começa do início da fila
    int cont = 0;                               // Contador de quantas já mostrei
    while (cont < quantidade) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);  // Mostra peça atual
        i = (i + 1) % TAMANHO_FILA;             // Vai para próxima posição (circular)
        cont++;                                 // Conta +1
    }
    printf("\n");                               // Pula linha no final
}

/* ============================================================= */
/*  Função: inicializarFila() – preenche com 5 peças no início   */
/* ============================================================= */
void inicializarFila() {
    printf("Inicializando fila com 5 peças...\n");
    while (quantidade < TAMANHO_FILA) {         // Enquanto não tiver 5 peças
        enqueue();                              // Adiciona uma nova (usa a função pronta)
    }
    printf("\n");                               // Linha em branco para separar
}

/* ============================================================= */
/*  Função: menu() – mostra as opções para o jogador             */
/* ============================================================= */
void menu() {
    printf("\n");                               // Pula uma linha
    printf("======================================\n");
    exibirFila();                               // Mostra o estado atual da fila
    printf("--------------------------------------\n");
    printf("Opções de ação:\n");
    printf("1 - Jogar peça (dequeue)\n");       // Remove da frente
    printf("2 - Inserir nova peça (enqueue)\n"); // Adiciona no final
    printf("0 - Sair\n");
    printf("Escolha: ");                        // Espera o jogador digitar
}

/* ============================================================= */
/*  main() – o coração do programa                               */
/*  É aqui que tudo começa a rodar                               */
/* ============================================================= */
int main() {
    srand(time(NULL));          // Faz o rand() gerar números diferentes a cada execução
                                // Sem isso, toda vez seria a mesma sequência!

    inicializarFila();          // Preenche a fila com 5 peças no começo do jogo

    int opcao;                  // Variável que guarda a escolha do jogador
    do {                        // Repete até o jogador digitar 0
        menu();                 // Mostra o menu
        scanf("%d", &opcao);    // Lê o número que o jogador digitou

        switch (opcao) {        // Verifica qual número foi digitado
            case 1:
                dequeue();      // Remove a peça da frente
                enqueue();      // E já adiciona uma nova no final (comportamento real do Tetris!)
                break;          // Sai do switch

            case 2:
                enqueue();      // Apenas adiciona uma peça nova (útil para testar fila cheia)
                break;

            case 0:
                printf("Saindo do jogo. Até mais!\n");
                break;          // Sai do loop

            default:            // Se digitou qualquer outro número
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);       // Continua enquanto não for 0

    return 0;                   // Termina o programa com sucesso
}