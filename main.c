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

    //Thread creation
    pthread_t ghostThread, hunterThread1, hunterThread2, hunterThread3, hunterThread4;






    placeHuntersInFirstRoom(house, hunters);
    printHuntersInHouse(house);


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

//get random number in range (0,max-1)
int getRandomInRange(int max){
    return rand() % max;
}



