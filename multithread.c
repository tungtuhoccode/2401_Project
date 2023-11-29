#include "defs.h"

int addHunterToRoom(RoomType *room, HunterType *hunter){
    int curNumbHunter = room->countHunter;
    if(curNumbHunter == NUM_HUNTERS){
        printf("Four hunters are in %s, cannot add more\n",room->roomName);
        return C_FALSE;
    }
    room->huntersInRoom[curNumbHunter] = hunter;
    room->countHunter++;
    hunter->currentRoom = room;
    return C_TRUE;
}
int checkGhostInRoom(RoomType *room){
    sem_wait(&room->room_mutex);
    
    int result = -1;
    if(room->roomGhost != NULL){
        result = C_TRUE;
    }else{
        result = C_FALSE;
    }
    sem_post(&room->room_mutex);
    return result;
}


int checkHunterInRoom(RoomType *room){
    sem_wait(&room->room_mutex);

    int result = -1;
    if(room->countHunter > 0){
        result = C_TRUE;
    }else{
        result = C_FALSE;
    }
    sem_post(&room->room_mutex);
    return result;
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
    printf("removed hunter from room: %s", room->roomName);
    return C_FALSE;
}

void leaveEvidence(RoomType *room, GhostType *ghost){
    sem_wait(&room->roomEvList.evList_mutex);
    EvidenceStructType *newEv;
    int randomEvidenceIndex = getRandomInRange(3);
    initEvidence(&newEv, ghost->ghostEvidenceTypes[randomEvidenceIndex]);
    addEvidenceToList(&ghost->allEvidenceInHouseList, newEv);

    addEvidenceToList(&room->roomEvList, newEv);
    l_ghostEvidence( ghost->ghostEvidenceTypes[randomEvidenceIndex], room->roomName);
    sem_post(&room->roomEvList.evList_mutex);
}


void collectEvidence(HunterType* hunter, EvidenceListType* evidencesInRoom){

    sem_wait(&hunter->sharedEvList->evList_mutex);
    sem_wait(&evidencesInRoom->evList_mutex);
    EvidenceNodeType* currNode = evidencesInRoom->head;

    while(currNode != NULL){
        if(currNode->data->evidenceType == hunter->hunterEquipmentType){
            l_hunterCollect(hunter->hunterName, currNode->data->evidenceType, hunter->currentRoom->roomName);
            addEvidenceToList(hunter->sharedEvList, currNode->data);
            removeEvidenceFromList(evidencesInRoom, currNode->data);  

            break;
        }
        currNode = currNode->next;
    }

    sem_post(&hunter->sharedEvList->evList_mutex);
    sem_post(&evidencesInRoom->evList_mutex);
}
void moveGhostToAdjacentRoom(GhostType* ghost){ 
    while(C_TRUE){
        int ghostAdded = C_FALSE;
        RoomNodeType *currNode = ghost->inRoom->connectedRooms.head;
        while(currNode != NULL){
            RoomType *currRoom = currNode->data;
        
            //add ghost to a room randomly
            int rand = randInt(0, 2);
            if(rand == 0 ){
                RoomType *newRoom = currRoom;
                RoomType *oldRoom = ghost->inRoom;
                
                sem_wait(&oldRoom->room_mutex);
                sem_wait(&newRoom->room_mutex);

                newRoom->roomGhost = ghost;
                oldRoom->roomGhost = NULL;
                ghost->inRoom = newRoom;

                sem_post(&oldRoom->room_mutex);
                sem_post(&newRoom->room_mutex);

                ghostAdded = C_TRUE;

                
                l_ghostMove(ghost->inRoom->roomName);
                break;
            }
            currNode = currNode->next;
        }

        //if hunter is succesfully added
        if(ghostAdded) break;
    }
}

void moveHunter(HunterType* hunter, RoomListType* connectedRooms, int firstMove){
    while(C_TRUE){
        int hunterAdded = C_FALSE;

        RoomNodeType *currNode = connectedRooms->head;
        while(currNode != NULL){
            RoomType *currRoom = currNode->data;
            
            if(firstMove == C_TRUE){
                if(strcmp(currRoom->roomName, "Van") == 0){
                    currNode = currNode->next;
                    continue;
                }
            }

            //add ghost to a room randomly
            int rand = randInt(0, 2);
            if(rand == 0 ){
                RoomType *newRoom = currRoom;
                RoomType *oldRoom = hunter->currentRoom;
                
                //CHANGE CODE TO AVOID DEADLOCK
                sem_t *mutex[2];
                sem_t *oldRoomMutex = &oldRoom->room_mutex;
                sem_t *newRoomMutex = &newRoom->room_mutex;
                if(oldRoomMutex < newRoomMutex){
                    mutex[0] = oldRoomMutex;
                    mutex[1] = newRoomMutex;
                }else{
                    mutex[0] = newRoomMutex;
                    mutex[1] = oldRoomMutex;
                }

                sem_wait(mutex[0]);
                sem_wait(mutex[1]);
  
                removeHunterFromRoom(oldRoom, hunter);
                addHunterToRoom(newRoom, hunter);

                sem_post(mutex[0]);
                sem_post(mutex[1]);

                hunterAdded = C_TRUE;

                l_hunterMove(hunter->hunterName, currRoom->roomName);
                break;
            }
            currNode = currNode->next;
        }

        //if hunter is succesfully added
        if(hunterAdded) break;
    }
}

int reviewEvidence(HunterType* hunter, EvidenceListType* sharedEvList){
    sem_wait(&hunter->sharedEvList->evList_mutex);
    EvidenceNodeType* currNode = sharedEvList->head;
    int evArr[EV_COUNT]= {-1, -1, -1, -1};
    int count = 0;
    int logType = LOG_INSUFFICIENT;
    while (currNode != NULL){
        if(evArr[currNode->data->evidenceType] == -1){
            evArr[currNode->data->evidenceType] = 1;
            count ++;
        }
        if(count == 3){
            printf("Enough evidence to guess the ghost type\n");
            printEvidenceList(sharedEvList);
            printf("\n");
            logType = LOG_SUFFICIENT;
            break;
        }
        currNode = currNode->next;
    }
    l_hunterReview(hunter->hunterName, logType);
    sem_post(&hunter->sharedEvList->evList_mutex);
    return count;
}