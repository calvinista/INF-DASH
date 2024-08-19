#include "gerenciador_header.h"
#include "gerenciador_func.h"

//=========================================================================================
/* DesenhaMenu: Cena_t, int -> void
  Função para desenhar o menu na tela.

Parâmetros:
  - cena (Cena_t): Estrutura contendo os dados da cena, como texturas e botões.
  - n (int): Número de botões a serem desenhados.
Objetivo:
  Desenha o background, o título e os botões do menu na tela.
*/
//=========================================================================================
void DesenhaMenu(Cena_t cena, int n){
  DrawTexture(cena.background, 0, 0, WHITE);
  DrawTexture(cena.texTitle, SCREEN_WIDTH/2 - cena.texTitle.width / 2, SCREEN_HEIGHT * 0.01f, WHITE);

  for(int i = 0; i < n; i++){
    DrawRectangleRec(cena.botoes[i], (Color){ 0, 0, 0, 0 });
    DrawText(cena.string[i], cena.botoes[i].x, cena.botoes[i].y, cena.fontSize, cena.textColor[i]);
  }
}

//=========================================================================================
/* DesenhaFase: Fase_t, int -> void
  Função para desenhar a fase do jogo na tela.

Parâmetros:
  - fase (Fase_t): Estrutura contendo os dados da fase, como mapa e tamanho dos blocos.
  - cooldown (int): Flag para indicar se o cooldown está ativo.
Objetivo:
  Desenha os blocos da fase, incluindo obstáculos, pontos de início e fim, e moedas (dependendo do cooldown).
*/
//=========================================================================================
void DesenhaFase(Fase_t fase, int cooldown){
  for(int i = 0; i < fase.lineSize; i++){
    for(int j = 0; j < fase.columnSize; j++){
      Rectangle rect = { j * fase.tamBloco, i * fase.tamBloco, fase.tamBloco, fase.tamBloco };
      if(fase.mapa[i][j] == 'G'){
        DrawRectangleRec(rect, BLUE);
        DrawRectangleLinesEx(rect, 2, DARKBLUE);
      }
      if(fase.mapa[i][j] == 'S'){
        DrawRectangleRec(rect, MAROON);
        DrawRectangleLinesEx(rect, 2, RED);
      }
      if(fase.mapa[i][j] == 'F'){
        DrawRectangleRec(rect, GREEN);
      }
      if(!cooldown){
        if(fase.mapa[i][j] == '@'){
          DrawRectangleRec(rect, GOLD);
          DrawRectangleLinesEx(rect, 2, YELLOW);
        }
      }
    }
  }
}

