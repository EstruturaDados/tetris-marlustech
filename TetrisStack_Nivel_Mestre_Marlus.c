/* ============================================================= */
/*  TETRIS STACK – NÍVEL MESTRE                                 */
/* ============================================================= */

#include <stdio.h>      // Biblioteca para usar printf, scanf, etc. (entrada/saída no terminal)
#include <stdlib.h>     // Biblioteca para usar malloc, free, exit, rand, srand
#include <time.h>       // Biblioteca para usar time() → faz o jogo ter peças diferentes a cada vez que roda

/* ------------------- DEFINIÇÃO DO TIPO "PEÇA" ---------------- */
// Crio um "molde" chamado Peca que representa uma peça do Tetris
typedef struct {
    char nome;   // Guarda a letra da peça: 'I', 'O', 'T', 'L', 'J', 'S' ou 'Z'
    int  id;     // Número único que cada peça recebe quando é criada (ex: 0, 1, 2...)
} Peca;          // Agora posso criar variáveis do tipo Peca em qualquer lugar

/* ------------------- CONFIGURAÇÕES DA FILA (Next Queue) ------ */
#define TAM_FILA 5                    // A fila sempre tem exatamente 5 posições
Peca fila[TAM_FILA];                  // Array que guarda as próximas 5 peças
int frente = 0;                       // Índice da peça que vai cair AGORA (primeira da fila)
int tras   = 0;                       // Índice onde será colocada a próxima peça nova
int qtdFila = 0;                      // Quantas peças estão na fila no momento (começa com 0)

/* ------------------- CONFIGURAÇÕES DA PILHA (Hold) ---------- */
#define TAM_PILHA 3                   // A reserva (Hold) aceita no máximo 3 peças
Peca pilha[TAM_PILHA];                // Array que guarda as peças reservadas
int topo = -1;                        // -1 significa pilha vazia. Quando tem peça, vira 0, 1 ou 2
int qtdPilha = 0;                     // Quantas peças estão reservadas agora

/* ------------------- HISTÓRICO PARA O "DESFAZER" (UNDO) ----- */
// Estrutura que guarda um "backup" completo do jogo em um momento
typedef struct {
    Peca fila_salva[TAM_FILA];        // Cópia completa da fila
    int frente_salvo, tras_salvo, qtdFila_salvo;  // Cópia dos índices e quantidade
    Peca pilha_salva[TAM_PILHA];      // Cópia completa da pilha
    int topo_salvo, qtdPilha_salvo;   // Cópia do topo e quantidade da pilha
} Estado;                             // Um "Estado" é uma foto do jogo inteiro

Estado historico[100];                // Array que guarda até 100 "fotos" do jogo
int qtdHistorico = 0;                 // Quantas fotos já foram tiradas (começa com 0)

/* ------------------- GERADOR DE PEÇAS ----------------------- */
int proximoId = 0;                    // Contador que dá ID único para cada peça
const char tiposPeca[7] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'}; // As .length7 peças do Tetris

/* ============================================================= */
/*  Função: gerarPeca()                                          */
/*  Cria uma peça nova com letra aleatória e ID único           */
/* ============================================================= */
Peca gerarPeca() {
    Peca p;                           // Crio uma peça temporária
    p.nome = tiposPeca[rand() % 7];   // rand() % 7 dá número de 0 a 6 → escolhe uma letra aleatória
    p.id   = proximoId++;             // Uso o próximo ID disponível e já aumento o contador
    return p;                         // Devolvo a peça pronta
}

/* ============================================================= */
/*  Função: salvarEstado()                                       */
/*  Tira uma "foto" do jogo inteiro e guarda no histórico      */
/* ============================================================= */
void salvarEstado() {
    if (qtdHistorico >= 100) return;  // Proteção (nunca vai acontecer)

    // Salva a fila inteira
    for (int i = 0; i < TAM_FILA; i++) 
        historico[qtdHistorico].fila_salva[i] = fila[i];
    
    // Salva os índices e quantidade da fila
    historico[qtdHistorico].frente_salvo = frente;
    historico[qtdHistorico].tras_salvo   = tras;
    historico[qtdHistorico].qtdFila_salvo = qtdFila;

    // Salva a pilha inteira
    for (int i = 0; i < TAM_PILHA; i++) 
        historico[qtdHistorico].pilha_salva[i] = pilha[i];
    
    // Salva os índices e quantidade da pilha
    historico[qtdHistorico].topo_salvo    = topo;
    historico[qtdHistorico].qtdPilha_salvo = qtdPilha;

    qtdHistorico++;                   // Aumenta o contador de fotos tiradas
}

