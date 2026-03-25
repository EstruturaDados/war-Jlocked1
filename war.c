// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
const int TAM_USER = 5;
const int TOTAL_MISSOES = 5;
const char *MISSOES[] = {
    "Destruir o exército vermelho",
    "Destruir o exército azul",
    "Destruir o exército verde",
    "Destruir o exército amarelo",
    "Conquistar 5 territorios"};

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct
{
    char name[30];
    char color[10];
    int troops;
    int dadoResultado;
} region;

// Estrutura do jogador para contar conquistas
typedef struct
{
    char cor[10];
    char missao[100];
    int territoriosConquistados;
} Jogador;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
void liberarMemoria(region *mapa, Jogador *jogadores);
int temNumero(char *str);
void atribuirMissao(Jogador *jogador, const char *missoes[], int totalMissoes);
int atacar(region *atacante, region *defensor);
int verificarMissao(const Jogador *jogador, region *territorios, int totalTerritorios, int exibirMensagem);
int inicializarTerritorios(region **territorios, Jogador **jogadores);
void exibirMenuPrincipal();
void exibirMapa(region *mapa, int totalTerritorios);
void exibirMissao(Jogador *jogador);
void faseDeAtaque(region *mapa, int totalTerritorios, Jogador *jogador);
void limparBufferEntrada();
int lerInteiro(const char *prompt, int *valor);
void exibirPlacarConquistas(const region *mapa, int totalTerritorios, const Jogador *jogadores, int totalJogadores);
void carregarTerritoriosPadrao(region *territorios);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
// 1. Configuração Inicial (Setup):
// - Define o locale para português.
// - Inicializa a semente para geração de números aleatórios com base no tempo atual.
// - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
// - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
// - Define a cor do jogador e sorteia sua missão secreta.
// 2. Laço Principal do Jogo (Game Loop):
// - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
// - A cada iteração, exibe o mapa, a missão e o menu de ações.
// - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
//   - Opção 1: Inicia a fase de ataque.
//   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
//   - Opção 0: Encerra o jogo.
// - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
// 3. Limpeza:
// - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

int main()
{
    // Gera números randomicos
    srand(time(NULL));
    printf("------------------------------\n");
    printf("Welcome to War Game!\n");
    printf("Carregando territórios padrão...\n");
    printf("------------------------------\n");

    // Inicialização do mapa do mundo e dos jogadores
    region *territorios = NULL;
    Jogador *jogadores = NULL;

    if (inicializarTerritorios(&territorios, &jogadores) != 0) // Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    {
        return 1;
    }
    carregarTerritoriosPadrao(territorios);

    int totalJogadores = 0;

    for(int i=0; i<TAM_USER; i++) {

        int corExistente = 0;
        for(int j=0; j<totalJogadores; j++) {
            if(strcmp(jogadores[j].cor, territorios[i].color) == 0) {
                corExistente = 1;
                break;
            }
        }

        if(!corExistente) {
            strcpy(jogadores[totalJogadores].cor, territorios[i].color);
            totalJogadores++;
        }
    }

    if (totalJogadores == 0)
    {
        printf("Nenhum jogador válido encontrado.\n");
        liberarMemoria(territorios, jogadores);
        return 1;
    }

    Jogador *jogadorAtual = &jogadores[0];
    strcpy(jogadorAtual->cor, territorios[0].color);
    jogadorAtual->territoriosConquistados = 0;
    atribuirMissao(jogadorAtual, MISSOES, TOTAL_MISSOES);
    printf("Jogador com cor %s recebeu a missão: %s\n",
           jogadorAtual->cor,
           jogadorAtual->missao);

    while (1)
    {
        exibirMapa(territorios, TAM_USER);
        exibirPlacarConquistas(territorios, TAM_USER, jogadores, totalJogadores);
        exibirMissao(jogadorAtual);
        exibirMenuPrincipal();

        int opcao = -1;
        if (!lerInteiro("Opção: ", &opcao))
        {
            printf("Opção inválida.\n");
            printf("Pressione Enter para continuar...");
            limparBufferEntrada();
            continue;
        }

        if (opcao == 0)
        {
            break;
        }

        if (opcao == 1)
        {
            faseDeAtaque(territorios, TAM_USER, jogadorAtual);
            if (verificarMissao(jogadorAtual, territorios, TAM_USER, 0))
            {
                printf("Parabéns! Jogador (%s) venceu automaticamente ao cumprir a missão: %s\n",
                       jogadorAtual->cor, jogadorAtual->missao);
                break;
            }
        }
        else if (opcao == 2)
        {
            if (verificarMissao(jogadorAtual, territorios, TAM_USER, 1))
            {
                printf("Jogo encerrado com vitória do jogador (%s).\n", jogadorAtual->cor);
                break;
            }
        }
        else
        {
            printf("Opção inválida.\n");
        }

        printf("Pressione Enter para continuar...");
        limparBufferEntrada();
    }

    liberarMemoria(territorios, jogadores); // Libera a memória alocada para os territórios e jogadores

    return 0;
}