//=========================================================================================
/* DesenhaTelaInput: Nome_t, Tela_t -> void
  Função para desenhar a tela de entrada de nome na tela.

Parâmetros:
  - dados (Nome_t): Estrutura contendo os dados de entrada do nome.
  - param (Tela_t): Estrutura contendo os parâmetros da tela, como largura e altura.
Objetivo:
  Desenha as instruções para o usuário, a caixa de texto para inserção do nome, e mensagens de confirmação ou aviso.
*/
//=========================================================================================
void DesenhaTelaInput(Nome_t dados, Tela_t param){
  const char *congrats = "Você entrou no Top 5 da fase!";
  const char *info = "Coloque o mouse sobre a barra de texto";
  const char *confirmation = "Pressione ENTER para enviar seu recorde!";
  const char *warning = "Pressione BACKSPACE para deletar letras...";

  DrawText(congrats, param.largura / 2 - MeasureText(congrats, 20)/2, param.altura * 0.2f, 20, PURPLE);
  DrawText(info, param.largura / 2 - MeasureText(info, 20)/2, param.altura * 0.2f + 20, 20, GRAY);
  DrawRectangleRec(dados.caixaTexto, LIGHTGRAY);
  DrawText(
    TextFormat("Quantidade de Caracteres: %i/%i", dados.contaLetras, NOME_TAM_MAX), 
    param.largura / 2 - MeasureText(TextFormat("Quantidade de Caracteres: %i/%i", dados.contaLetras, NOME_TAM_MAX), 20)/2, 
    dados.caixaTexto.y + (dados.caixaTexto.height * 2), 
    20,
    DARKGRAY
  );

  if(dados.mouseSobreCaixa){
      DrawRectangleLines(
        (int)dados.caixaTexto.x,
        (int)dados.caixaTexto.y,
        (int)dados.caixaTexto.width,
        (int)dados.caixaTexto.height,
        RED
      );
  } else {
      DrawRectangleLines(
        (int)dados.caixaTexto.x,
        (int)dados.caixaTexto.y,
        (int)dados.caixaTexto.width,
        (int)dados.caixaTexto.height,
        DARKGRAY
      );
  }

  if(dados.contaLetras > 0){
    DrawText(dados.nome, (int)dados.caixaTexto.x + 5, (int)dados.caixaTexto.y + 8, 40, MAROON);
    DrawText(confirmation, param.largura/2 - MeasureText(confirmation, 20)/2, param.altura * 0.8f, 20, GRAY);
  }

  if(dados.mouseSobreCaixa){
    if(dados.contaLetras < NOME_TAM_MAX){
      if(((dados.contaFrame / 20) % 2) == 0){
        DrawText("_", (int)dados.caixaTexto.x + 8 + MeasureText(dados.nome, 40), (int)dados.caixaTexto.y + 12, 40, MAROON);
      }
    } else {
      DrawText(warning, param.largura/2 - MeasureText(warning, 20)/2, param.altura - 20, 20, GRAY);
    }
  }
}

//=========================================================================================
/* DesenhaScores: Scores_t, int -> void
  Função para desenhar os scores na tela.

Parâmetros:
  - leaderboards (Scores_t): Estrutura contendo os dados do leaderboard.
  - i (int): Índice do nível que está sendo exibido.
Objetivo:
  Desenha a lista de melhores scores para o nível selecionado, junto com os botões de retorno e reset.
*/
//=========================================================================================
void DesenhaScores(Scores_t leaderboards, int i){
  if(leaderboards.desenhaScore){
    int y = SCREEN_HEIGHT / 4 + 80; // Começa um pouco acima do centro para alinhar melhor
    int fontSize = 40;

    DrawRectangleRec((Rectangle){ 0, y, SCREEN_WIDTH, SCREEN_HEIGHT - y}, DARKBLUE);

    // Desenha cada nível e suas tentativas
    char levelText[50];
    snprintf(levelText, sizeof(levelText), "Level %d", leaderboards.fases[i].level_id);
    int textWidth = MeasureText(levelText, fontSize);
    DrawText(levelText, SCREEN_WIDTH / 2 - textWidth / 2, y, fontSize, BLACK);
    y += 100;

    for(int j = 0; j < 5; j++){
      char scoreText[100];
      snprintf(scoreText, sizeof(scoreText), "%s -> Tentativas: %d", 
        leaderboards.fases[i].top5[j].name, 
        leaderboards.fases[i].top5[j].tries
      );
      int scoreTextWidth = MeasureText(scoreText, fontSize);
      DrawText(scoreText, SCREEN_WIDTH / 2 - scoreTextWidth / 2, y, fontSize, LIME);
      y += 50;
    }

    
    DrawRectangleRec(leaderboards.retorno, RED);
    DrawRectangleLinesEx(leaderboards.retorno, 1, MAROON);
    DrawText(TextFormat("Voltar"), leaderboards.retorno.x+2, leaderboards.retorno.y, 30, BLACK);

    DrawRectangleRec(leaderboards.reset, RED);
    DrawRectangleLinesEx(leaderboards.reset, 1, MAROON);
    DrawText(TextFormat("Reiniciar"), leaderboards.reset.x+2, leaderboards.reset.y, 30, BLACK);
  }
}

