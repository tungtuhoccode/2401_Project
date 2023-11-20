#include "defs.h"

RoomType* createRoom(char* roomNameIn){
    //    char roomName[MAX_STR]; 
                //Already initilized
    //    GhostType *roomGhost;
    //    RoomListType connectedRooms;
    //    EvidenceListType roomEvList;
    //    HunterType* hunters[NUM_HUNTERS];

    RoomType *newRoom = (RoomType*) calloc(1, sizeof(RoomType));
    //room name
    strcpy(newRoom->roomName, roomNameIn);

    //roomList
    initRoomList(&newRoom->connectedRooms);

    //evidence list
    initEvidenceList(&newRoom->roomEvList);

    //hunter array
    initHuntersArray(newRoom->huntersInRoom);

    //initilize semaphore

    return newRoom;
}

void freeRoomList(RoomListType *list){
    RoomNodeType *currNode = list->head;
    RoomNodeType *prevNode = NULL;
   
    while (currNode != NULL){
        prevNode = currNode; 
        currNode = currNode->next;
        free(prevNode);
    }
}

void freeRoom(RoomListType *list){
    RoomNodeType *currNode = list->head;
   
    while (currNode != NULL){
        freeRoomList(&currNode->data->connectedRooms);
        free(currNode->data);

        currNode = currNode->next;
    }
}
void initRoomList(RoomListType *list){
    list->head = NULL;
    list->tail = NULL;
}

void connectRooms(RoomType* firstRoom, RoomType* secondRoom){
    addRoom(&firstRoom->connectedRooms, secondRoom);
    addRoom(&secondRoom->connectedRooms, firstRoom);
}

void printRoomList(RoomListType *list){
    RoomNodeType *currNode = list->head;
    while(currNode != NULL){
        printRoom(currNode->data);
        currNode = currNode->next;
    }
    printf("\n");
}

void printRoom(RoomType *room){
    printf("room name: %s\n", room->roomName);
}

//free
void addRoom(RoomListType *list, RoomType *room){
    RoomNodeType *newNode =  (RoomNodeType*) calloc(1, sizeof(RoomNodeType));
    newNode->data = room;
    newNode->next = NULL;

    // if size = 0, add to head, head.next = tail, tail = NULL
    if (list->tail == NULL && list->head == NULL){
        list->head = newNode;
    }
    //if size = 1, tail = newNode
    else if(list->head != NULL && list->tail == NULL){
        list->tail = newNode;
        list->head->next = list->tail;
    }
    else{
        list->tail->next = newNode;
        list->tail = list->tail->next;
    }
}

void printHuntersInRoom(RoomType *room){
    printf("\nALl hunters in room (%s) are: \n", room->roomName);
    for(int i = 0;i< room->countHunter;i++){
        printf("Hunter %d: %s\n",(i+1),room->huntersInRoom[i]->hunterName);
    }
    printf("---------------------------\n");
}

//** MULTITHREAD FUNCTION **//
int addHunterToRoom(RoomType *room, HunterType *hunter){
    //remember to lock and unlock  

    int curNumbHunter = room->countHunter;
    if(curNumbHunter == NUM_HUNTERS){
        printf("Four hunters are in room, cannot add more\n");
        return C_FALSE;
    }
    room->huntersInRoom[curNumbHunter] = hunter;
    room->countHunter++;

    return C_TRUE;
}

int removeHunterFromRoom(RoomType *room, HunterType *hunter){
    //remember to lock and unlock
    int curNumbHunter = room->countHunter;
    for(int i=0;i<curNumbHunter;i++){
        if(strcmp(hunter->hunterName, room->huntersInRoom[i]->hunterName)){
            room->huntersInRoom[i] = NULL;
        }
        for(int j=i;j<curNumbHunter-1;j--){

        }
    }
    
}