/* ============================================================= */
/*  Função: desfazer()                                           */
/*  Volta para o estado anterior (UNDO)                          */
/* ============================================================= */
void desfazer() {
    if (qtdHistorico == 0) {          // Se não tem nenhuma foto para voltar
        printf("  Nada para desfazer!\n");
        return;
    }
    qtdHistorico--;                   // Volta uma foto no histórico

    // Restaura a fila
    for (int i = 0; i < TAM_FILA; i++) 
        fila[i] = historico[qtdHistorico].fila_salva[i];
    frente  = historico[qtdHistorico].frente_salvo;
    tras    = historico[qtdHistorico].tras_salvo;
    qtdFila = historico[qtdHistorico].qtdFila_salvo;

    // Restaura a pilha
    for (int i = 0; i < TAM_PILHA; i++) 
        pilha[i] = historico[qtdHistorico].pilha_salva[i];
    topo    = historico[qtdHistorico].topo_salvo;
    qtdPilha = historico[qtdHistorico].qtdPilha_salvo;

    printf("  Última ação desfeita!\n");
}

/* ============================================================= */
/*  Função: enqueue()                                            */
/*  Adiciona uma peça nova no final da fila (circular)          */
/* ============================================================= */
void enqueue() {
    if (qtdFila < TAM_FILA) {         // Só adiciona se tiver espaço
        Peca nova = gerarPeca();      // Cria peça nova
        fila[tras] = nova;            // Coloca na posição "tras"
        tras = (tras + 1) % TAM_FILA; // Avança circularmente (ex: 4+1 → 0)
        qtdFila++;                    // Aumenta quantidade
    }
}

/* ============================================================= */
/*  Função: dequeue()                                            */
/*  Remove e devolve a peça da frente da fila                   */
/* ============================================================= */
Peca dequeue() {
    Peca p = fila[frente];            // Pega a peça da frente
    frente = (frente + 1) % TAM_FILA; // Avança o ponteiro da frente
    qtdFila--;                        // Diminui quantidade
    return p;                         // Devolve a peça removida
}

/* ============================================================= */
/*  Opção 1 – Jogar peça normal                                  */
/* ============================================================= */
void jogarPeca() {
    salvarEstado();                   // Guarda o estado antes de jogar
    if (qtdFila == 0) { printf("  Fila vazia!\n"); return; }
    Peca jogada = dequeue();          // Remove da frente
    printf("  Jogou peça [%c %d]\n", jogada.nome, jogada.id);
    enqueue();                        // Gera nova peça → fila volta a ter 5
}

/* ============================================================= */
/*  Opção 2 – Reservar peça (Hold)                               */
/* ============================================================= */
void reservarPeca() {
    salvarEstado();
    if (qtdFila == 0) { printf("  Fila vazia!\n"); return; }
    if (qtdPilha >= TAM_PILHA) { printf("  Reserva cheia!\n"); return; }

    Peca p = dequeue();               // Tira da frente da fila
    topo++; pilha[topo] = p; qtdPilha++;  // Empilha
    printf("  Reservou [%c %d]\n", p.nome, p.id);
    enqueue();                        // Repõe na fila
}

/* ============================================================= */
/*  Opção 3 – Usar peça reservada                                */
/* ============================================================= */
void usarReservada() {
    salvarEstado();
    if (qtdPilha == 0) { printf("  Reserva vazia!\n"); return; }

    Peca usada = pilha[topo]; topo--; qtdPilha--;  // Desempilha
    printf("  Usou reservada [%c %d]\n", usada.nome, usada.id);

    Peca jogada = dequeue();          // Joga a peça que estava na frente
    printf("  Jogou da fila [%c %d]\n", jogada.nome, jogada.id);
    enqueue();
}

/* ============================================================= */
/*  Opção 4 – Trocar topo da pilha com frente da fila           */
/* ============================================================= */
void trocarTopoComFrente() {
    salvarEstado();
    if (qtdFila == 0 || qtdPilha == 0) {
        printf("  Não é possível trocar: uma das estruturas está vazia!\n");
        return;
    }
    Peca temp = pilha[topo];          // Guarda temporariamente a peça do topo
    pilha[topo] = fila[frente];       // Topo recebe peça da frente
    fila[frente] = temp;              // Frente recebe peça do topo
    printf("  Trocou topo da pilha com frente da fila!\n");
}

