#include "structs.h"

#include <time.h>

int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    long end = RAND_MAX / n;
    end *= n;

    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}

int getLineSize(const char* filename){
    FILE* fichier = NULL;
    int caractereActuel = 0;
    fichier = fopen(filename, "r");
    int r = 0;
    if (fichier != NULL){
        caractereActuel = fgetc(fichier);
        while (caractereActuel != EOF && caractereActuel != '\n'){
            r++;
            caractereActuel = fgetc(fichier);
        }
        fclose(fichier);
    }
    return r;
}

int getColumnSize(const char* filename){
    FILE* fichier = NULL;
    int caractereActuel = 0;
    fichier = fopen(filename, "r");
    int currentSize = 0;
    int checkSize = 0;
    int r = 0;
    if (fichier != NULL){
        do{
            caractereActuel = fgetc(fichier);
            if(caractereActuel == '\n'){
                if(checkSize == 0){
                    checkSize = currentSize;
                }
                else if(currentSize != checkSize){
                    exit(2);// invalid format
                }
                currentSize = 0;
                r++;
            }
            else{
                currentSize++;
            }
        }while(caractereActuel != EOF);
        fclose(fichier);
    }
    return r;
}

Grid* loadGrid(const char* filename){
    FILE* fichier = NULL;
    char caractereActuel;
    int h_size = getLineSize(filename);
    int v_size = getColumnSize(filename);
    Letter* l = NULL;
    Box* b = NULL;
    Grid* g = (Grid*)malloc(sizeof(Grid));
    int hRBonus[h_size];
    int vRBonus[v_size];
    int distinctBox;
    int bonusCt = 0;
    int i = 0, j = 0;

    SDL_Surface* boxSprite = IMG_Load("boxSprite.png");
    if(!boxSprite)
    {
        printf( "Unable to load image boxSprite.png! SDL_image Error: %s\n", IMG_GetError() );
        exit(2);
    }

    g->h_size = h_size;
    g->v_size = v_size;
    g->boxes = (Box**)malloc(sizeof(Box*) * h_size);
    for(; i < v_size; i++) {
      g->boxes[i] = (Box*)malloc(sizeof(Box) * v_size);
    }i = 0;

    srand(time(NULL));
    for(; i < h_size; i++){
        hRBonus[i] = randint(4);
    }i = 0;
    for(; i < v_size; i++){
        do{
            distinctBox = TRUE;
            vRBonus[i] = randint(4);
            for(; j < v_size; j++){
                if(i != j){
                    if(vRBonus[i] == vRBonus[j] && hRBonus[i] == hRBonus[j]){
                        distinctBox = FALSE;
                    }
                }
            }j = 0;
        }while(!distinctBox);
    }i = 0;

    fichier = fopen(filename, "r");

    if (fichier != NULL)
    {
        do
        {
            caractereActuel = toupper((char)fgetc(fichier));
            if(caractereActuel != '\n' && caractereActuel != EOF){
                l = (Letter*)malloc(sizeof(Letter));
                l->c = caractereActuel;

                switch(caractereActuel){
                case 'E':
                case 'A':
                case 'I':
                case 'N':
                case 'O':
                case 'R':
                case 'S':
                case 'T':
                case 'U':
                case 'L':
                    l->value = 1;
                    break;
                case 'D':
                case 'G':
                case 'M':
                    l->value = 2;
                    break;
                case 'B':
                case 'C':
                case 'P':
                    l->value = 3;
                    break;
                case 'H':
                case 'V':
                case 'F':
                    l->value = 4;
                    break;
                case 'J':
                case 'Q':
                    l->value = 8;
                    break;
                case 'K':
                case 'W':
                case 'X':
                case 'Y':
                case 'Z':
                    l->value = 10;
                    break;
                }

                b = (Box*)malloc(sizeof(Box));
                b->letter = l;

                if((i == hRBonus[0] && j == vRBonus[0]) ||
                   (i == hRBonus[1] && j == vRBonus[1]) ||
                   (i == hRBonus[2] && j == vRBonus[2]) ||
                   (i == hRBonus[3] && j == vRBonus[3])){
                    switch(bonusCt){
                    case 0://DOUBLE LETTER
                        b->bonusMultiplier = 2;
                        b->bonusType = LETTRE;
                        break;
                    case 1://TRIPLE LETTER
                        b->bonusMultiplier = 3;
                        b->bonusType = LETTRE;
                        break;
                    case 2://DOUBLE WORD
                        b->bonusMultiplier = 2;
                        b->bonusType = MOT;
                        break;
                    case 3://TRIPLE WORD
                        b->bonusMultiplier = 3;
                        b->bonusType = MOT;
                        break;
                    }
                    bonusCt++;
                }else{
                    b->bonusMultiplier = 1;
                    b->bonusType = LETTRE;
                }

                b->x = j;
                b->y = i;

                b->hitbox = (SDL_Rect*)malloc(sizeof(SDL_Rect));
                b->hitbox->x = 150 + (j) * (boxSprite->w + 10);//gap = 10
                b->hitbox->y = 50 + (screen->h - (boxSprite->h + 10) * v_size) / 2 + (i) * (boxSprite->h + 10);//gap = 10

                b->selected = FALSE;

                g->boxes[i][j] = *b;
                j++;
            }else{
                i++;
                j = 0;
            }
        } while (caractereActuel != EOF);
        fclose(fichier);
    }
    SDL_FreeSurface(boxSprite);
    return g;
}

void deleteGrid(Grid* g){
    free(g->boxes);
    free(g);
}

void drawGrid(Grid* g){
    int i = 0;
    int j = 0;
    for(; i < g->h_size; i++){
        for(; j < g->v_size; j++){
            drawBox(&(g->boxes[i][j]));
        }j = 0;
    }
}

