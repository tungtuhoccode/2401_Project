#include "defs.h"

void getHunterName(HunterType **hunters){
    int equipments[NUM_HUNTERS] = {EMF, TEMPERATURE, FINGERPRINTS, SOUND};
    int i;
    
    for(i = 0; i < NUM_HUNTERS;i++){
        //get user input for hunter name
        printf("\nEnter the hunter name: ");
        char hunter_name[MAX_STR];
        fgets(hunter_name, MAX_STR-1, stdin); 
        hunter_name[strlen(hunter_name)-1] = '\0';
        
        //promp user to get hunter type
        
        printf("Choose the hunter type for %s. Available equipments:\n", hunter_name);
        int choice = -1;
        while(C_TRUE){
            printf("|");
            for(int i=0;i<NUM_HUNTERS;i++){
                if(equipments[i] == -1 ){
                    continue;
                }else{
                    char str[MAX_STR];
                    evidenceToString(equipments[i], str);
                    printf(" %d: %s | ",i,str);
                }
            }
            printf("\n");
            printf("Your choice: ");
            scanf("%d", &choice);
            clearBuffer();

            if(choice < 0 || choice > 3 || equipments[choice] == -1){
                choice = -1;
            }

            if(choice != -1){
                break;
            }
        }


        //allocate dynamic memory for hunter    
        HunterType *newHunter = (HunterType*)calloc(1,sizeof(HunterType));

        //initialize hunter with the hunter name and a default equipment type
        initHunter(newHunter, hunter_name, equipments[choice]);

        //set the hunter into the array
        hunters[i] = newHunter;
        equipments[choice] = -1;

        //log hunter name and equipment
        l_hunterInit(newHunter->hunterName, newHunter->hunterEquipmentType);
        printf("\n");
    }
}

void clearBuffer(){
  char c;
  while((c = getchar()) != '\n');
}

void freeHunterList(HunterType **hunters){
    for(int i = 0;i < NUM_HUNTERS;i++){
        free(hunters[i]);
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