/* ============================================================= */
/*  Opção 6 – Inverter fila com pilha (SWAP TOTAL)              */
/* ============================================================= */
void inverterFilaComPilha() {
    salvarEstado();

    // Variáveis temporárias para guardar tudo
    Peca tempFila[TAM_FILA];
    int tempFrente = frente, tempTras = tras, tempQtdF = qtdFila;
    Peca tempPilha[TAM_PILHA];
    int tempTopo = topo, tempQtdP = qtdPilha;

    // Copia fila para temp
    for (int i = 0; i < TAM_FILA; i++) tempFila[i] = fila[i];
    // Copia pilha para fila (as peças da pilha viram as primeiras da fila)
    for (int i = 0; i < qtdPilha && i < TAM_FILA; i++) fila[i] = pilha[i];
    // Completa o resto da fila com as antigas peças
    for (int i = qtdPilha; i < TAM_FILA; i++) fila[i] = tempFila[i - qtdPilha];

    frente = 0; tras = qtdPilha; qtdFila = qtdPilha;  // Atualiza índices

    // Copia antigas peças da fila para a pilha
    for (int i = 0; i < tempQtdF && i < TAM_PILHA; i++) pilha[i] = tempFila[i];
    topo = (tempQtdF < TAM_PILHA ? tempQtdF - 1 : TAM_PILHA - 1);
    qtdPilha = (tempQtdF < TAM_PILHA ? tempQtdF : TAM_PILHA);

    printf("  Inverteu fila com pilha! (SWAP TOTAL)\n");
}

/* ============================================================= */
/*  Funções de exibição                                          */
/* ============================================================= */
void exibirFila() {
    printf("Fila     : ");
    if (qtdFila == 0) printf("<vazia>\n");
    else {
        int i = frente;
        for (int c = 0; c < qtdFila; c++) {
            printf("[%c %d] ", fila[i].nome, fila[i].id);
            i = (i + 1) % TAM_FILA;
        }
        printf("\n");
    }
}

void exibirPilha() {
    printf("Reserva  : ");
    if (qtdPilha == 0) printf("<vazia>\n");
    else {
        for (int i = topo; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
        printf(" ← topo\n");
    }
}

void exibirMenu() {
    printf("╔══════════════════════════════════════════╗\n");
    exibirFila();
    exibirPilha();
    printf("╠──────────────────────────────────────────╣\n");
    printf("║ 1 - Jogar peça atual                     ║\n");
    printf("║ 2 - Reservar peça (Hold)                 ║\n");
    printf("║ 3 - Usar peça reservada                  ║\n");
    printf("║ 4 - Trocar topo com frente               ║\n");
    printf("║ 5 - Desfazer última ação                 ║\n");
    printf("║ 6 - Inverter fila com pilha (SWAP)       ║\n");
    printf("║ 0 - Sair                                 ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("→ ");
}

/* ============================================================= */
/*  Inicialização do jogo                                        */
/* ============================================================= */
void inicializar() {
    printf("=== TETRIS STACK – NÍVEL MESTRE ===\n");
    printf("Gerando 5 peças iniciais...\n");
    while (qtdFila < TAM_FILA) enqueue();  // Preenche a fila
    salvarEstado();                        // Salva estado inicial
    printf("\n");
}

/* ============================================================= */
/*  Função principal – onde o programa começa                   */
/* ============================================================= */
int main() {
    srand(time(NULL));      // Faz o rand() gerar números diferentes a cada execução
    inicializar();          // Prepara o jogo

    int op;                 // Variável que guarda a opção do jogador
    do {                    // Repete até digitar 0
        exibirMenu();       // Mostra o estado atual + opções
        scanf("%d", &op);   // Lê a escolha

        switch (op) {       // Executa a função certa
            case 1: jogarPeca();          break;
            case 2: reservarPeca();       break;
            case 3: usarReservada();      break;
            case 4: trocarTopoComFrente(); break;
            case 5: desfazer();           break;
            case 6: inverterFilaComPilha(); break;
            case 0: printf("Obrigado por jogar, Mestre do Tetris!\n"); break;
            default: printf("Opção inválida!\n");
        }
        printf("\n");
    } while (op != 0);      // Sai do loop quando digitar 0

    return 0;               // Termina o programa com sucesso
}