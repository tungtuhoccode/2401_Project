#include "../include/defs.h"

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

void initRoomList(RoomListType *list){
    list->head = NULL;
    list->tail = NULL;
}

void connectRooms(RoomType* firstRoom, RoomType* secondRoom){
    addRoom(&firstRoom->connectedRooms, secondRoom);
    addRoom(&secondRoom->connectedRooms, firstRoom);
}

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

int addHunterToRoom(RoomType *room, HunterType *hunter){
    int curNumbHunter = room->countHunter;
    if(curNumbHunter == NUM_HUNTERS){
        printf("Four hunters are in %s, cannot add more.\n",room->roomName);
        return C_FALSE;
    }
    room->huntersInRoom[curNumbHunter] = hunter;
    room->countHunter++;
    hunter->currentRoom = room;
    return C_TRUE;
}

int removeHunterFromRoom(RoomType *room, HunterType *hunter){
    int curNumbHunter = room->countHunter;

    //case: there is only 1 element
    if(curNumbHunter == 1){
        //if id match 
        if(strcmp(hunter->hunterName, room->huntersInRoom[0]->hunterName) == 0){
            room->countHunter--;
            return C_TRUE;
        }
        else{
            return C_FALSE;
        }
    }

    //case: more than 1 element
    for (int i = 0; i < curNumbHunter; i++){
        if (strcmp(hunter->hunterName, room->huntersInRoom[i]->hunterName) == 0){
            for (int j = i; j < curNumbHunter-1;j++){
                room->huntersInRoom[j] = room->huntersInRoom[j+1];
            }
            room->countHunter--;
            return C_TRUE;
        }
    }
    return C_FALSE;
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

void freeRoomList(RoomListType *list){
    RoomNodeType *currNode = list->head;
    RoomNodeType *prevNode = NULL;
   
    while (currNode != NULL){
        prevNode = currNode; 
        currNode = currNode->next;
        free(prevNode);
    }
}