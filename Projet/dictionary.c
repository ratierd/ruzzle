#include "structs.h"

Dictionary* loadDictionary(char* file){
	FILE* fichier = NULL;
	Dictionary* dico = (Dictionary*)malloc(sizeof(Dictionary));
	Dictionary* current = dico;
	dico->letter = 0;
	int i = 0;
	for (; i < 27; i++){
		current->next[i] = NULL;
	}i = 0;

	fichier = fopen(file, "r");

	char caractereActuel;
	if (fichier != NULL){
		do{
			caractereActuel = (char)fgetc(fichier);
			if (caractereActuel != '\n' && caractereActuel != '¶' && caractereActuel != '¹' && caractereActuel != EOF){
				Dictionary* temp = current;
				if (((unsigned int)caractereActuel) - 97 < 0 || ((unsigned int)caractereActuel) - 97 > 26){
					//Utilisation d'une case suplémentaire en cas de caractère spécial
					if (current->next[26] == NULL){
						current->next[26] = (Dictionary*)malloc(sizeof(Dictionary));
						current = current->next[26];
						for (i = 0; i < 27; i++){
							current->next[i] = NULL;
						}i = 0;
					}
					else{
						current = current->next[26];
					}
					current->previous = temp;
					current->letter = ' ';
					Dictionary* findEmptySpace = NULL;
					int i = 0;
					do{
						findEmptySpace = current->next[i];
						i++;
					} while (findEmptySpace != NULL && i < 27);
					if (i >= 27){
						exit(5);//No more space for special character
					}
					findEmptySpace = (Dictionary*)malloc(sizeof(Dictionary));
					findEmptySpace->previous = current;
					current = findEmptySpace;
					for (i = 0; i < 27; i++){
						current->next[i] = NULL;
					} i = 0;
					current->isWord |= 0;
					current->letter = caractereActuel;
				}
				else if (current->next[((unsigned int)caractereActuel) - 97] == NULL){
					current->next[((unsigned int)caractereActuel) - 97] = (Dictionary*)malloc(sizeof(Dictionary));
					current = current->next[((unsigned int)caractereActuel) - 97];
					current->previous = temp;
					for (i = 0; i < 27; i++){
						current->next[i] = NULL;
					}i = 0;
					current->isWord = 0;
					current->letter = caractereActuel;
				}
				else{
					current = current->next[((unsigned int)caractereActuel) - 97];
					current->previous = temp;
				}
			}
			else{
				current->isWord = 1;
				current = dico;
			}
		} while (caractereActuel != EOF);
	}
	fclose(fichier);
	dico->isWord = 0;
	return dico;
}

int isInDictionary(Dictionary* dico, const char * mot){
	Dictionary* parcourt = dico;
	char caractere = ' ';
	caractere = *mot;
	while (caractere != '\0'){
		if (parcourt->next[tolower(caractere) - 97] != NULL){
			parcourt = parcourt->next[tolower(caractere) - 97];
		}
		else{
			return 0;
		}
		mot++;
		caractere = *mot;
	}
	return parcourt->isWord;
}

void listDictionary(Dictionary* dico, FILE* fichier){
	static unsigned int cpt = 0;
	cpt++;
	int i = 0;
	for (; i < 27; i++){
		if (dico->isWord){
			int j = 0;
			Dictionary* currentNode = dico;
			char* word = (char*)malloc(sizeof(char)* cpt);
			while (currentNode->letter != '\0'){
				word[j] = currentNode->letter;
				j++;
				currentNode = currentNode->previous;
			}
			word[j] = '\0';
			fprintf(fichier, "\n%s", word);
			free(word);
		}
		if (dico->next[i]){
			listDictionary(dico->next[i], fichier);
		}
	}
	cpt = 0;
}

void deleteDictionary(Dictionary* dico){
    int i = 0;
	for (; i < 27; i++){
		if (dico->next[i]){
			deleteDictionary(dico->next[i]);
		}
	}
	free(dico);
}

/*SimpleList* getPossibleWords(Grid* g, Dictionary* root){
	SimpleList* r = NULL;
	int i = 0;
	int j = 0;
	for (; i < g->h_size; i++){
		for (; j < g->v_size; j++){
			SimpleList* start = NULL;
			start = listAppend(start, &(g->boxes[i][j]));
			SimpleList* words = NULL;
			getPossibleWordsSub(g, root, start, &words);
			listCat(&r, words);
		}j = 0;
	}i = 0;
	return r;
}

void getPossibleWordsSub(Grid* g, Dictionary* currentNode, SimpleList* currentElement, SimpleList** wordList){
	char letter = tolower((((Box*)currentElement->data)->letter->c));
	currentNode = currentNode->next[(unsigned int)letter - 97];
	if (currentNode != NULL){
		if (currentNode->isWord){
			char* word = getWordFromBoxList(listBegin(currentElement));
			*wordList = listAppend(*wordList, word);
		}
		Box* currentBox = (Box*)currentElement->data;
		int x = currentBox->x - 1;
		int y = currentBox->y - 1;
		while (x < currentBox->x + 1 || y < currentBox->y + 1){
			if (x == currentBox->x && y == currentBox->y){
				x++;
			}
			if (x >= 0 && x < g->h_size && y >= 0 && y < g->v_size){
				if (!isInBoxList(listBegin(currentElement), &(g->boxes[x][y]))){
					listAppend(currentElement, &(g->boxes[x][y]));
					*wordList = listBegin(*wordList);
					getPossibleWordsSub(g, currentNode, currentElement->next, wordList);
				}
			}
			x++;
			if (x > currentBox->x + 1){
				y++;
				x = currentBox->x - 1;
			}
		}
	}
}*/

SimpleList* getPossibleWords(Grid* g, Dictionary* root){
	SimpleList* r = NULL;
	int i = 0;
	int j = 0;
	for (; i < g->h_size; i++){
		for (; j < g->v_size; j++){
			SimpleList* words = NULL;
			getPossibleWordsSub(g, root, &g->boxes[i][j], &words);
			listCat(&r, words);
		}j = 0;
	}i = 0;
	return r;
}

void reverse(char s[])
{
	int length = strlen(s);
	int c, i, j;

	for (i = 0, j = length - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

char * getWordFromNode(Dictionary* currentNode){
	char* str = (char*)malloc(sizeof(char) * 16);
	int i = 0;
	while (currentNode->letter != '\0'){
		str[i] = currentNode->letter;
		currentNode = currentNode->previous;
		i++;
	}
	str[i] = '\0';
	reverse(str);
	return str;
}

void getPossibleWordsSub(Grid* g, Dictionary* currentNode, Box* currentBox, SimpleList** wordList){
	char letter = tolower((currentBox->letter->c));
	currentBox->selected = 1;
	currentNode = currentNode->next[(unsigned int)letter - 97];
	if (currentNode != NULL){
		if (currentNode->isWord){
			char* word = getWordFromNode(currentNode);
			*wordList = listAppend(*wordList, word);
		}
		int x = currentBox->x - 1;
		int y = currentBox->y - 1;
		while (x < currentBox->x + 1 || y < currentBox->y + 1){
			if (x == currentBox->x && y == currentBox->y){
				x++;
			}
			if (x >= 0 && x < g->h_size && y >= 0 && y < g->v_size){
				if (!g->boxes[x][y].selected){
					currentBox = &g->boxes[x][y];
					getPossibleWordsSub(g, currentNode, currentBox, wordList);
					currentBox->selected = FALSE;
				}
			}
			x++;
			if (x > currentBox->x + 1){
				y++;
				x = currentBox->x - 1;
			}
		}
	}
}
