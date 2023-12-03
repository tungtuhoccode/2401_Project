#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures

    //Initilization 
    HouseType *house;
    initHouse(&house);
    populateRooms(house); 

    GhostType *ghost;
    initGhost(&ghost);
    placeGhostInRandomRoom(ghost, house, C_TRUE);
    l_ghostInit(ghost->ghostClass, ghost->inRoom->roomName);

    HunterType* hunters[NUM_HUNTERS]; 
    createNewHunters(hunters, &house->sharedEvList);
    placeHuntersInFirstRoom(house, hunters);


    //Threads simulation
    pthread_t ghostThread, hunterThread[NUM_HUNTERS];

    pthread_create(&ghostThread, NULL, runGhostSimulationThread, ghost);
    for (int i =0;i<NUM_HUNTERS;i++){
        pthread_create(&hunterThread[i], NULL, runHunterSimulationThread, hunters[i]);
    }


    pthread_join(ghostThread, NULL);
    for (int i =0;i<NUM_HUNTERS;i++){
        pthread_join(hunterThread[i], NULL);
    }

    printResult(house);
    
    //free memory
    freeHouse(house);
    freeHunterList(hunters);

    freeEvidence(&ghost->allEvidenceInHouseList);
    freeEvidenceList(&ghost->allEvidenceInHouseList);
    freeGhost(ghost);

    return 0;
}

void* runGhostSimulationThread(void* arg){   
    GhostType* ghost = (GhostType*) arg;
    
    while(ghost->boredomTimer < BOREDOM_MAX){
        if(checkHunterInRoom(ghost->inRoom)){
            ghost->boredomTimer = 0;
            if(randInt(0,2) == 0 ){
                leaveEvidence(ghost->inRoom, ghost);
            }
        }else{
            ghost->boredomTimer++;
            int randNumb = randInt(0,3);
            if(randNumb == 0 ){
                leaveEvidence(ghost->inRoom, ghost);
            }else if (randNumb == 1){
                moveGhostToAdjacentRoom(ghost);
            }

        }
        usleep(GHOST_WAIT);
    }
    
    l_ghostExit(LOG_BORED);
    return 0;
}

void* runHunterSimulationThread(void* arg){   
    HunterType* hunter = (HunterType*) arg;
    
    int firstmove = C_TRUE;
    int exitReason = -1;
 
    while (hunter->fear < FEAR_MAX && hunter->bore < BOREDOM_MAX){
        if (checkGhostInRoom(hunter->currentRoom) == C_TRUE){
            hunter->fear++;
            hunter->bore = 0;
        } else {
            hunter->bore++;
        }
        int choice = randInt(0, 3);
        if(choice == 0){
            collectEvidence(hunter, &hunter->currentRoom->roomEvList);
        } else if (choice == 1){
            moveHunter(hunter, &hunter->currentRoom->connectedRooms, firstmove);
            firstmove = C_FALSE;
        } else {
            int count = reviewEvidence(hunter, hunter->sharedEvList);
            if(count == 3){
                exitReason = LOG_EVIDENCE;
                break;
            }
        }
        usleep(HUNTER_WAIT);
    }
    if(hunter->fear >= FEAR_MAX && exitReason == -1){
        exitReason = LOG_FEAR;
    }
    if(hunter->bore >= BOREDOM_MAX && exitReason == -1){
        exitReason = LOG_BORED;
    }
    //remove hunter from room before exit thread
    sem_wait(&hunter->currentRoom->room_mutex);
    removeHunterFromRoom(hunter->currentRoom, hunter);
    sem_post(&hunter->currentRoom->room_mutex);
    
    l_hunterExit(hunter->hunterName, exitReason);
    return 0;
}

void printResult(HouseType* house){
    printf("=================================\n");
    printf("All done! Let's see the result...\n");
    printf("=================================\n");
    int countHunter = 0;
    int ghostWinFlag = C_FALSE;
    for(int i = 0; i < NUM_HUNTERS; i++){
        HunterType *hunter = house->huntersInHouse[i];
        if(hunter->fear >= FEAR_MAX || hunter->bore >= BOREDOM_MAX){
            countHunter++;
            if(hunter->fear >= FEAR_MAX){
                printf("* %s has run away in fear!\n", hunter->hunterName);
            }
            if(hunter->bore >= BOREDOM_MAX) {
                printf("* %s has run away in bore!\n", hunter->hunterName);
            } 
        }
    }

    printf("---------------------------------\n");
    if(countHunter == 4){
        ghostWinFlag = C_TRUE;
        printf("The ghost has won the game!\n");
    } 
    
    identifyGhost(house, ghostWinFlag);
}