//=========================================================================================
/* AnimationMenu: Cena_t*, Vector2 -> void
  Função para animar os elementos do menu.

Parâmetros:
  - cena (Cena_t*): Ponteiro para a estrutura contendo os dados da cena.
  - mousePos (Vector2): Posição atual do mouse.
Objetivo:
  Atualiza a cor dos textos e o estado de hover dos botões com base na posição do mouse.
*/
//=========================================================================================
void AnimationMenu(Cena_t *cena, Vector2 mousePos){
  // Controles do Menu Principal
  if(!cena->menuLock){
    for(int i = 0; i < OPCOES_MENU; i++){
      if(CheckCollisionPointRec(mousePos, cena->botoes[i])){
        cena->hover[i] = 1;
      } else {
        cena->hover[i] = 0;
      }

      if(cena->hover[i]){
        cena->textColor[i] = GREEN;
      } else {
        cena->textColor[i] = YELLOW;
      }
    }
  }

  // Controles na tela de Leaderboard (Sub-menu)
  if(cena->scores.desenhaScore){
    // Retorna o menu principal, removendo o lock.
    if(CheckCollisionPointRec(mousePos, cena->scores.retorno)){
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        cena->menuLock = 0;
        cena->scores.desenhaScore = 0;
        cena->hover[1] = 0; // Zera a flag da opção de Leaderboard, IMPORTANTE -> Se não houver, fica em loop infinito.
      }
    }
    // Reinicia os dados no arquivo de Leaderboard.
    if(CheckCollisionPointRec(mousePos, cena->scores.reset)){
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        CreateBinaryFile("records.bin");
      }
    }
    PaginationControl(&cena->scores);
  }
}

//=========================================================================================
/* DesenhaSeletorLevel: int*, Tela_t, Gerenciador_e* -> void
  Função para desenhar o seletor de nível na tela.

Parâmetros:
  - selectedLevel (int*): Ponteiro para a variável que armazena o nível selecionado.
  - param (Tela_t): Estrutura contendo os parâmetros da tela, como largura e altura.
  - gerenciador (Gerenciador_e*): Ponteiro para a variável que gerencia o estado atual do jogo.
Objetivo:
  Desenha os botões de seleção de nível na tela e permite o jogador selecionar um nível ou voltar ao menu principal.
*/
//=========================================================================================
void DesenhaSeletorLevel(int *selectedLevel, Tela_t param, Gerenciador_e *gerenciador){
  const int numLevels = 5;

  Rectangle levelButtons[numLevels];
  Rectangle backButton;

  int buttonWidth = 200;
  int buttonHeight = 60;
  int spacing = 20;
  
  // Calcula as posições dos botões de forma horizontal espaçada.
  int totalWidth = numLevels * (buttonWidth + spacing) - spacing;
  int startX = (param.largura - totalWidth) / 2;
  int buttonY = param.altura / 2 - buttonHeight / 2;

  // Botões de seleção de nível
  for (int i = 0; i < numLevels; i++) {
    levelButtons[i].x = startX + i * (buttonWidth + spacing);
    levelButtons[i].y = buttonY;
    levelButtons[i].width = buttonWidth;
    levelButtons[i].height = buttonHeight;
  }

  // Botão de retorno.
  backButton.x = (param.largura - buttonWidth) / 2;
  backButton.y = param.altura - buttonHeight - spacing;
  backButton.width = buttonWidth;
  backButton.height = buttonHeight;

  DrawRectangleRec((Rectangle){ 0, 0, param.largura, param.altura}, (Color){ 0, 0, 0, 64 });

  // Draw each button and check for clicks
  for(int i = 0; i < numLevels; i++){
    if(CheckCollisionPointRec(GetMousePosition(), levelButtons[i])){
      DrawRectangleRec(levelButtons[i], DARKBLUE);
      DrawRectangleLinesEx(levelButtons[i], 2, BLUE);
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        *selectedLevel = i + 1; // Atualiza o level selecionado;
      }
    } else {
      DrawRectangleRec(levelButtons[i], DARKGRAY);
    }

    DrawText(TextFormat("Level %d", i + 1), 
      levelButtons[i].x + buttonWidth / 2 - MeasureText(TextFormat("Level %d", i + 1), 20) / 2,
      levelButtons[i].y + buttonHeight / 2 - 10,
      20, RAYWHITE
    );
  }

  // Draw the back button
  if(CheckCollisionPointRec(GetMousePosition(), backButton)){
    DrawRectangleRec(backButton, DARKBLUE);
    DrawRectangleLinesEx(backButton, 2, BLUE);
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      *gerenciador = MENU;  // Go back to the menu
    }
  } else {
    DrawRectangleRec(backButton, DARKGRAY);
  }

  DrawText("Back to Menu", 
    backButton.x + buttonWidth / 2 - MeasureText("Back to Menu", 20) / 2,
    backButton.y + buttonHeight / 2 - 10,
    20, RAYWHITE
  );
}

