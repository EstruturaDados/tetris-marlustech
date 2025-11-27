/* ============================================================= */
/*  TETRIS STACK – NÍVEL AVENTUREIRO                           */
/* ============================================================= */

#include <stdio.h>      // Permite usar printf, scanf (entrada/saída no terminal)
#include <stdlib.h>     // Permite usar malloc, free, exit, etc.
#include <time.h>       // Permite usar time() para gerar números aleatórios diferentes a cada execução

/* ------------------- DEFINIÇÃO DA PEÇA ----------------------- */
// Crio um "molde" chamado Peca que representa uma peça do Tetris
typedef struct {
    char nome;   // Guarda a letra da peça: 'I', 'O', 'T', 'L', 'J', 'S', 'Z'
    int  id;     // Número único para identificar cada peça criada (como um CPF da peça)
} Peca;          // Agora posso criar variáveis do tipo Peca

/* ------------------- CONFIGURAÇÕES DA FILA (Next Queue) ------ */
#define TAMANHO_FILA 5                    // A fila sempre tem exatamente 5 posições
Peca fila[TAMANHO_FILA];                  // O "armário" com 5 gavetas onde ficam as próximas peças
int frente = 0;                           // Aponta para a peça que vai cair AGORA (índice da frente)
int tras   = 0;                           // Aponta para onde vamos colocar a próxima peça nova
int qtdFila = 0;                          // Quantas peças estão na fila no momento (começa com 0)
int proximoId = 0;                        // Contador global: toda peça nova recebe esse número e ele aumenta

/* ------------------- CONFIGURAÇÕES DA PILHA (Hold) ----------- */
#define TAMANHO_PILHA 3                   // A reserva (Hold) só aceita até 3 peças
Peca pilha[TAMANHO_PILHA];                // O "armário" da reserva (pilha)
int topo = -1;                            // -1 = pilha vazia. Quando empilhamos vira 0, 1, 2...
int qtdPilha = 0;                         // Quantas peças estão reservadas agora

/* ------------------- TIPOS DE PEÇAS DO TETRIS ---------------- */
const char tiposPeca[7] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};  // As 7 peças clássicas

/* ============================================================= */
/*  Função: gerarPeca()                                          */
/*  Cria uma peça totalmente nova e aleatória                   */
/* ============================================================= */
Peca gerarPeca() {
    Peca nova;                              // Crio uma peça temporária
    nova.nome = tiposPeca[rand() % 7];      // rand() % 7 → número de 0 a 6 → escolhe uma letra aleatória
    nova.id   = proximoId++;                // Pego o próximo número disponível e já aumento o contador
    return nova;                            // Devolvo a peça pronta para usar
}

/* ============================================================= */
/*  Função: enqueue()                                            */
/*  Adiciona uma peça nova no FINAL da fila (circular)           */
/* ============================================================= */
void enqueue() {
    if (qtdFila < TAMANHO_FILA) {           // Só adiciona se ainda tiver espaço (normalmente sempre tem)
        Peca nova = gerarPeca();            // Gero uma peça aleatória
        fila[tras] = nova;                  // Coloco ela na posição "tras" (final da fila)
        tras = (tras + 1) % TAMANHO_FILA;   // Avanço o ponteiro circular (ex: 4+1 → 0)
        qtdFila++;                          // Aumento a quantidade de peças na fila
        printf("  → Nova peça na fila: [%c %d]\n", nova.nome, nova.id);
    }
    // Se a fila já tiver 5, não faz nada (mantém cheia)
}

/* ============================================================= */
/*  Função: dequeue()                                            */
/*  Remove e devolve a peça da FRENTE da fila                    */
/* ============================================================= */
Peca dequeue() {
    Peca removida = fila[frente];           // Pego a peça que está na frente
    frente = (frente + 1) % TAMANHO_FILA;   // Avanço o ponteiro da frente (circular)
    qtdFila--;                              // Diminuo a quantidade de peças na fila
    return removida;                        // Devolvo a peça removida
}

/* ============================================================= */
/*  Função: pushHold()                                           */
/*  Reserva (Hold) a peça que está na frente da fila            */
/* ============================================================= */
void pushHold() {
    if (qtdPilha >= TAMANHO_PILHA) {        // Verifico se a reserva já está cheia (3 peças)
        printf("  Pilha de reserva cheia! (máx. 3)\n");
        return;                             // Não faço nada
    }
    if (qtdFila == 0) {                     // Segurança (nunca deve acontecer)
        printf("  Fila vazia! Nada para reservar.\n");
        return;
    }

    Peca peca = dequeue();                  // Tiro a peça da frente da fila
    topo++;                                 // Avanço o topo da pilha (ex: -1 → 0)
    pilha[topo] = peca;                     // Coloco a peça no topo da pilha
    qtdPilha++;                             // Aumento contador da pilha
    printf("  Reservou peça [%c %d] na pilha!\n", peca.nome, peca.id);

    enqueue();                              // Como tirei uma da fila, gero uma nova → fila volta a ter 5
}

