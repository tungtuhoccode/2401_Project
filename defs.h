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
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
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
   /*
   TODO:
   1. EvidenceType
   3. HunterType
   4. GhostType
   
   5. EvidenceNodeType
   6. EvidenceListType

   7. RoomListType
   8. RoomNodeType



   9. RoomType

   10. House
    
    11. HunterNode
    12. HunterListType
   
   */

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
void initEvidence(EvidenceStructType **evStruct, EvidenceType evType);
void initEvidenceList(EvidenceListType* evList);
void addEvidenceToList(EvidenceListType *list, EvidenceStructType *evidence);
void printEvidenceList(EvidenceListType *list);
void removeEvidenceFromList(EvidenceListType *list, EvidenceStructType *evidence);
void freeEvidence(EvidenceListType *list);
void freeEvidenceList(EvidenceListType *list);

//Room functions
RoomType* createRoom(char* roomNameIn);
void initRoomList(RoomListType *list);
void connectRooms(RoomType* firstRoom, RoomType* secondRoom);
void addRoom(RoomListType *list, RoomType *room);

void printRoom(RoomType *room);
void printRoomList(RoomListType *list);

void freeRoom(RoomListType *list);
void freeRoomList(RoomListType *list);

int checkHunterInRoom(RoomType *room);

//hunter
void initHuntersArray(HunterType** hunters);
void createNewHunters(HunterType **hunters, EvidenceListType *evList);
void initHunter(HunterType *hunter, char* hunterNameIn, EvidenceType hunterEquipmentType, EvidenceListType *sharedEvList);
void freeHunterList(HunterType **hunters);
void printHuntersInRoom(RoomType *room);

int addHunterToRoom(RoomType *room, HunterType *hunter);
int removeHunterFromRoom(RoomType *room, HunterType *hunter);
int checkGhostInRoom(RoomType *room);
void collectEvidence(HunterType* hunter, EvidenceListType* evidencesInRoom);
void moveHunter(HunterType* hunter, RoomListType* connectedRooms, int firstMove);
int reviewEvidence(HunterType* hunter, EvidenceListType* sharedEvList);
void moveGhostToAdjacentRoom(GhostType *ghost);
void leaveEvidence(RoomType *room, GhostType *ghost);
//house 
void populateRooms(HouseType* house);
void freeHouse(HouseType *house);
void initHouse(HouseType **house);
void printHuntersInHouse(HouseType *house);
void addHuntersToHouse(HouseType* houseDestination, HunterType **huntersSource);
void placeHuntersInFirstRoom(HouseType* houseDestination, HunterType **huntersSource);
void placeGhostInRandomRoom(GhostType *ghost, HouseType *house, int firstMove);


//ghost 
void initGhost(GhostType **ghost);
void initGhostEvidenceTypeArray(GhostType *ghost);
void freeGhost(GhostType *ghost);

//main function
void* runGhostSimulationThread(void *arg);
void* runHunterSimulationThread(void* arg);
void printResult(HouseType* house);
void identifyGhost(HouseType* house);

//helper in main
int getRandomInRange(int max);


//tester functions
void testEvidenceRemoveAndAdd();
void testMove(HunterType *hunter);