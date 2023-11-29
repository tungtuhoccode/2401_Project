#include "defs.h"

int main()
{
    int i = 0;
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures

    //Initilize
    HouseType *house;
    initHouse(&house);
    populateRooms(house); 
    //printRoomList(&house->rooms);

    HunterType* hunters[NUM_HUNTERS]; 
    createNewHunters(hunters, &house->sharedEvList);
    placeHuntersInFirstRoom(house, hunters);

    GhostType *ghost;
    initGhost(&ghost);
    placeGhostInRandomRoom(ghost, house, C_TRUE);
    l_ghostInit(ghost->ghostClass, ghost->inRoom->roomName);

    //Thread creation
    //Simnulation started
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
    
    //free
    freeGhost(ghost);
    freeHunterList(hunters);
    freeHouse(house);

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
    if(hunter->fear >= FEAR_MAX){
        exitReason = LOG_FEAR;
    }
    if(hunter->bore >= BOREDOM_MAX){
        exitReason = LOG_BORED;
    }
    //remove hunter from room before exit thread
    sem_wait(&hunter->currentRoom->room_mutex);
    int remove = removeHunterFromRoom(hunter->currentRoom, hunter);
    sem_post(&hunter->currentRoom->room_mutex);
    
    l_hunterExit(hunter->hunterName, exitReason);
    return 0;
}

//get random number in range (0,max-1)
int getRandomInRange(int max){
    return rand() % max;
}

void printResult(HouseType* house){
    int countHunter = 0;
    for(int i = 0; i < NUM_HUNTERS; i++){
        HunterType *hunter = house->huntersInHouse[i];
        if(hunter->fear >= FEAR_MAX || hunter->bore >= BOREDOM_MAX){
            countHunter++;
            if(hunter->fear >= FEAR_MAX){
                printf("%s is too fear.\n", hunter->hunterName);
            }
            if(hunter->bore >= BOREDOM_MAX) {
                printf("%s is too bore.\n", hunter->hunterName);
            } 
        }
    }
    if(countHunter == 4){
        printf("The ghost has won.\n");
    }

    printf("List of all evidences:\n");
    printEvidenceList(&house->sharedEvList);
    identifyGhost(house);
}

void identifyGhost(HouseType* house){
    sem_wait(&house->sharedEvList.evList_mutex);
    EvidenceNodeType* currNode = house->sharedEvList.head;
    int evArr[EV_COUNT]= {-1, -1, -1, -1};
    int count = 0;

    while (currNode != NULL){
        if(evArr[currNode->data->evidenceType] == -1){
            evArr[currNode->data->evidenceType] = 1;
            count ++;
        }
        if(count == 3){
            printf("Enough evidence to guess the ghost type...\n");
            if(evArr[EMF] > 0 && evArr[TEMPERATURE] > 0 && evArr[FINGERPRINTS] > 0){
                printf("POLTERGEIST is found.\n");  
            } else if(evArr[EMF] > 0 && evArr[TEMPERATURE] > 0 && evArr[SOUND] > 0){
                printf("BANSEE is found.\n");
            } else if(evArr[EMF] > 0 && evArr[FINGERPRINTS] > 0 && evArr[SOUND] > 0){
                printf("BULLIES is found.\n");
            } else if(evArr[TEMPERATURE] > 0 && evArr[FINGERPRINTS] > 0 && evArr[SOUND] > 0){
                printf("PHANTOM is found.\n");
            }
            printf("\n");
            break;
        }
        currNode = currNode->next;
    }
    sem_post(&house->sharedEvList.evList_mutex);
}

