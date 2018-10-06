#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define TRUE 1
#define FALSE 0

int getLineSize(const char* filename);
int getColumnSize(const char* filename);

typedef enum BonusType{LETTRE, MOT} BonusType;

typedef struct Letter{
    char* c;
    unsigned int value;
} Letter;

typedef struct Box{
    Letter* letter;
    int bonusMultiplier;
    BonusType bonusType;

    //coordinates in the grid:
    int x;
    int y;

    //position on screen:
    SDL_Rect* hitbox;
    short selected;
} Box;

typedef struct Grid{
    int h_size;
    int v_size;
    Box** boxes;
} Grid;

typedef struct Dictionary {
	char letter;
	int isWord;
	struct Dictionary* next[27];
	struct Dictionary* previous;
} Dictionary;

typedef struct SimpleList
{
	struct SimpleList* next;
	struct SimpleList* previous;
	void* data;
} SimpleList;

const char* getCharacter(Letter*);
const char* getValueCharacter(Letter*);

void drawBox(Box*);
Box** getAdjacentBoxes(Grid*, Box*, int*);

Grid* loadGrid(const char *);
void deleteGrid(Grid*);
void drawGrid(Grid*);

Dictionary* loadDictionary(char*);
void deleteDictionary(Dictionary*);
int isInDictionary(Dictionary*, const char *);
SimpleList* getPossibleWords(Grid*, Dictionary*);
void getPossibleWordsSub(Grid*, Dictionary*, Box*, SimpleList**);

SimpleList* listCreate(SimpleList*, void*);
SimpleList* listAppend(SimpleList*, void*);
void listCat(SimpleList**, SimpleList*);
SimpleList* listRemoveFirst(SimpleList*);
void listDestroy(SimpleList*);
int listLength(SimpleList*);
SimpleList* listBegin(SimpleList*);
int isInWordList(SimpleList*, const char*);
int isInBoxList(SimpleList* list, Box* box);

//Globals
SDL_Surface* screen;

Grid* g;
Box* selectedBox;
Box** selectedWord;
Dictionary* dictionary;

SDL_Surface* img;
SDL_Surface* texte;
TTF_Font* police;

#endif // STRUCTS_H_INCLUDED
