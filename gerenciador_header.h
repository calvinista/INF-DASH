#ifndef GERENCIADOR_HEADER_H
#define GERENCIADOR_HEADER_H
//===================================================
// Bibliotecas utilizadas
//===================================================
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//===================================================
// Propriedades da Janela
//===================================================
#define SCREEN_WIDTH 1200 // Largura da tela
#define SCREEN_HEIGHT 600 // Altura da tela
#define TITLE "Inf Dash" // Título da janela
#define FPS 60 // Frames por segundo

//===================================================
// Defines do Jogo
//===================================================
#define STRING_MAX_SIZE 64 // Quantidade de caracteres que podem estar nas opções do menu
#define OPCOES_MENU 3 // Quantidade de opções no menu inicial
#define LINHAS 10 // Quantidade de linhas na matriz do mapa
#define COLUNAS 300 // Quantidade de colunas na matriz do mapa
#define NOME_TAM_MAX 15 // Tamanho limite do nome do jogador.

//===================================================
// Definição de Apelido
//===================================================
typedef struct JOGADOR Jogador_t;
typedef struct BLOCO Bloco_t;
typedef struct CENA Cena_t;
typedef struct TELA Tela_t;
typedef struct FASE Fase_t;
typedef struct LEADERBOARD Scores_t;
typedef struct NOME Nome_t;
typedef struct RECORD Record;
typedef struct LEVELDATA LevelData;
typedef enum GERENCIADOR_CENA Gerenciador_e;

//===================================================
// Estruturas Definidas
//===================================================
/* RECORD
- Armazena informações de recordes de jogadores para fases específicas.

Campos:
  - level_id (int): Identifica a fase à qual o recorde pertence.
  - name (char*): Nome do jogador que alcançou o recorde.
  - tries (int): Número de tentativas necessárias para alcançar o recorde.
//=========================================================================================
Definição:
*/
struct RECORD{
  int level_id;
  char name[NOME_TAM_MAX];
  int tries;
};

/* LEVELDATA
- Armazena os dados de recordes de uma fase específica.

Campos:
  - level_id (int): Identifica a fase.
  - top5 (Record[5]): Lista dos 5 melhores recordes da fase.
=========================================================================================
Definição:
*/
struct LEVELDATA{
  int level_id;
  Record top5[5];
};

/* LEADERBOARD
- Gerencia o placar e a interface de usuário para exibição dos recordes.

Campos:
  - retorno (Rectangle): Área do botão de retorno na tela de placar.
  - reset (Rectangle): Área do botão de reset na tela de placar.
  - fases (LevelData[5]): Dados dos recordes das 5 fases disponíveis.
  - desenhaScore (int): Flag para indicar se o placar deve ser desenhado.
  - pagina (int): Página atual do placar sendo exibida.
=========================================================================================
Definição:
*/
struct LEADERBOARD{
  Rectangle retorno;
  Rectangle reset;
  LevelData fases[5];
  int desenhaScore;
  int pagina;
};

/* NOME
- Gerencia a entrada e exibição do nome do jogador.

Campos:
  - nome (char[NOME_TAM_MAX]): Nome do jogador sendo inserido.
  - contaLetras (int): Número de letras já inseridas no nome.
  - contaFrame (int): Contador de frames para animação da inserção de texto.
  - caixaTexto (Rectangle): Área da caixa de texto na interface.
  - mouseSobreCaixa (int): Flag para indicar se o mouse está sobre a caixa de texto.
=========================================================================================
Definição:
*/
struct NOME{
  char nome[NOME_TAM_MAX];
  int contaLetras;
  int contaFrame;
  Rectangle caixaTexto;
  int mouseSobreCaixa;
};

/* CENA
- Gerencia a exibição das cenas do jogo, como o menu principal.

Campos:
  - texTitle (Texture2D): Textura do título do jogo.
  - background (Texture2D): Textura do fundo da cena.
  - string (char[OPCOES_MENU][STRING_MAX_SIZE]): Textos das opções de menu.
  - fontSize (int): Tamanho da fonte para os textos do menu.
  - encerrarJogo (int): Flag para encerrar o jogo.
  - menuLock (int): Flag para bloquear a navegação no menu.
  - flagNome (int): Flag para indicar se a entrada de nome está ativa.
  - hover (int[OPCOES_MENU]): Indica quais botões do menu estão sendo "sobrepostos" pelo mouse.
  - botoes (Rectangle[OPCOES_MENU]): Áreas dos botões do menu.
  - textColor (Color[OPCOES_MENU]): Cores dos textos dos botões do menu.
  - scores (Scores_t): Dados do placar (LeaderBoards).
  - nome (Nome_t): Dados referente a Cena de input de nome.
=========================================================================================
Definição:
*/
struct CENA{
  Texture2D texTitle;
  Texture2D background;
  char string[OPCOES_MENU][STRING_MAX_SIZE];
  int fontSize;
  int encerrarJogo;
  int menuLock;
  int flagNome;
  int hover[OPCOES_MENU];
  Rectangle botoes[OPCOES_MENU];
  Color textColor[OPCOES_MENU];
  Scores_t scores;
  Nome_t nome;
};

/* FASE
- Representa uma fase do jogo, incluindo o layout do mapa.

Campos:
  - mapa (char[LINHAS][COLUNAS]): Matriz que representa o layout do mapa da fase.
  - lineSize (int): Número de linhas no mapa.
  - columnSize (int): Número de colunas no mapa.
  - tamBloco (int): Tamanho de cada bloco no mapa.
  - indice (int): Índice da fase.
=========================================================================================
Definição:
*/
struct FASE{
  char mapa[LINHAS][COLUNAS];
  int lineSize;
  int columnSize;
  int tamBloco;
  int indice;
};

/* TELA
- Armazena as dimensões da tela do jogo.

Campos:
  - largura (int): Largura da tela.
  - altura (int): Altura da tela.
=========================================================================================
Definição:
*/
struct TELA{
  int largura;
  int altura;
};

/* JOGADOR
- Gerencia os dados do jogador, incluindo física e estado no jogo.

Campos:
  - tex (Texture2D): Textura do jogador.
  - velocidade (Vector2): Vetor de velocidade do jogador.
  - gravidade (Vector2): Vetor de gravidade que afeta o jogador.
  - posicao (Vector2): Posição do jogador na tela.
  - tamanho (Vector2): Tamanho do jogador.
  - pulando (int): Flag para indicar se o jogador está pulando.
  - rotacao (float): Ângulo de rotação do jogador.
  - tentativas (int): Número de tentativas realizadas pelo jogador.
  - pts (int): Pontos acumulados pelo jogador.
  - cd (int): Cooldown (tempo de espera) para ações.
  - faseCompleta (int): Flag para indicar se a fase foi completada.
  - idFase (int): ID da fase atual.
=========================================================================================
Definição:
*/
struct JOGADOR{
  Texture2D tex;
  Vector2 velocidade;
  Vector2 gravidade;
  Vector2 posicao;
  Vector2 tamanho;
  int pulando;
  float rotacao;
  int tentativas;
  int pts;
  int cd;
  int faseCompleta;
  int idFase;
};

/* GERENCIADOR_CENA
- Enumera as diferentes cenas/estados do jogo.

Valores:
  - MENU (1): Menu principal.
  - SELECAO (2): Seleção de fases.
  - JOGO (3): Fase do jogo em andamento.
  - FIM (4): Tela de fim de jogo.
=========================================================================================
Definição:
*/
enum GERENCIADOR_CENA{
  MENU = 1,
  SELECAO = 2,
  JOGO = 3,
  FIM = 4
};


#endif
