#include "gerenciador_header.h"
#include "gerenciador_func.h"

//=========================================================================================
/* PreencheMenuInicial: Cena_t*, Tela_t -> void
   Função para inicializar e configurar os elementos do menu principal de um jogo.

Parâmetros:
   - cena (Cena_t*): Ponteiro para a estrutura 'Cena_t' que armazena os dados da cena atual, incluindo strings do menu, botões e parâmetros de texto.
   - param (Tela_t): Estrutura 'Tela_t' que contém as dimensões da tela (largura e altura), utilizada para centralizar e posicionar os elementos do menu.
Objetivo:
   Inicializa as strings do menu, posiciona e dimensiona os botões e outros elementos visuais.
Detalhes:
   - As opções do menu ("Play", "LeaderBoard", "Quit") são centralizadas horizontalmente e espaçadas verticalmente com base no tamanho da fonte e no espaçamento definido.
   - O botão de retorno da tela de 'LeaderBoards' e o botão de 'Reiniciar' também são configurados com base nas dimensões da tela.
   - A caixa de texto para entrada de nome é dimensionada e posicionada adequadamente na tela.
*/
//=========================================================================================
void PreencheMenuInicial(Cena_t *cena, Tela_t param){
  char *string[3] = {
    "Play",
    "LeaderBoard",
    "Quit"
  };
  float verticalSpacing = 10; // Dado de front-end para design.

  // Define um tamanho de fonte.
  cena->fontSize = 45;

  for(int i = 0; i < OPCOES_MENU; i++){
    // Copia as strings para a estrutura base.
    strncpy(cena->string[i], string[i], STRING_MAX_SIZE - 1);
    cena->string[i][STRING_MAX_SIZE - 1] = '\0';

    // Preenche os dados dos retângulos conforme o tamanho das strings de entrada.
    float rectWidth = MeasureText(cena->string[i], cena->fontSize); 

    cena->botoes[i].width = rectWidth;
    cena->botoes[i].height = cena->fontSize;
    cena->botoes[i].x = (param.largura - rectWidth) / 2;  // Centraliza horizontalmente
    cena->botoes[i].y = (param.altura / 1.5f) - ((OPCOES_MENU * (cena->fontSize) + (OPCOES_MENU - 1) * verticalSpacing) / 2) 
      + i * (cena->fontSize + verticalSpacing);  // Posiciona verticalmente com espaçamento

    // Definição de cores
    cena->textColor[i] = YELLOW;

    // Definição do botão de retorno da tela de LeaderBoards;
    cena->scores.retorno = (Rectangle){ param.largura - MeasureText("Voltar", 30) - 20, param.altura - 60, MeasureText("Voltar", 30)+ 4, 34 };
    cena->scores.reset = (Rectangle){ 20, param.altura - 60, MeasureText("Reiniciar", 30)+4, 34};

    cena->nome.caixaTexto = (Rectangle){ param.largura/2.0f - (param.largura * 0.3f), 180, param.largura * 0.6f, 50 };
    cena->nome.nome[NOME_TAM_MAX] = '\0'; 
  }
}