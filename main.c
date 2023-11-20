#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    // HouseType house;
    // initHouse(&house);
    // populateRooms(&house);

    //Initilize: 
    HouseType *house = (HouseType*) calloc(1, sizeof(HouseType));

    // HunterType hunter1, hunter2, hunter3, hunter4;
    // HunterType* hunters[NUM_HUNTERS];
    // hunters[0] = &hunter1;
    // hunters[1] = &hunter2;
    // hunters[2] = &hunter3;
    // hunters[3] = &hunter4;

    // struct Room* van                = createRoom("Van");


    // getHunterName(hunters);
    // for(int i = 0; i < NUM_HUNTERS;i++){
    //     printf("Hunter name %d is, %s\n",i, hunters[i]->hunterName);
    // }

    populateRooms(house);

    return 0;
}