//=========================================================================================
/* DesenhaFundoParallax: Texture2D, Vector2, Tela_t -> void
  Função para desenhar o fundo com efeito parallax.

Parâmetros:
  - background (Texture2D): Textura do fundo a ser desenhada.
  - counter (Vector2): Vetor contendo os valores de deslocamento horizontal e vertical para o efeito parallax.
  - param (Tela_t): Estrutura contendo os parâmetros da tela, como largura e altura.
Objetivo:
  Desenha o fundo na tela com um efeito de parallax, criando a ilusão de movimento ao longo de eixos.
*/
//=========================================================================================
void DesenhaFundoParallax(Texture2D background, Vector2 counter, Tela_t param){
	DrawTextureNPatch(
		background, 
		(NPatchInfo){(Rectangle){ 0, 0, background.width, background.height }, NPATCH_NINE_PATCH}, 
		(Rectangle){ floor(0+counter.x), 0, param.largura, param.altura }, 
		(Vector2){ 0, 0 }, 
		0, 
		WHITE
	);

	DrawTextureNPatch(
		background, 
		(NPatchInfo){(Rectangle){ 0, 0, -background.width, background.height }, NPATCH_NINE_PATCH}, 
		(Rectangle){ floor(param.largura+counter.y), 0, param.largura, param.altura }, 
		(Vector2){ 0, 0 }, 
		0, 
		WHITE
	);
}

//=========================================================================================
/* DesenharJogador: Jogador_t, Rectangle -> void
  Função para desenhar o jogador na tela.

Parâmetros:
  - jogador (Jogador_t): Estrutura contendo os dados do jogador.
  - source (Rectangle): Área da textura a ser desenhada.
Objetivo:
  Desenha a textura do jogador na tela, aplicando a rotação conforme necessário.
*/
//=========================================================================================
void DesenharJogador(Jogador_t jogador, Rectangle source){
  DrawTexturePro(jogador.tex, source, (Rectangle){ jogador.posicao.x + 30, jogador.posicao.y + 30, 60, 60 }, (Vector2){ 30, 30 }, jogador.rotacao, WHITE);
}

//=========================================================================================
/* PaginationControl: Scores_t* -> void
  Função para controlar a navegação entre páginas do leaderboard.

Parâmetros:
  - leaderboards (Scores_t*): Ponteiro para a estrutura contendo os dados do leaderboard.
Objetivo:
  Permite a navegação entre as páginas do leaderboard utilizando as teclas de seta esquerda e direita.
*/
//=========================================================================================
void PaginationControl(Scores_t *leaderboards){
  if(IsKeyPressed(KEY_RIGHT)){
    if(leaderboards->pagina == 4){
      leaderboards->pagina = 4;
    } else{
      leaderboards->pagina++;
    }
  } 
  
  if(IsKeyPressed(KEY_LEFT)){
    if(leaderboards->pagina == 0){
      leaderboards->pagina = 0;
    } else{
      leaderboards->pagina--;
    }
  }
}