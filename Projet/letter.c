#include "structs.h"

const char* getCharacter(Letter* letter){
    return &(letter->c);
}

const char* getValueCharacter(Letter* letter){
    switch(letter->value){
    case 1:

        return "1";
    case 2:
        return "2";
    case 3:
        return "3";
    case 4:
        return "4";
    case 8:
        return "8";
    case 10:
        return "10";
    default:
        return "1";
    }
}
