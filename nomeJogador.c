#include "gerenciador_header.h"
#include "gerenciador_func.h"

//=========================================================================================
/* RecebeNomeJogador: Record*, Nome_t*, Jogador_t, int* -> int
   Função para capturar o nome do jogador após a conclusão de uma fase ou jogo.

Parâmetros:
   - novoRecorde (Record*): Ponteiro para a estrutura que armazena o novo recorde do jogador.
   - dados (Nome_t*): Ponteiro para a estrutura que armazena os dados da entrada de nome, incluindo a caixa de texto.
   - jogador (Jogador_t): Estrutura contendo os dados do jogador, como tentativas e id da fase.
   - encerrar (int*): Ponteiro para uma flag que indica se o processo de captura de nome deve ser encerrado.
Objetivo:
   Captura e armazena o nome do jogador, verificando a entrada de caracteres na caixa de texto, e processa o registro do nome.
Retorno:
   - int: Retorna 1 se o nome foi recebido e o processo pode ser encerrado, ou 0 se ainda não foi concluído.
*/
//=========================================================================================
int RecebeNomeJogador(Record *novoRecorde, Nome_t *dados, Jogador_t jogador, int *encerrar){
  if(CheckCollisionPointRec(GetMousePosition(), dados->caixaTexto)){
    dados->mouseSobreCaixa = 1;
  } else{
    dados->mouseSobreCaixa = 0;
  }

  if(dados->mouseSobreCaixa){
    int key = GetCharPressed();
    // Handle character input
    while(key > 0){
      if((key >= 32) && (key <= 125) && (dados->contaLetras < NOME_TAM_MAX)){
        dados->nome[dados->contaLetras] = (char)key;
        dados->nome[dados->contaLetras+1] = '\0'; // Terminate the string
        dados->contaLetras++;
      }
      key = GetCharPressed();  // Check for next character
    }

    // Handle backspace
    if(IsKeyPressed(KEY_BACKSPACE)){
      if(dados->contaLetras > 0){
        dados->contaLetras--;
        dados->nome[dados->contaLetras] = '\0';
      }
    }
  }

  if(dados->mouseSobreCaixa){
    dados->contaFrame++;
  } else {
    dados->contaFrame = 0;
  }

  // Preenche os dados do novo recorde
  if(IsKeyPressed(KEY_ENTER) && dados->contaLetras > 0){
    strcpy(novoRecorde->name, dados->nome);
    puts(novoRecorde->name);
    *encerrar = 0;
    return 1;
  }
  return 0;
}

//=========================================================================================
/* AtualizaRecordeAtual: Record*, Jogador_t -> void
   Função para atualizar o recorde atual do jogador com os dados da fase e tentativas.

Parâmetros:
   - novo (Record*): Ponteiro para a estrutura que armazenará o novo recorde.
   - ref (Jogador_t): Estrutura contendo as tentativas e o id da fase do jogador.
Objetivo:
   Atualiza a estrutura de recorde com os dados da fase atual e o número de tentativas do jogador.
*/
//=========================================================================================
void AtualizaRecordeAtual(Record *novo, Jogador_t ref){
  novo->tries = ref.tentativas;
  novo->level_id = ref.idFase;
  printf("Fase: %d / Tentativas: %d\n", novo->level_id, novo->tries);
}

//=========================================================================================
/* JogadorEntraNoRanking: Record, LevelData*, int -> int
   Função para verificar se o jogador qualifica para entrar no ranking dos melhores tempos/tentativas.

Parâmetros:
   - novo (Record): Estrutura contendo o novo recorde do jogador.
   - fases (LevelData*): Ponteiro para os dados dos níveis, incluindo os rankings top 5.
   - n (int): Número de posições no ranking.
Objetivo:
   Verifica se o novo recorde do jogador é melhor que qualquer um das 5 melhores tentativas no ranking atual.
Retorno:
   - int: Retorna 1 se o jogador qualifica para entrar no ranking, ou 0 caso contrário.
*/
//=========================================================================================
int JogadorEntraNoRanking(Record novo, LevelData *fases, int n){
  int normalizer = novo.level_id - 1; 
  for(int i = 0; i < n; i++){
    if(novo.tries < fases[normalizer].top5[i].tries){
      return 1;
    }
  }
  return 0;
}
