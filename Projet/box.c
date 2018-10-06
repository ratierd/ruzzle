#include "structs.h"

void drawBox(Box* box){
    SDL_Rect textePos;
    SDL_Color couleur = {0, 0, 0};

    SDL_Surface* bonus = NULL;
    SDL_Rect bonusPos;

    SDL_Surface* boxSprite = NULL;

    if(box->selected){
        if(box == selectedBox){
            boxSprite = IMG_Load("boxSpriteCurrent.png");
            if(!boxSprite)
            {
                printf( "Unable to load image boxSpriteCurrent.png! SDL_image Error: %s\n", IMG_GetError() );
                exit(4);
            }
        }else{
            boxSprite = IMG_Load("boxSpriteSelected.png");
            if(!boxSprite)
            {
                printf( "Unable to load image boxSpriteSelected.png! SDL_image Error: %s\n", IMG_GetError() );
                exit(4);
            }
        }
    }
    else{
        boxSprite = IMG_Load("boxSprite.png");
        if(!boxSprite)
        {
            printf( "Unable to load image boxSprite.png! SDL_image Error: %s\n", IMG_GetError() );
            exit(4);
        }
    }

    police = TTF_OpenFont("retro.ttf", 24);
    if (!police){
        printf("Unable to load ttf file: %s\n", SDL_GetError());
        exit(4);
    }
    texte = TTF_RenderText_Solid(police, getCharacter(box->letter), couleur);
    textePos.x = (boxSprite->w / 2) - (texte->w / 2);
    textePos.y = (boxSprite->h / 2) - (texte->h / 2);
    SDL_BlitSurface(texte, NULL, boxSprite, &textePos);
    SDL_Flip(boxSprite);

    SDL_FreeSurface(texte);
    TTF_CloseFont(police);

    police = TTF_OpenFont("retro.ttf", 10);
    if (!police){
        printf("Unable to load ttf file: %s\n", SDL_GetError());
        exit(4);
    }
    texte = TTF_RenderText_Solid(police, getValueCharacter(box->letter), couleur);
    textePos.x = boxSprite->w - texte->w - 10;
    textePos.y = 7;
    SDL_BlitSurface(texte, NULL, boxSprite, &textePos);
    SDL_Flip(boxSprite);

    SDL_FreeSurface(texte);
    TTF_CloseFont(police);

    bonusPos.x = 7;
    bonusPos.y = 7;

    if(box->bonusMultiplier == 2){
        if(box->bonusType == LETTRE){
            bonus = IMG_Load("DL.png");
            if(!bonus)
            {
                printf( "Unable to load image TW.png! SDL_image Error: %s\n", IMG_GetError() );
                exit(4);
            }
            SDL_BlitSurface(bonus, 0, boxSprite, &bonusPos);
        }
        else{
            bonus = IMG_Load("DW.png");
            if(!bonus)
            {
                printf( "Unable to load image TW.png! SDL_image Error: %s\n", IMG_GetError() );
                exit(4);
            }
            SDL_BlitSurface(bonus, 0, boxSprite, &bonusPos);
        }
    }
    else if(box->bonusMultiplier == 3){
        if(box->bonusType == LETTRE){
            bonus = IMG_Load("TL.png");
            if(!bonus)
            {
                printf( "Unable to load image TW.png! SDL_image Error: %s\n", IMG_GetError() );
                exit(4);
            }
            SDL_BlitSurface(bonus, 0, boxSprite, &bonusPos);
        }
        else{
            bonus = IMG_Load("TW.png");
            if(!bonus)
            {
                printf( "Unable to load image TW.png! SDL_image Error: %s\n", IMG_GetError() );
                exit(4);
            }
            SDL_BlitSurface(bonus, 0, boxSprite, &bonusPos);
        }
    }

    SDL_BlitSurface(boxSprite, 0, screen, box->hitbox);
    SDL_FreeSurface(boxSprite);
}
