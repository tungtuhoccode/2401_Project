#include "defs.h"

void getHunterName(HunterType **hunters){
    int equipments[NUM_HUNTERS] = {EMF, TEMPERATURE, FINGERPRINTS, SOUND};
    int i;
    
    for(i = 0; i < NUM_HUNTERS;i++){
        //get user input for hunter name
        printf("Enter the hunter name: ");
        char hunter_name[MAX_STR];
        fgets(hunter_name, MAX_STR-1, stdin); 
        hunter_name[strlen(hunter_name)-1] = '\0';
        
        //allocate dynamic memory for hunter    
        HunterType *newHunter = (HunterType*)calloc(1,sizeof(HunterType));

        //initialize hunter with the hunter name and a default equipment type
        initHunter(newHunter, hunter_name, equipments[i]);

        //log hunter name and equipment
        l_hunterInit(newHunter->hunterName, newHunter->hunterEquipmentType);
    }
}

void initHunter(HunterType *hunter, char* hunterNameIn, EvidenceType hunterEquipmentType){
    strcpy(hunter->hunterName, hunterNameIn);
    hunter->hunterEquipmentType = hunterEquipmentType;
    hunter->sharedEvidence = NULL;
    hunter->fear = 0;
    hunter->bore = 0;
}

void initHuntersArray(HunterType** hunters){
    for(int i = 0;i<NUM_HUNTERS;i++){
        hunters[i] = NULL;
    }
}
