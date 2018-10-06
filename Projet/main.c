#include "structs.h"

void initSDLComponents(const char *);
void quitSDL(void);

char * getCurrentWord(unsigned int wordSize){
    unsigned int i = 0;
    char* l = NULL;
	char* word = (char*)malloc(sizeof(char) * (wordSize + 1));
    while(i < wordSize){
		l = &(selectedWord[i]->letter->c);
		word[i] = *l;
		i++;
    }
	word[wordSize] = '\0';
    return word;
}

int calculScore(unsigned int wordSize){
    unsigned int i = 0;
    unsigned int multiWord = 1;
    unsigned int scoreTotal = 0;
    Box* b = selectedWord[i];
    while(i < wordSize){
        if(b->bonusType == LETTRE)
        {
            scoreTotal += b->letter->value * b->bonusMultiplier;
        }
        else
        {
            if(b->bonusType == MOT)
                multiWord *= b->bonusMultiplier;
            scoreTotal += b->letter->value;
        }
        i++;
        b = selectedWord[i];
    }
    scoreTotal *= multiWord;
    if(wordSize >= 5){
        scoreTotal += wordSize * 5 - 20;
    }
    return scoreTotal;
}

int main ( int argc, char** argv ){
    SimpleList* wordList = NULL;
    unsigned int timer = 60;

    const char * fichierGrille = "exemple.txt";
    initSDLComponents(fichierGrille);
    dictionary = loadDictionary("dico.txt");
    int score = 0;

    int i = 0;
    int j = 0;
    g = loadGrid(fichierGrille);

    int wordSize = 0;
    selectedWord = (Box**)malloc(sizeof(Box*) * g->h_size * g->v_size + 1);
    for(; i < g->h_size * g->v_size + 1; i++){
        selectedWord[i] = NULL;
    }i = 0;

    clock_t start = clock(), diff;

    int done = FALSE;
    while (!done){

        diff = clock() - start;
		if (diff * 1000 / CLOCKS_PER_SEC > 1000){
			timer--;
			start = clock();
		}
		if (timer <= 0){
			done = TRUE;
		}

        // DRAWING:
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 18, 91, 126));

        SDL_Rect textePos;
        SDL_Color couleur = {255, 120, 0};
        const char* strScore = (const char*)malloc(sizeof(const char) * 4);
        sprintf(strScore, "%d", score);
        police = TTF_OpenFont("retro.ttf", 30);
        if (!police){
            printf("Unable to load ttf file: %s\n", SDL_GetError());
            exit(4);
        }
        texte = TTF_RenderText_Solid(police, (const char*)(strScore), couleur);
        textePos.x = g->h_size * (g->boxes[0][0].hitbox->w + 10) + g->boxes[0][0].hitbox->x + 20;
        textePos.y = 110;
        SDL_BlitSurface(texte, NULL, screen, &textePos);
        char* strWord = getCurrentWord(wordSize);
        texte = TTF_RenderText_Solid(police, strWord, couleur);
        textePos.x = ((g->h_size * (g->boxes[0][0].hitbox->w + 10) + g->boxes[0][0].hitbox->x) / 2);
        textePos.y = 20;
        SDL_BlitSurface(texte, NULL, screen, &textePos);

        char* strTimer = (char*)malloc(sizeof(char)* 4);
		sprintf(strTimer, "%d:%02i", timer/60, timer%60);
		texte = TTF_RenderText_Solid(police, strTimer, couleur);
		textePos.x = 20;
		textePos.y = 110;
		SDL_BlitSurface(texte, NULL, screen, &textePos);

        SDL_Rect infosPos;
        img = IMG_Load("Infos.png");
        if(!img)
        {
            printf( "Unable to load image Infos.png! SDL_image Error: %s\n", IMG_GetError() );
            exit(4);
        }
        infosPos.x = g->h_size * (g->boxes[0][0].hitbox->w + 10) + g->boxes[0][0].hitbox->x + 20;
        infosPos.y = 50 + screen->h / 2 - infosPos.h / 2;
        SDL_BlitSurface(img, NULL, screen, &infosPos);


        drawGrid(g);
        //--------------------------------------------------------------

        // UPDATE:
        SDL_Flip(screen);

        // EVENTS:
        SDL_Event event;
        //SDL_WaitEvent(&event);
        while (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
                done = 2;
            }
            else if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                     || (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)){
                char * word = getCurrentWord(wordSize);
                if(!isInWordList(wordList, word)){
                    if(isInDictionary(dictionary, word)){
                        score += calculScore(wordSize);
                    }
                }
                wordList = listAppend(wordList, word);
                int i = 0;
                int j = 0;
                for(i = 0; i < g->h_size; i++){
                    for(j = 0; j < g->v_size; j++){
                        g->boxes[i][j].selected = FALSE;
                    }j = 0;
                }i = 0;
                selectedBox = NULL;
                for(; i < g->h_size * g->v_size + 1; i++){
                    selectedWord[i] = NULL;
                }i = 0;
                wordSize = 0;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                int x = event.button.x;
                int y = event.button.y;

                int i = 0;
                int j = 0;

                for(; i < g->h_size; i++){
                    for(; j < g->v_size; j++){
                        Box* box = &g->boxes[i][j];
                        if( ( x > box->hitbox->x ) && ( x < box->hitbox->x + box->hitbox->w ) &&
                            ( y > box->hitbox->y ) && ( y < box->hitbox->y + box->hitbox->h ) &&
                            (selectedBox != box) && (box->selected == FALSE)){
                            if(!selectedBox){
                                box->selected = TRUE;
                                selectedBox = box;
                                selectedWord[wordSize] = box;
                                wordSize++;
                            }
                            else{
                                if(box->x <= selectedBox->x+1 && box->x >= selectedBox->x-1 &&
                                box->y <= selectedBox->y+1 && box->y >= selectedBox->y-1){
                                   box->selected = TRUE;
                                    selectedBox = box;
                                    selectedWord[wordSize] = box;
                                    wordSize++;
                               }
                            }
                        }
                    }j = 0;
                }i = 0;
            }
        }
    }

    //Final display
	if (done){
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 18, 91, 126));

		SDL_Rect textePos;
		SDL_Color couleur = { 255, 120, 0 };
		const char* strScore = (const char*)malloc(sizeof(const char)* 4);
		sprintf(strScore, "ScoreTotal : %d", score);
		police = TTF_OpenFont("retro.ttf", 30);
		if (!police){
			printf("Unable to load ttf file: %s\n", SDL_GetError());
			exit(4);
		}
		texte = TTF_RenderText_Solid(police, (const char*)(strScore), couleur);
		textePos.x = 50;
		textePos.y = 20;
		SDL_BlitSurface(texte, NULL, screen, &textePos);

		SimpleList* allWords = getPossibleWords(g, dictionary);
		int size = listLength(allWords);
		SDL_Surface* cadre = SDL_CreateRGBSurface(0, 500, 50 * size + 10, 32, 0, 0, 0, 0);
		SDL_FillRect(cadre, 0, SDL_MapRGB(cadre->format, 18, 91, 126));

		police = TTF_OpenFont("retro.ttf", 30);
		if (!police){
			printf("Unable to load ttf file: %s\n", SDL_GetError());
			exit(4);
		}

		int i = 0;
		while (allWords){
			SDL_Rect textePos;
			texte = TTF_RenderText_Solid(police, (char*)allWords->data, couleur);
			textePos.x = 10;
			textePos.y = i * 50 + 5;
			SDL_BlitSurface(texte, NULL, cadre, &textePos);
			allWords = allWords->next;
			i++;
		}

		SDL_Rect cadrePos;
		cadrePos.x = 50;
		cadrePos.y = 100;
		cadrePos.w = 500;
		cadrePos.h = 30 * size;
		SDL_Rect frame;
		frame.x = 0;
		frame.y = 0;
		frame.w = 500;
		frame.h = 50 * 6 + 30;
		SDL_BlitSurface(cadre, &frame, screen, &cadrePos);

		SDL_Surface* upArrow = IMG_Load("upArrow.png");
		if (!upArrow)
		{
			printf("Unable to load image upArrow.png! SDL_image Error: %s\n", IMG_GetError());
			exit(4);
		}
		SDL_Rect upPos;
		upPos.x = 550;
		upPos.y = 150;
		SDL_BlitSurface(upArrow, NULL, screen, &upPos);

		SDL_Surface* downArrow = IMG_Load("downArrow.png");
		if (!downArrow)
		{
			printf("Unable to load image downArrow.png! SDL_image Error: %s\n", IMG_GetError());
			exit(4);
		}
		SDL_Rect downPos;
		downPos.x = 550;
		downPos.y = 250;
		SDL_BlitSurface(downArrow, NULL, screen, &downPos);

		SDL_Flip(screen);
		int done = FALSE;
		while (!done){
			SDL_Event event;
			while (SDL_PollEvent(&event)){
				if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
					done = TRUE;
				}
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
					int x = event.button.x;
					int y = event.button.y;
					if (x > upPos.x && x < upPos.x + upPos.w && y > upPos.y && y < upPos.y + upPos.h){
						if (frame.y > 0){
							frame.y -= 50;
						}
						else{
							frame.y = 0;
						}
					}
					if (x > downPos.x && x < downPos.x + downPos.w && y > downPos.y && y < downPos.y + downPos.h){
						if (frame.y + frame.h < cadre->h){
							frame.y += 50;
						}
						else{
							frame.y = cadre->h - frame.h;
						}
					}
					SDL_BlitSurface(cadre, &frame, screen, &cadrePos);
					SDL_Flip(screen);
				}
			}
		}
	}

    free(selectedWord);
    deleteGrid(g);
    deleteDictionary(dictionary);

    quitSDL();
    return 0;
}

void initSDLComponents(const char * fichierGrille){
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(1);
    }

    atexit(SDL_Quit);

    int x = getLineSize(fichierGrille);
    int y = getColumnSize(fichierGrille);
    x = 200 + 250 + x * (96 + 10); //info sprite size = 250px, box sprite size = 96px, gap = 10px
    y = 50 + 100 + y * (96 + 10);//box sprite size = 96px, gap = 10px

    screen = SDL_SetVideoMode(x, y, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen ){
        printf("Unable to set  video: %s\n", SDL_GetError());
        exit(1);
    }

    if(TTF_Init() == -1){
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        exit(1);
    }
}

void quitSDL(void){
    SDL_FreeSurface(screen);
    TTF_Quit();
}