/* ============================================================= */
/*  Função: popHold()                                            */
/*  Usa a peça que está reservada (topo da pilha)                */
/* ============================================================= */
void popHold() {
    if (qtdPilha == 0) {                    // Verifico se tem peça reservada
        printf("  Pilha de reserva vazia! Nada para usar.\n");
        return;
    }

    Peca usada = pilha[topo];               // Pego a peça do topo da pilha
    printf("  Usou peça reservada [%c %d]\n", usada.nome, usada.id);

    topo--;                                 // Volto o topo da pilha (ex: 1 → 0)
    qtdPilha--;                             // Diminuo o contador da pilha

    // Agora jogo a peça que estava na frente da fila (comportamento real do Tetris)
    Peca jogada = dequeue();
    printf("  Jogou peça da fila [%c %d]\n", jogada.nome, jogada.id);

    enqueue();                              // Gero uma nova peça → fila volta a ter 5
}

/* ============================================================= */
/*  Função: jogarPecaNormal()                                   */
/*  Joga a peça que está na frente da fila (ação comum)         */
/* ============================================================= */
void jogarPecaNormal() {
    if (qtdFila == 0) {                     // Segurança
        printf("  Fila vazia!\n");
        return;
    }
    Peca jogada = dequeue();                // Tiro da frente
    printf("  Jogou peça [%c %d]\n", jogada.nome, jogada.id);
    enqueue();                              // Gero nova peça → fila continua com 5
}

/* ============================================================= */
/*  Função: exibirFila()                                         */
/*  Mostra todas as peças da fila na ordem correta              */
/* ============================================================= */
void exibirFila() {
    printf("Fila de peças futuras : ");
    if (qtdFila == 0) {
        printf("<vazia>\n");
        return;
    }
    int i = frente;                         // Começo pela peça que vai cair agora
    for (int c = 0; c < qtdFila; c++) {     // Repito quantas peças tiver
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        i = (i + 1) % TAMANHO_FILA;         // Avanço circularmente
    }
    printf("\n");
}

/* ============================================================= */
/*  Função: exibirPilha()                                        */
/*  Mostra as peças reservadas (do fundo até o topo)            */
/* ============================================================= */
void exibirPilha() {
    printf("Pilha de reserva (Hold): ");
    if (qtdPilha == 0) {
        printf("<vazia>\n");
    } else {
        for (int i = topo; i >= 0; i--) {   // Mostro do topo até o fundo
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
        printf(" ← topo\n");                // Indico onde está o topo
    }
}

/* ============================================================= */
/*  Função: inicializar()                                        */
/*  Preenche a fila com as 5 peças iniciais                     */
/* ============================================================= */
void inicializar() {
    printf("=== TETRIS STACK – NÍVEL AVENTUREIRO ===\n");
    printf("Inicializando fila com 5 peças...\n");
    while (qtdFila < TAMANHO_FILA) {        // Enquanto não tiver 5
        enqueue();                          // Adiciono uma por uma
    }
    printf("\n");
}

/* ============================================================= */
/*  Função: menu()                                               */
/*  Mostra o estado atual do jogo e as opções                   */
/* ============================================================= */
void menu() {
    printf("╔════════════════════════════════════════╗\n");
    exibirFila();                           // Mostra as próximas 5 peças
    exibirPilha();                          // Mostra o que está reservado
    printf("╠────────────────────────────────────────╣\n");
    printf("║  1 - Jogar peça atual                  ║\n");
    printf("║  2 - Reservar peça (Hold)              ║\n");
    printf("║  3 - Usar peça reservada               ║\n");
    printf("║  0 - Sair                              ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Escolha → ");
}

/* ============================================================= */
/*  main() – onde o programa realmente começa                   */
/* ============================================================= */
int main() {
    srand(time(NULL));      // Faz o rand() gerar sequências diferentes a cada execução
    inicializar();          // Preenche a fila com 5 peças no começo do jogo

    int op;                 // Variável que guarda a opção digitada pelo jogador
    do {                    // Repete até o jogador escolher 0
        menu();             // Mostra o menu
        scanf("%d", &op);   // Lê o número digitado

        switch (op) {       // Verifica qual opção foi escolhida
            case 1:
                jogarPecaNormal();   // Joga a peça da frente normalmente
                break;
            case 2:
                pushHold();          // Reserva a peça atual
                break;
            case 3:
                popHold();           // Usa a peça reservada
                break;
            case 0:
                printf("Obrigado por jogar! Até a próxima!\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        printf("\n");       // Pula uma linha para ficar bonito
    } while (op != 0);      // Continua enquanto não for 0

    return 0;               // Termina o programa com sucesso
}