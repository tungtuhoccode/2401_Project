#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    // HouseType house;

    // populateRooms(&house);

    //Initilize
    HouseType *house;
    initHouse(&house);
    populateRooms(house); 
    printRoomList(&house->rooms);

    HunterType* hunters[NUM_HUNTERS]; 
    createNewHunters(hunters, &house->sharedEvList);

    GhostType *ghost;
    initGhost(&ghost);
    placeGhostInRandomRoom(ghost, house);
    l_ghostInit(ghost->ghostClass, ghost->inRoom->roomName);

    placeHuntersInFirstRoom(house, hunters);

    printHuntersInHouse(house);

    RoomType *VanRoom = house->rooms.head->data;

    // addHunterToRoom(VanRoom, house->huntersInHouse[0]);
    // addHunterToRoom(VanRoom, house->huntersInHouse[1]);
    // addHunterToRoom(VanRoom, house->huntersInHouse[2]);
    // addHunterToRoom(VanRoom, house->huntersInHouse[3]);
    // removeHunterFromRoom(house->rooms.head->next->data, house->huntersInHouse[3]);
    printHuntersInRoom(house->rooms.head->next->data);


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



    // RoomNodeType *currNode = house->rooms.head;
    // while(currNode != NULL){
    //     printf("Connected room to %s are:\n", currNode->data->roomName);
    //     printRoomList(&currNode->data->connectedRooms);
    //     printf("\n\n-----------------------\n\n");
    //     currNode = currNode->next;
    // }


    //free
    freeGhost(ghost);
    freeHunterList(hunters);
    freeHouse(house);

    return 0;
}

void* runGhostSimulationThread(void* arg){   
    GhostType* ghost = (GhostType*) arg;

}

void* runHunterSimulationThread(void* arg){   
    HunterType* hunter = (HunterType*) arg;
    
    int firstmove = C_TRUE;
    while (hunter->fear < FEAR_MAX && hunter->bore < BOREDOM_MAX){
        if (checkGhostInRoom(hunter->currentRoom) == C_TRUE){//NOTE: need to update to use semaphore
            hunter->fear++;
            hunter->bore = 0;
        } else {
            hunter->bore++;
        }

        int choice = randInt(0, 3);
        if(choice == 0){
            collectEvidence(hunter, &hunter->currentRoom->roomEvList); //!!!!!NOTE: TEST
        } else if (choice == 1){
            moveHunter(hunter, &hunter->currentRoom->connectedRooms, firstmove); //DONE TEST
            firstmove = C_FALSE;
        } else {
            reviewEvidence(hunter, hunter->sharedEvList); //!!!!!NOTE: TEST and if count == 3 exit the program
        }
        //REMEMBER TO SLEEP
        sleep(HUNTER_WAIT/1000/50);
    }

    printf("Hunter thread is running\n");

    printf("Hunter thread is running after sleep\n");
    return 0;
}

//get random number in range (0,max-1)
int getRandomInRange(int max){
    return rand() % max;
}



