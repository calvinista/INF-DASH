#include "gerenciador_header.h"
#include "gerenciador_func.h"

int main(){
    // Declaração E Inicialização de variáveis.
    Gerenciador_e telaAtual = MENU;
    Tela_t telaTamanho = { SCREEN_WIDTH, SCREEN_HEIGHT };
    Cena_t menuInicial = { 0 };
    Fase_t faseJogo = { 0 };
    Jogador_t jogador = { 0 };
    Record referenciaNula = { 0 };
    Record novoRecord = { 0 };
    Vector2 controladorParallax = { 0 };
    int nivelSelecionado = -1;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(FPS);

    Texture2D fundos[6] = { 0 };

    Camera2D camera = { 0 };
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.zoom = 1.0f;

    // Se o arquivo não existe, cria ele e depois lê.
    if(!(FileExists("records.bin"))){
        CreateBinaryFile("records.bin");
    }
    ReadBinaryFile("records.bin", menuInicial.scores.fases);

    // Preenchimento do menu inicial
    PreencheMenuInicial(&menuInicial, telaTamanho);
    menuInicial.texTitle = LoadTexture("resources/inf_dash.png");
    menuInicial.background = LoadTexture("resources/fundo.png");

    // Carrega todos os fundos de fases;
    for(int i = 0; i < 5; i++){
      fundos[i] = LoadTexture(TextFormat("resources/fundo%d.png", i+1));
    }

    Rectangle source = { 0, 0, 60, 60 }; // Referente ao sprite que é carregado abaixo.
    jogador.tex = LoadTexture("resources/sprite.png");

    while(!WindowShouldClose() && !menuInicial.encerrarJogo){
        camera.target = jogador.posicao; // Atualiza camera;
        if(IsKeyPressed(KEY_F1)) telaAtual = MENU; // Corta-caminho, gambiarra;
        //=========================================================================================
        // Área de atualização de dados: (Back-end)
        //=========================================================================================
        switch(telaAtual){
            case MENU:
                AnimationMenu(&menuInicial, GetMousePosition());
                switch(MenuInteraction(menuInicial, GetMousePosition())){
                    case 1:
                        telaAtual = SELECAO;
                        break;
                    case 2:
                        ReadBinaryFile("records.bin", menuInicial.scores.fases);
                        menuInicial.scores.desenhaScore = 1;
                        menuInicial.menuLock = 1;
                        break;
                    case 3:
                        menuInicial.encerrarJogo = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SELECAO:
                if(nivelSelecionado != -1){
                    ReadFileDataEx(TextFormat("fases/nivel%d.txt", nivelSelecionado), &faseJogo);
                    jogador.posicao = ProcuraJogador(faseJogo, faseJogo.tamBloco);
                    InicializaJogador(&jogador, nivelSelecionado);
                    nivelSelecionado = -1;
                    telaAtual = JOGO;
                }
                break;
            case JOGO:
                ParallaxCounterControl(&controladorParallax, telaTamanho);
                AtualizarJogador(&jogador, faseJogo);
                if(jogador.faseCompleta){
                    jogador.faseCompleta = 0;
                    AtualizaRecordeAtual(&novoRecord, jogador);
                    if(JogadorEntraNoRanking(novoRecord, menuInicial.scores.fases, 5)){
                        menuInicial.flagNome = 1;
                    }
                    telaAtual = FIM;
                }
                break;
            case FIM:
                if(menuInicial.flagNome){
                    if(RecebeNomeJogador(&novoRecord, &menuInicial.nome, jogador, &menuInicial.flagNome)){
                        UpdateLevelData(menuInicial.scores.fases, jogador.idFase, &novoRecord);
                        WriteBinaryFile("records.bin", menuInicial.scores.fases);
                        memcpy(&novoRecord, &referenciaNula, sizeof(Record));
                    }
                } else {
                    telaAtual = MENU;
                }
                break;
        }

        //=========================================================================================
        // Área Gráfica: (Front-end)
        //=========================================================================================
        BeginDrawing();
        switch(telaAtual){
            case MENU:
                ClearBackground(DARKBLUE);
                DesenhaMenu(menuInicial, OPCOES_MENU);
                DesenhaScores(menuInicial.scores, menuInicial.scores.pagina);
                break;
            case SELECAO:
                DesenhaSeletorLevel(&nivelSelecionado, telaTamanho, &telaAtual);
                break;
            case JOGO:
                ClearBackground(BLACK);
                DesenhaFundoParallax(fundos[jogador.idFase-1], controladorParallax, telaTamanho);
                BeginMode2D(camera);
                    DesenhaFase(faseJogo, jogador.cd);
                    DesenharJogador(jogador, source);
                EndMode2D();
                    DrawText("Pressione F1 para retornar ao menu", 20, 20, 20, DARKGRAY);
                    DrawText(TextFormat("Número de Tentativas: %d", jogador.tentativas), 20, 40, 20, DARKGRAY);
                    DrawText(TextFormat("Moedas Coletadas no Total: %d", jogador.pts), 20, 60, 20, GOLD);
                break;
            case FIM:
                ClearBackground(BLACK);
                DrawText(TextFormat("Número de Tentativas: %d", jogador.tentativas), 
                    telaTamanho.largura/2 - MeasureText(TextFormat("Número de Tentativas: %d", jogador.tentativas), 20)/2, 
                    40, 
                    20, 
                    WHITE
                );
                DesenhaTelaInput(menuInicial.nome, telaTamanho);
                break;
        }
        EndDrawing();
    }
    
    //=========================================================================================
    // Área de desalocação de memória: (Manual Garbage Collector)
    //=========================================================================================
    UnloadTexture(menuInicial.texTitle);
    UnloadTexture(menuInicial.background);
    UnloadTexture(jogador.tex);
    for(int i = 0; i < 6; i++){
        UnloadTexture(fundos[i]);
    }
    CloseWindow();
    return 0;
}
