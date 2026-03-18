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
int TAM_USER;

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct
{
    char name[30];
    char color[10];
    int troops;
    int dadoResultado;
} region;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
void liberarMemoria(region *mapa);
int temNumero(char *str);

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
void atacar(region *atacante, region *defensor)
{
    srand(time(NULL));

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
        strcpy(atacante->color, defensor->color);
    }
    else if (atacante->troops == 0)
    {
        printf("O atacante %s foi conquistado pelo defensor %s!\n\n", atacante->name, defensor->name);
        strcpy(defensor->color, atacante->color);
    }
}

int main()
{
    printf("------------------------------\n");
    printf("Welcome to War Game!\n");
    printf("Please register the territories:\n");
    printf("------------------------------\n");

    // Coleção de arrays com estruturas heterogeneas e tamanho definido
    printf("Digite o total de territorios: ");
    scanf("%d", &TAM_USER);
    region *territorios = (region *)calloc(TAM_USER, sizeof(region *));
    if (territorios == NULL)
    {
        printf("Falha ao alocar memória\n");
        return 1; // Encerra o programa com código de erro
    }
    getchar(); // Limpa o buffer do teclado para a próxima entrada

    // Loop para registrar os territorios
    for (int i = 0; i < TAM_USER; i++)
    {
        printf("\nRegion %d\n", i + 1);

        do
        {
            printf("Type Name: ");
            fgets(territorios[i].name, 30, stdin);
            territorios[i].name[strcspn(territorios[i].name, "\n")] = '\0'; // Remove o '\n' do final da string

            if (temNumero(territorios[i].name))
            {
                printf("O nome do território não pode conter números. Por favor, digite novamente.\n");
            }
        } while (temNumero(territorios[i].name)); // Verifica se o nome do território contém números, se sim, solicita novamente

        do
        {
            printf("Type Color: ");
            fgets(territorios[i].color, 10, stdin);
            territorios[i].color[strcspn(territorios[i].color, "\n")] = '\0'; // Remove o '\n' do final da string
            
            if(temNumero(territorios[i].color))
            {
                printf("A cor do território não pode conter números. Por favor, digite novamente.\n");
            }
        } while (temNumero(territorios[i].color)); // Verifica se a cor do território contém números, se sim, solicita novamente

        printf("Type Number of troops: ");
        scanf("%d", &territorios[i].troops);

        getchar(); // Limpa o buffer do teclado para a próxima entrada
    }

    while (1)
    {
        printf("\n================================\n\n");
        printf("  MAPA DO MUNDO - ESTADO ATUAL   ");
        printf("\n\n================================\n\n");

        // Loop para exibir os territorios registrados
        for (int i = 0; i < TAM_USER; i++)
        {
            printf("Territory %d: (Name: %s, Color: %s, Troops: %d)\n", i + 1,
                   territorios[i].name,
                   territorios[i].color,
                   territorios[i].troops);
        }

        printf("\nLet the games begin!\n");
        printf(" FASE DE ATAQUE \n");
        printf("Escolha um territorio atacante (1 a %d, ou 0 para sair)\n", TAM_USER);

        int escolha1, escolha2;

        scanf("%d", &escolha1);
        if (escolha1 == 0)
            break;
        escolha1 = escolha1 - 1;

        printf("Escolha um territorio defensor (1 a %d, ou 0 para sair)\n", TAM_USER);
        scanf("%d", &escolha2);
        if (escolha2 == 0)
            break;
        escolha2 = escolha2 - 1;

        // Range das escolhas e verificação de territórios iguais
        if (escolha1 == escolha2 || escolha1 < 0 || escolha1 >= TAM_USER || escolha2 < 0 || escolha2 >= TAM_USER)
        {
            if (escolha1 == escolha2 || territorios[escolha1].color == territorios[escolha2].color)
            {
                printf("Escolha um territorio diferente do atacante\n");
            }
            else
            {
                printf("Escolha um territorio válido (entre 1 e %d)\n", TAM_USER);
            }
            continue; // Pula para o próximo loop
        }

        atacar(&territorios[escolha1], &territorios[escolha2]);

        getchar(); // Limpa o buffer do teclado para a próxima entrada
        printf("Pressione Enter para continuar...");
        getchar();
    }

    liberarMemoria(territorios); // Libera a memória alocada para os territórios

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(region *mapa)
{
    free(mapa);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

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