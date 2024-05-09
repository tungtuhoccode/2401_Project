#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     2000
#define GHOST_WAIT      800
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define NUMB_EV_TYPES   3
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef enum LoggerDetails LoggerDetails;

typedef     struct  Room            RoomType; 
typedef     struct  Evidence        EvidenceStructType; 
typedef     struct  Hunter          HunterType; 
typedef     struct  Ghost           GhostType; 
typedef     struct  House           HouseType;

typedef     struct  EvidenceNode   EvidenceNodeType;
typedef     struct  EvidenceList    EvidenceListType;

typedef     struct  RoomNode RoomNodeType;
typedef     struct  RoomList RoomListType;

typedef     struct  HunterNode HunterNodeType;
typedef     struct  HunterList HunterListType;



enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

//Evidence
struct Evidence {
    EvidenceType evidenceType;
};

struct EvidenceNode{
    EvidenceStructType *data;
    EvidenceNodeType *next;
};

struct EvidenceList{
    EvidenceNodeType *head;
    EvidenceNodeType *tail;
    sem_t evList_mutex;
};

//Hunter
struct Hunter {
    RoomType *currentRoom; //pointer to the room they are currently in
    EvidenceType hunterEquipmentType;
    char hunterName[MAX_STR];
    EvidenceListType *sharedEvList;//pointer to collection of evidence
    int fear;
    int bore;
};

struct HunterNode{
    HunterType *data;
    HunterNodeType *next;
};

struct HunterList{
    HunterNodeType *head;
    HunterNodeType *tail;
};



//Room
struct RoomNode {
    RoomType *data;
    RoomNodeType *next;
};

struct RoomList {
    RoomNodeType *head;
    RoomNodeType *tail;
};

struct Room {
   char roomName[MAX_STR];

   GhostType *roomGhost; //each room has 1 ghost
   RoomListType connectedRooms;
   EvidenceListType roomEvList;
   HunterType* huntersInRoom[NUM_HUNTERS];
   int countHunter;
   sem_t room_mutex;
};


//Ghost
struct Ghost {
    GhostClass ghostClass;
    RoomType *inRoom; 
    int boredomTimer;
    EvidenceType ghostEvidenceTypes[NUMB_EV_TYPES];
    EvidenceListType allEvidenceInHouseList; 
};

//House
struct House {
    HunterType *huntersInHouse[NUM_HUNTERS];
    RoomListType rooms; 
    EvidenceListType sharedEvList; 
   
};


// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter


void clearBuffer();
// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

//evidence
/*  Function: initEvidence
    Description: Initilize an evidence struct

    out: evStruct -  double pointer to the struct to be initialized
    in: evType - type of evidence
*/
void initEvidence(EvidenceStructType **evStruct, EvidenceType evType);

/*  Function: initEvidenceList
    Description: Initilize an evidence list

    out: evList -  pointer to evidence list to be initialized
*/
void initEvidenceList(EvidenceListType* evList);

/*  Function: addEvidenceToList
    Description: Add an evidence to an evidence list

    out: list -  pointer to evidence list to add evidence to
    in: evidence - pointer to evidence to be added
*/
void addEvidenceToList(EvidenceListType *list, EvidenceStructType *evidence);

/*  Function: removeEvidenceFromList
    Description: remove an evidence from an evidence list

    out: list -  pointer to evidence list to remove evidence from
    in: evidence - pointer to evidence to be removed
*/
void removeEvidenceFromList(EvidenceListType *list, EvidenceStructType *evidence);

/*  Function: freeEvidence
    Description: free all evidence structs inside the evidence list

    out: evidence -  pointer to evidence to be freed
*/
void freeEvidence(EvidenceListType *list);

/*  Function: freeEvidenceList
    Description: free all evidence nodes inside the evidence list and the evidence list itself 

    out: evidence -  pointer to evidence list to be freed
*/
void freeEvidenceList(EvidenceListType *list);

//Room functions

/*  Function: createRoom
    Description: create a room struct by allocating dynamic memory

    roomNameIn - name of the room
    return: pointer to the created room struct
*/
RoomType* createRoom(char* roomNameIn);

/*  Function: initRoomList
    Description: Initilize a room list

    out: list -  pointer to room list to be initialized
*/
void initRoomList(RoomListType *list);

/*  Function: connectRooms
    Description: connect two rooms together

    out: firstRoom -  pointer to the first room
    out: secondRoom -  pointer to the second room
*/
void connectRooms(RoomType* firstRoom, RoomType* secondRoom);

/*  Function: addRoom
    Description: add a room to a room list

    out: list -  pointer to room list to add room to
    in: room - pointer to room to be added
*/
void addRoom(RoomListType *list, RoomType *room);

/*  Function: addHunterToRoom
    Description: add a hunter to a room

    out: room -  pointer to room to add hunter to
    in: hunter - pointer to hunter to be added
    return C_TRUE if the hunter is added successfully, C_FALSE otherwise
*/
int addHunterToRoom(RoomType *room, HunterType *hunter);

/* Function: removeHunterFromRoom
    Description: remove a hunter from a room

    out: room -  pointer to room to remove hunter from
    in: hunter - pointer to hunter to be removed

    return C_TRUE if the hunter is removed successfully, C_FALSE otherwise
*/
int removeHunterFromRoom(RoomType *room, HunterType *hunter);

/*  Function: freeRoom
    Description: free all room structs inside the room list

    out: list -  pointer to room list to be freed
*/
void freeRoom(RoomListType *list);

