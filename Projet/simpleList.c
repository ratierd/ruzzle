#include "structs.h"

SimpleList* listCreate(SimpleList* previous, void* data)
{
	SimpleList* list = malloc(sizeof(SimpleList));
	if (list)
	{
		list->data = data;
		list->next = NULL;
		list->previous = previous;
	}
	return list;
}

SimpleList* listAppend(SimpleList* list, void* data){
	SimpleList** plist = &list;
	SimpleList* previous = NULL;
	while (*plist){
		previous = *plist;
		plist = &(*plist)->next;
	}
	*plist = listCreate(previous, data);
	if (*plist)
		return list;
	else
		return NULL;
}

void listCat(SimpleList** l1, SimpleList* l2){
	if (l2 != NULL){
		while (l2){
			*l1 = listAppend(*l1, l2->data);
			l2 = l2->next;
		}
	}
}

SimpleList* listRemoveFirst(SimpleList* list){
	SimpleList *first = list;
	list = list->next;
	free(first);
	return list;
}

void listDestroy(SimpleList* list){
	while (list != NULL)
		list = listRemoveFirst(list);
}

int listLength(SimpleList* list){
	int length = 0;
	while (list)
	{
		length++;
		list = list->next;
	}
	return length;
}

SimpleList* listBegin(SimpleList* list){
	if (list != NULL){
		while (list->previous)
		{
			list = list->previous;
		}
	}
	return list;
}

int isInWordList(SimpleList* list, const char* word){
	while (list){
		if (strcmp((char *)list->data, word) == 0)
			return 1;
		list = list->next;
	}
	return 0;
}

int isInBoxList(SimpleList* list, Box* box){
	while (list){
		if (box == list->data)
			return 1;
		list = list->next;
	}
	return 0;
}

/*
* Return the word composed by selected boxes
*/
char * getWordFromBoxList(SimpleList* boxList){
	unsigned int i = 0;
	unsigned int wordSize = listLength(boxList);
	char * r = (char *)malloc(sizeof(char)* (wordSize + 1));
	while (boxList){
		char * c = getCharacter(((Box*)boxList->data)->letter);
		r[i] = *c;
		boxList = boxList->next;
		i++;
	}
	r[wordSize] = '\0';
	return r;
}
