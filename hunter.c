#include "defs.h"

void getHunterName(HunterType **hunters){
    int i;
    
    for(i = 0; i < NUM_HUNTERS;i++){
        printf("Enter the hunter name: ");
        char hunter_name[MAX_STR];
        fgets(hunter_name, MAX_STR-1, stdin); 
        hunter_name[strlen(hunter_name)-1] = '\0';
        
        strcpy(hunters[i]->hunterName, hunter_name);
        printf("Hunter name %d is, %s\n",i, hunters[i]->hunterName);
    }
}


void initHuntersArray(HunterType** hunters){
    for(int i = 0;i<NUM_HUNTERS;i++){
        hunters[i] = NULL;
    }
}
