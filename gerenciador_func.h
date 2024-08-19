#ifndef GERENCIADOR_FUNC_H
#define GERENCIADOR_FUNC_H

//=========================================================================================
// Structure Update Controllers
//=========================================================================================
void PreencheMenuInicial(Cena_t *cena, Tela_t param);
void InicializaJogador(Jogador_t *jogador, int fase);
void AtualizaRecordeAtual(Record *novo, Jogador_t ref);
void AnimationMenu(Cena_t *cena, Vector2 mousePos);
int MenuInteraction(Cena_t menu, Vector2 mousePos);
int RecebeNomeJogador(Record *novoRecorde, Nome_t *dados, Jogador_t jogador, int *encerrar);
Vector2 ProcuraJogador(Fase_t mapa, int tamBloco);

int JogadorEntraNoRanking(Record novo, LevelData *fases, int n);

//=========================================================================================
// Movement and Physics
//=========================================================================================
void AtualizarJogador(Jogador_t *jogador, Fase_t mapa);
void Pulo(Jogador_t *jogador);
void ParallaxCounterControl(Vector2 *counter, Tela_t param);
int SideCollision(Fase_t mapa, Vector2 jogadorPos);

//=========================================================================================
// Front-End Elements
//=========================================================================================
void DesenharJogador(Jogador_t jogador, Rectangle source);
void DesenhaScores(Scores_t leaderboards, int i);
void DesenhaMenu(Cena_t cena, int n);
void DesenhaFase(Fase_t fase, int cooldown);
void DesenhaTelaInput(Nome_t dados, Tela_t param);
void DesenhaSeletorLevel(int *selectedLevel, Tela_t param, Gerenciador_e *gerenciador);
void DesenhaFundoParallax(Texture2D background, Vector2 counter, Tela_t param);

void PaginationControl(Scores_t *leaderboards);

//=========================================================================================
// File Managers (CRUD)
//=========================================================================================
void CreateBinaryFile(const char *filename);
void ReadBinaryFile(const char *filename, LevelData levels[5]);
void UpdateLevelData(LevelData *levels, int level_id, Record *newTop5);
void WriteBinaryFile(const char *filename, LevelData levels[5]);
int ReadFileDataEx(const char *path, Fase_t *mapa);

#endif