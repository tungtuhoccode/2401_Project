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
    HouseType *house = (HouseType*) calloc(1, sizeof(HouseType)); //allocate data, have to free later
    HunterType* hunters[NUM_HUNTERS]; //allocated data, have to free later

    getHunterName(hunters);
    populateRooms(house);

    //free
    freeHunterList(hunters);

    freeHouse(house);

    return 0;
}