// --- Implementação das Funções ---

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
int inicializarTerritorios(region **territorios, Jogador **jogadores)
{
    *territorios = (region *)calloc(TAM_USER, sizeof(region));
    *jogadores = (Jogador *)calloc(TAM_USER, sizeof(Jogador));
    if (*territorios == NULL || *jogadores == NULL)
    {
        free(*territorios);
        free(*jogadores);
        printf("Falha ao alocar memória\n");
        return 1; // Encerra o programa com código de erro
    }

    return 0;
}

void carregarTerritoriosPadrao(region *territorios)
{
    const char *nomes[] = {"America", "Africa", "Europa", "Asia", "Oceania"};
    const char *cores[] = {"vermelho", "amarelo", "azul", "verde", "preto"};
    const int tropas[] = {5, 5, 5, 5, 5};

    for (int i = 0; i < TAM_USER; i++)
    {
        strcpy(territorios[i].name, nomes[i]);
        strcpy(territorios[i].color, cores[i]);
        territorios[i].troops = tropas[i];
        territorios[i].dadoResultado = 0;
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(region *mapa, Jogador * jogadores)
{
    free(mapa);
    free(jogadores);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal()
{
    printf("\nEscolha uma ação:\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(region *mapa, int totalTerritorios)
{
    printf("\n================================\n\n");
    printf("  MAPA DO MUNDO - ESTADO ATUAL   ");
    printf("\n\n================================\n\n");

    for (int i = 0; i < totalTerritorios; i++)
    {
        printf("Territory %d: (Name: %s, Color: %s, Troops: %d)\n", i + 1,
               mapa[i].name,
               mapa[i].color,
               mapa[i].troops);
    }
}

void exibirPlacarConquistas(const region *mapa, int totalTerritorios, const Jogador *jogadores, int totalJogadores)
{
    printf("\nPlacar de territórios:\n");

    for (int i = 0; i < totalJogadores; i++)
    {
        int territoriosControlados = 0;

        for (int j = 0; j < totalTerritorios; j++)
        {
            if (strcmp(mapa[j].color, jogadores[i].cor) == 0)
            {
                territoriosControlados++;
            }
        }

        printf("- %s: controla %d território(s) | conquistas acumuladas: %d\n",
               jogadores[i].cor,
               territoriosControlados,
               jogadores[i].territoriosConquistados);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(Jogador *jogador)
{
    printf("\nJogador (%s) - Missão: %s\n", jogador->cor, jogador->missao);
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(region *mapa, int totalTerritorios, Jogador *jogador)
{
    int escolha1, escolha2;

    printf("\nLet the games begin!\n");
    printf(" FASE DE ATAQUE \n");
    printf("Escolha um territorio atacante (1 a %d, ou 0 para sair)\n", totalTerritorios);
    if (!lerInteiro(">> ", &escolha1))
    {
        printf("Entrada inválida.\n");
        return;
    }
    if (escolha1 == 0)
    {
        return;
    }
    escolha1--;

    printf("Escolha um territorio defensor (1 a %d, ou 0 para sair)\n", totalTerritorios);
    if (!lerInteiro(">> ", &escolha2))
    {
        printf("Entrada inválida.\n");
        return;
    }
    if (escolha2 == 0)
    {
        return;
    }
    escolha2--;

    if (escolha1 < 0 || escolha1 >= totalTerritorios || escolha2 < 0 || escolha2 >= totalTerritorios)
    {
        printf("Escolha um territorio válido (entre 1 e %d)\n", totalTerritorios);
        return;
    }

    if (escolha1 == escolha2)
    {
        printf("Escolha um territorio diferente do atacante\n");
        return;
    }

    if (strcmp(mapa[escolha1].color, mapa[escolha2].color) == 0)
    {
        printf("Não é permitido atacar território da mesma cor.\n");
        return;
    }

    int resultado = atacar(&mapa[escolha1], &mapa[escolha2]);

    if (resultado == 1)
    {
        jogador->territoriosConquistados++;
    }
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
int atacar(region *atacante, region *defensor)
{
    atacante->dadoResultado = rand() % 6 + 1;
    defensor->dadoResultado = rand() % 6 + 1;

    if (atacante->dadoResultado > defensor->dadoResultado)
    {
        printf("%s rolou o dado e saiu: %d\n\n", atacante->name, atacante->dadoResultado);
        printf("%s rolou o dado e saiu: %d\n\n", defensor->name, defensor->dadoResultado);
        defensor->troops--;
        printf("VITORIA DO ATAQUE! o defensor perdeu 1 tropa\n\n");
    }
    else if (atacante->dadoResultado < defensor->dadoResultado)
    {
        printf("%s rolou o dado e saiu: %d\n\n", defensor->name, defensor->dadoResultado);
        printf("%s rolou o dado e saiu: %d\n\n", atacante->name, atacante->dadoResultado);
        atacante->troops--;
        printf("VITORIA DO DEFENSOR! o atacante perdeu 1 tropa\n\n");
    }
    else
    {
        printf("Ambas as tropas recuaram, resultado de ambos os dados: %d\n\n", atacante->dadoResultado);
        printf("EMPATE! Ambos perderam 1 tropa!\n\n");
        atacante->troops--;
        defensor->troops--;
    }
    // Verifica quando um territorio é conquistado
    if (defensor->troops == 0)
    {
        printf("O defensor %s foi conquistado pelo atacante %s!\n\n", defensor->name, atacante->name);
        strcpy(defensor->color, atacante->color);
        defensor->troops = 1;
        return 1;
    }
    else if (atacante->troops == 0)
    {
        printf("O atacante %s foi conquistado pelo defensor %s!\n\n", atacante->name, defensor->name);
        strcpy(atacante->color, defensor->color);
        atacante->troops = 1;
        return -1;
    }

    return 0;
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
void atribuirMissao(Jogador *jogador, const char *missoes[], int totalMissoes)
{
    const char *coresAlvo[] = {"vermelho", "azul", "verde", "amarelo"};
    int indiceSorteado = 0;

    do
    {
        indiceSorteado = rand() % totalMissoes;
    } while (indiceSorteado < 4 && strcmp(jogador->cor, coresAlvo[indiceSorteado]) == 0);

    strcpy(jogador->missao, missoes[indiceSorteado]);
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarMissao(const Jogador *jogador, region *territorios, int totalTerritorios, int exibirMensagem) {
    const char *coresAlvo[] = {"vermelho", "azul", "verde", "amarelo"};
    int cumpriu = 0;

    for (int i = 0; i < TOTAL_MISSOES; i++) {
        if(strcmp(jogador->missao, MISSOES[i]) == 0) {
            // Lógica para verificar se a missão foi cumprida
            if (i < 4) { // Missões de destruir exércitos
                int tropasRestantes = 0;
                for (int j = 0; j < totalTerritorios; j++) {
                    if (strcmp(territorios[j].color, coresAlvo[i]) == 0) {
                        tropasRestantes += territorios[j].troops;
                    }
                }
                if (tropasRestantes == 0) {
                    cumpriu = 1;
                }
            } else { // Missão de conquistar territórios
                if (jogador->territoriosConquistados >= 5) {
                    cumpriu = 1;
                }
            }
            break;
        }
    }

    if (cumpriu)
    {
        if (exibirMensagem)
        {
            printf("Parabéns! Você cumpriu a missão: %s\n", jogador->missao);
        }
    }
    else
    {
        if (exibirMensagem)
        {
            printf("Missão ainda não concluída: %s\n", jogador->missao);
        }
    }

    return cumpriu;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Descarta os caracteres até encontrar um '\n' ou EOF
    }
}

int lerInteiro(const char *prompt, int *valor)
{
    char linha[64];
    char *fim = NULL;
    long numero = 0;

    printf("%s", prompt);
    if (fgets(linha, sizeof(linha), stdin) == NULL)
    {
        return 0;
    }

    numero = strtol(linha, &fim, 10);
    if (fim == linha)
    {
        return 0;
    }

    while (*fim != '\0')
    {
        if (*fim != ' ' && *fim != '\t' && *fim != '\n')
        {
            return 0;
        }
        fim++;
    }

    *valor = (int)numero;
    return 1;
}

// temNumero();
// Função para verificar se uma string literal tem números
int temNumero(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
        {
            return 1;
        }
    }
    return 0;
}
