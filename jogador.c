#include "gerenciador_header.h"
#include "gerenciador_func.h"

//=========================================================================================
/* AtualizarJogador: Jogador_t*, Fase_t -> void
  Função para atualizar a posição e estado do jogador durante o jogo.

Parâmetros:
  - jogador (Jogador_t*): Ponteiro para a estrutura do jogador que será atualizada.
  - mapa (Fase_t): Estrutura contendo o mapa da fase atual.

Descrição:
  A função gerencia a movimentação do jogador, incluindo física de pulo e gravidade,
  colisão com obstáculos no mapa e coleta de itens. Também atualiza o estado do jogador
  conforme ele avança na fase ou morre.
*/
//=========================================================================================
void AtualizarJogador(Jogador_t *jogador, Fase_t mapa){
  Vector2 oldPos = jogador->posicao;
  Vector2 auxPos;

  Pulo(jogador);

  int isGrounded = 0;
  if(jogador->pulando){
    jogador->gravidade.y = 1.8f;
  }
  
  if(jogador->velocidade.y > 0){
    jogador->gravidade.y = 0.5f;
  } else {
    jogador->gravidade.y = 0.5f;
  }

  jogador->velocidade.y += jogador->gravidade.y;
  jogador->velocidade.x = 5.0f;

  // Atualizar posição
  jogador->posicao.x += jogador->velocidade.x;
  switch(SideCollision(mapa, jogador->posicao)){
    case 1:
      jogador->posicao = ProcuraJogador(mapa, mapa.tamBloco);
      jogador->tentativas++;
      break;
    case 2:
      jogador->faseCompleta = 1;
      break;
    case 3:
      if(!(jogador->cd)){ 
        jogador->pts += 1;
        jogador->cd = 120;
      }
      break;
    default:
      break;
  }

  jogador->posicao.y += jogador->velocidade.y;

  for (int i = 0; i < mapa.columnSize; i++) {
    for (int j = 0; j < mapa.lineSize; j++) {
      if(mapa.mapa[j][i] == 'G'){
        auxPos = (Vector2){ i * mapa.tamBloco, j * mapa.tamBloco };
        if(CheckCollisionRecs(
        (Rectangle) { jogador->posicao.x, jogador->posicao.y, 60, 60}, 
        (Rectangle) { auxPos.x, auxPos.y, 60, 60 })
        ){
            jogador->posicao.y = oldPos.y;  // Reseta posição antiga;
            jogador->velocidade.y = 0;      // Para o movimento vertical;
            jogador->pulando = 0;           // Reset do pulo;
            isGrounded = 1;                 // Confirma que está no chão;
            break;
        }
      }
    }
  }

  if(!isGrounded){
    jogador->velocidade.y += jogador->gravidade.y;
  }
  if(jogador->cd > 0){
    jogador->cd--;
  }
}

//=========================================================================================
/* SideCollision: Fase_t, Vector2 -> int
  Função para detectar colisões laterais do jogador com o mapa.

Parâmetros:
  - mapa (Fase_t): Estrutura contendo o mapa da fase atual.
  - jogadorPos (Vector2): Posição atual do jogador.
Retorno:
  - int: Um valor inteiro que indica o resultado da colisão:
    - 1: O jogador morreu.
    - 2: O jogador completou a fase.
    - 3: O jogador coletou um item.
    - 0: Nenhuma colisão relevante.
Descrição:
  A função verifica se o jogador colidiu com obstáculos ou objetos especiais no mapa,
  determinando o efeito dessa colisão (morte, término da fase ou coleta de item).
*/
//=========================================================================================
int SideCollision(Fase_t mapa, Vector2 jogadorPos){
  Vector2 auxPos;
  
  for(int i = 0; i < mapa.columnSize; i++){
    for(int j = 0; j < mapa.lineSize; j++){
      if(mapa.mapa[j][i] == 'G' || mapa.mapa[j][i] == 'S'){
        auxPos = (Vector2){ i * mapa.tamBloco, j * mapa.tamBloco };
        if(CheckCollisionRecs(
        (Rectangle) { jogadorPos.x, jogadorPos.y, mapa.tamBloco, mapa.tamBloco}, 
        (Rectangle) { auxPos.x, auxPos.y, mapa.tamBloco, mapa.tamBloco })
        ){
          return 1; // Retorna que o jogador morreu;
        }
      }
      if(mapa.mapa[j][i] == 'F'){
        auxPos = (Vector2){ i * mapa.tamBloco, j * mapa.tamBloco };
        if(CheckCollisionRecs(
        (Rectangle) { jogadorPos.x, jogadorPos.y, mapa.tamBloco, mapa.tamBloco}, 
        (Rectangle) { auxPos.x, auxPos.y, mapa.tamBloco, mapa.tamBloco })
        ){
          return 2; // Retorna que terminou a fase;
        }
      }
      if(mapa.mapa[j][i] == '@'){
        auxPos = (Vector2){ i * mapa.tamBloco, j * mapa.tamBloco };
        if(CheckCollisionRecs(
        (Rectangle) { jogadorPos.x, jogadorPos.y, mapa.tamBloco, mapa.tamBloco}, 
        (Rectangle) { auxPos.x, auxPos.y, mapa.tamBloco, mapa.tamBloco })
        ){
          return 3; // Retorna coletou um item na fase;
        }
      }
    }
  }
  return 0;
}

//=========================================================================================
/* Pulo: Jogador_t* -> void
  Função para gerenciar o pulo do jogador.

Parâmetros:
  - jogador (Jogador_t*): Ponteiro para a estrutura do jogador.
Descrição:
  A função verifica se a tecla de pulo (espaço) foi pressionada e se o jogador não
  está pulando atualmente, aplicando a física de pulo. Também atualiza a rotação
  do jogador durante o pulo.
*/
//=========================================================================================
void Pulo(Jogador_t *jogador){
  if(IsKeyDown(KEY_SPACE) && !jogador->pulando){
    jogador->pulando = 1;
    jogador->velocidade.y = -22.0f;
  }
  if(jogador->pulando){
    jogador->rotacao += 10;
  } else {
    jogador->rotacao = 0;
  }
}

//=========================================================================================
/* InicializaJogador: Jogador_t*, int -> void
  Função para inicializar os dados do jogador no início de uma fase.

Parâmetros:
  - jogador (Jogador_t*): Ponteiro para a estrutura do jogador.
  - fase (int): ID da fase atual.
Objetivo:
  A função reinicia os dados do jogador, como a flag de pulo, o cooldown (coleta de item), e o número
  de tentativas, preparando para iniciar uma nova fase.
*/
//=========================================================================================
void InicializaJogador(Jogador_t *jogador, int fase){
  jogador->faseCompleta = 0;
  jogador->idFase = fase;
  jogador->pulando = 0;
  jogador->cd = 0;
  jogador->tentativas = 0;
}