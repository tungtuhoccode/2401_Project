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

    HunterType* hunters[NUM_HUNTERS]; 
    createNewHunters(hunters);

    populateRooms(house); 
    printRoomList(&house->rooms);

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
    freeHunterList(hunters);
    freeHouse(house);

    return 0;
}





