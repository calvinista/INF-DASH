#include "gerenciador_header.h"
#include "gerenciador_func.h"

//=========================================================================================
/* CreateBinaryFile: const char* -> void
   Função para criar um arquivo binário e inicializar dados de níveis.

Parâmetros:
   - filename (const char*): Nome do arquivo binário a ser criado.
Objetivo:
   Cria um arquivo binário com dados iniciais de cinco níveis, cada um contendo um top 5 de recordes pré-definidos.
*/
//=========================================================================================
void CreateBinaryFile(const char *filename){
  FILE *file;
  LevelData level;

  // Abre o arquivo em modo escrita binária
  if(!(file = fopen(filename, "wb"))){
    printf("Erro ao criar arquivo de dados\n");
  }

  for(int fases = 1; fases < 6; fases++){
    level.level_id = fases;
    
    strcpy(level.top5[0].name, "Abra");
    level.top5[0].tries = 10;

    strcpy(level.top5[1].name, "Pikachu");
    level.top5[1].tries = 25;

    strcpy(level.top5[2].name, "Dragonite");
    level.top5[2].tries = 50;

    strcpy(level.top5[3].name, "Charmander");
    level.top5[3].tries = 75;

    strcpy(level.top5[4].name, "Latios");
    level.top5[4].tries = 100;

    // Escreve dados padrão no arquivo.
    fwrite(&level, sizeof(LevelData), 1, file);
  }
  // Fecha arquivo
  fclose(file);
  printf("Arquivo binario criado com sucesso!\n");
}

//=========================================================================================
/* ReadBinaryFile: const char*, LevelData[5] -> void
   Função para ler dados de níveis de um arquivo binário.

Parâmetros:
   - filename (const char*): Nome do arquivo binário a ser lido.
   - levels (LevelData[5]): Array para armazenar os dados dos cinco níveis lidos.
Objetivo:
   Lê os dados de cinco níveis de um arquivo binário e armazena-os no array fornecido.
*/
//=========================================================================================
void ReadBinaryFile(const char *filename, LevelData levels[5]){
  FILE *file;
  if(!(file = fopen(filename, "rb"))){
    printf("Problema na leitura do arquivo.\n");
  }

  // Lê exatos 5 levels do arquivo binário.
  fread(levels, sizeof(LevelData), 5, file);
  fclose(file);
}

//=========================================================================================
/* UpdateLevelData: LevelData*, int, Record* -> void
   Função para atualizar os dados de um nível com um novo recorde.

Parâmetros:
   - levels (LevelData*): Array de estruturas contendo os dados dos níveis.
   - level_id (int): ID do nível a ser atualizado.
   - newTop5 (Record*): Ponteiro para a estrutura do novo recorde a ser inserido.
Objetivo:
   Atualiza a lista de top 5 recordes de um nível específico, inserindo o novo recorde na posição correta.
*/
//=========================================================================================
void UpdateLevelData(LevelData *levels, int level_id, Record *newTop5){
  int normalizer = level_id -1;
  int index = -1;

  // Procura a posição do novo recorde na lista;
  for(int i = 0; i < 5; i++){
    if(newTop5->tries < levels[normalizer].top5[i].tries && index == -1){
      index = i;
    }
  }

  // Se o record é melhor que o último ou está no top 5, faz um shift e insere.
  if(index != -1){
    for(int i = 4; i > index; i--){
      levels[normalizer].top5[i] = levels[normalizer].top5[i - 1];
    }

    // Insere o novo recorde na posição correta
    strcpy(levels[normalizer].top5[index].name, newTop5->name);
    levels[normalizer].top5[index].tries = newTop5->tries;
    levels[normalizer].top5[index].level_id = newTop5->level_id;
  }

  // Debugger da função
  /*
  for(int i = 0; i < 5; i++){
    printf("%d: %s - Tries: %d\n", i + 1, levels[normalizer].top5[i].name, levels[normalizer].top5[i].tries);
  }
  */
}

//=========================================================================================
/* WriteBinaryFile: const char*, LevelData[5] -> void
   Função para escrever os dados atualizados de níveis em um arquivo binário.

Parâmetros:
   - filename (const char*): Nome do arquivo binário onde os dados serão escritos.
   - levels (LevelData[5]): Array contendo os dados atualizados dos níveis.
Objetivo:
   Sobrescreve o arquivo binário com os dados atualizados de cinco níveis.
*/
//=========================================================================================
void WriteBinaryFile(const char *filename, LevelData levels[5]){
  FILE *file = fopen(filename, "wb");
  if(!file){
    printf("Falha na leitura do arquivo - Escrita de Arquivo Binario\n");
  }

  // Escreve no arquivo os dados novos;
  fwrite(levels, sizeof(LevelData), 5, file);
  fclose(file);
}