/*  Function: freeRoomList
    Description: free all room nodes inside the room list and the room list itself 

    out: list -  pointer to room list to be freed
*/
void freeRoomList(RoomListType *list);

//hunter functions
/*  Function: createNewHunters
    Description: Prompt user to choose their hunters's name and equipment for each hunter.
    When done, create an array of hunter pointers

    in: evList - pointer to the shared evidence list
    out: hunters - pointer to the array of hunter pointers
*/
void createNewHunters(HunterType **hunters, EvidenceListType *evList);

/*  Function: initHunter
    Description: Initialize a hunter struct

    out: hunter - pointer to the hunter struct to be initialized
    in: hunterNameIn - name of the hunter
    in: hunterEquipmentType - type of equipment the hunter has
    in: sharedEvList - pointer to the shared evidence list
*/
void initHunter(HunterType *hunter, char* hunterNameIn, EvidenceType hunterEquipmentType, EvidenceListType *sharedEvList);

/*  Function: initHuntersArray
    Description: Initialize an array of hunter pointers

    out: hunters - pointer to the array of hunter pointers
*/
void initHuntersArray(HunterType** hunters);

/*  Function: placeHuntersInFirstRoom
    Description: Place hunters in the first room of the house

    in: huntersSource - pointer to the source array of hunter pointers
    out: houseDestination - pointer to the house
*/
void placeHuntersInFirstRoom(HouseType* houseDestination, HunterType **huntersSource);

/*  Function: freeHunterList
    Description: free all hunter structs inside the array of hunter pointers 

    in/out: hunter -  pointer to hunter to be freed
*/
void freeHunterList(HunterType **hunters);

//house functions
void populateRooms(HouseType* house);

/*  Function: initHouse
    Description: Initialize a house struct

    out: house - double pointer to the house struct to be initialized
*/
void initHouse(HouseType **house);

/*  Function: freeHouse
    Description: free all things inside the house and the house struct itself

    out: house -  pointer to house to be freed
*/ 
void freeHouse(HouseType *house);

//ghost functions
/*  Function: initGhost
    Description: Initialize a ghost struct

    out: ghost - double pointer to the ghost struct to be initialized
*/
void initGhost(GhostType **ghost);

/*  Function: initGhostEvidenceTypeArray
    Description: Initialize the ghost's evidence array

    out: ghost - pointer to the ghost struct to be initialized
*/
void initGhostEvidenceTypeArray(GhostType *ghost);

/*  Function: placeGhostInRandomRoom
    Description: Place the ghost in a random room of the house

    out: ghost - pointer to the ghost struct
    in: house - pointer to the house
    in: firstMove - flag to indicate if this is the ghost's first move
*/
void placeGhostInRandomRoom(GhostType *ghost, HouseType *house, int firstMove);

/*  Function: identifyGhost
    Description: Identify the ghost's type

    in: house - pointer to the house to get
    in: ghostWinFlag - flag to indicate if the ghost has won
*/
void identifyGhost(HouseType* house, int ghostWinFlag);

/*  Function: freeGhost
    Description: free the ghost struct

    in: ghost -  pointer to ghost to be freed
*/
void freeGhost(GhostType *ghost);

//multithreading functions

/* Function: checkGhostInRoom
    Description: check if the ghost is in current room

    in: room - pointer to the room to check
    return: C_TRUE if there is a ghost in the room, C_FALSE otherwise
*/
int checkGhostInRoom(RoomType *room);

/* Function: leaveEvidence
    Description: leave evidence in the room

    out: room - pointer to the room to leave evidence in
    in: ghost - pointer to the ghost
*/
void leaveEvidence(RoomType *room, GhostType *ghost);

/* Function: moveGhostToAdjacentRoom
    Description: move the ghost to an adjacent room

    out: ghost - pointer to the ghost
*/
void moveGhostToAdjacentRoom(GhostType *ghost);

/* Function: checkHunterInRoom
    Description: check if there is a hunter in the room

    in: room - pointer to the room to check
    return: C_TRUE if there is a hunter in the room, C_FALSE otherwise
*/
int checkHunterInRoom(RoomType *room);

/*  Function: moveHunter
    Description: move the hunter to an adjacent room

    out: hunter - pointer to the hunter
    in: connectedRooms - pointer to the list of connected rooms
    in: firstMove - flag to indicate if this is the hunter's first move.
*/
void moveHunter(HunterType* hunter, RoomListType* connectedRooms, int firstMove);

/*  Function: collectEvidence
    Description: use the hunter to collect an evidence in the room

    out: hunter - pointer to the hunter
    in: evidencesInRoom - pointer to the list of evidence in the room
*/
void collectEvidence(HunterType* hunter, EvidenceListType* evidencesInRoom);

/*  Function: reviewEvidence
    Description: review the evidence in the shared evidence list to see if we have enough evidence to identify the ghost

    in: hunter - pointer to the hunter
    in: sharedEvList - pointer to the shared evidence list

    return: number of unique evidence types in the shared evidence list
*/
int reviewEvidence(HunterType* hunter, EvidenceListType* sharedEvList);

//main functions
/*  Function: runGhostSimulationThread
    Description: run the ghost simulation thread

    in: arg - pointer to the argument struct
*/
void* runGhostSimulationThread(void *arg);

/*  Function: runHunterSimulationThread
    Description: run the hunter simulation thread

    in: arg - pointer to the argument struct
*/
void* runHunterSimulationThread(void* arg);

/*  Function: printResult
    Description: print the result of the simulation

    in: house - pointer to the house
*/
void printResult(HouseType* house);