#include "defs.h"

RoomType* createRoom(char* roomNameIn){

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
    sem_init(&newRoom->room_mutex, 0,1);

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
        freeEvidenceList(&currNode->data->roomEvList);
        
        //finally free the room
        free(currNode->data);
        currNode = currNode->next;
    }
}

//1. free evidence
//2. free evidence list
//3. free room
//4. free room list
//5. free hunter
//6. free hunter shared evidence list
//7. free hunter list

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
    if(room->countHunter == 0){
        printf("None in room (%s)\n", room->roomName);
    }
    else
    {
        for(int i = 0;i< room->countHunter;i++){
            printf("Hunter %d: %s\n",(i+1),room->huntersInRoom[i]->hunterName);
        }
    }
    printf("\n\n");
}

