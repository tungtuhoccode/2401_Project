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
    char evidenceName[MAX_STR];
    sem_t evidence_mutex;
    float evidenceValues[4];
};

struct EvidenceNode{
    EvidenceType *data;
    EvidenceNodeType *next;
};

struct EvidenceList{
    EvidenceNodeType *head;
    EvidenceNodeType *tail;
};

//Hunter
struct Hunter {
    RoomType *currentRoom; //pointer to the room they are currently in
    EvidenceStructType hunterEvType;
    char hunterName[MAX_STR];
    EvidenceListType *sharedEvidence;//pointer to collection of evidence
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
    /*
    linked list of room
        - RoomListType
        - RoomNodeType

    linked list of evidence
        - EvidenceListType
        - EvidenceNodeType

    Collection of hunter Linkedlist
        - HunterType
    Pointer to ghost
        - GhostType
    */
   GhostType *roomGhost; //each room has 1 ghost
   RoomListType connectedRooms;
   EvidenceListType roomEvList;
   HunterType* hunters[NUM_HUNTERS];
};





//Ghost
struct Ghost {
    GhostClass ghostClass;
    RoomType *inRoom; 
        //pointer to the room that the ghost is in 
    int boredomTimer;
};

//House
struct House {
    //hunter collections
    //linkedlist all rooms
    //llist evidencelist

    HunterType hunters[NUM_HUNTERS];
    RoomListType rooms;
    EvidenceListType sharedEvList;
};



// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

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
void initEvidenceList(EvidenceListType* evList);
void addEvidenceToList(EvidenceListType *list, EvidenceType *evidence);

//Room functions
RoomType* createRoom(char* roomNameIn);
void initRoomList(RoomListType *list);
void connectRooms(RoomType* firstRoom, RoomType* secondRoom);
void addRoom(RoomListType *list, RoomType *room);

void printRoom(RoomType *room);
void printRoomList(RoomListType *list);

//hunter
void initHuntersArray(HunterType** hunters);
void getHunterName(HunterType **hunters);

//house 
void populateRooms(HouseType* house);

//main function 
