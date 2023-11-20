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
    createNewHunters(hunters);

    GhostType *ghost;
    initGhost(&ghost);
    placeGhostInRandomRoom(ghost, house);
    l_ghostInit(ghost->ghostClass, ghost->inRoom->roomName);

    placeHuntersInFirstRoom(house, hunters);
    printHuntersInHouse(house);

    addHunterToRoom(house->rooms.head->next->data, house->huntersInHouse[0]);
    addHunterToRoom(house->rooms.head->next->data, house->huntersInHouse[1]);
    printHuntersInRoom(house->rooms.head->next->data);


    //Thread creation
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
    printf("Ghost thread is running\n");
    printf("Ghost thread is running2");

}

void* runHunterSimulationThread(void* arg){   
    printf("Hunter thread is running\n");
    printf("Hunter thread is running after sleep\n");
}

//get random number in range (0,max-1)
int getRandomInRange(int max){
    return rand() % max;
}



