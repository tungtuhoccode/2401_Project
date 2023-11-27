#include "defs.h"

int addHunterToRoom(RoomType *room, HunterType *hunter){
    //remember to lock and unlock  
    int curNumbHunter = room->countHunter;
    if(curNumbHunter == NUM_HUNTERS){
        printf("Four hunters are in %s, cannot add more\n",room->roomName);
        return C_FALSE;
    }
    room->huntersInRoom[curNumbHunter] = hunter;
    room->countHunter++;
    hunter->currentRoom = room;
    // printf("added hunter to room %s\n", hunter->currentRoom->roomName);
    return C_TRUE;
}

int checkHunterInRoom(RoomType *room){
    //lock the room so nothing can modify it
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
    //remember to lock and unlock
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

void leaveEvidence(RoomType *room, GhostType *ghost){
    EvidenceStructType *newEv;
    int randomEvidenceIndex = getRandomInRange(3);
    initEvidence(&newEv, ghost->ghostEvidenceTypes[randomEvidenceIndex]);
    addEvidenceToList(&room->roomEvList, newEv);
}

int checkGhostInRoom(RoomType *room){
    //lock the room so nothing can modify it
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

void collectEvidence(HunterType* hunter, EvidenceListType* evidencesInRoom){
    //remember to lock and unlock 
    EvidenceNodeType* currNode = evidencesInRoom->head;

    while(currNode != NULL){
        if(currNode->data->evidenceType == hunter->hunterEquipmentType){
            addEvidenceToList(hunter->sharedEvList, currNode->data);
            removeEvidenceFromList(evidencesInRoom, currNode->data);
            break;
        }
        currNode = currNode->next;
    }
}

void moveHunter(HunterType* hunter, RoomListType* connectedRooms, int firstMove){
    //move first: moveHunter(true)
    //move normal: moveHunter(false)
    while(C_TRUE){
        int hunterAdded = C_FALSE;

        RoomNodeType *currNode = connectedRooms->head;
        while(currNode != NULL){
            RoomType *currRoom = currNode->data;
            
            if(firstMove == C_TRUE){
                // printf("First move\n");
                if( strcmp(currRoom->roomName, "Van") == 0){
                    // printf("van move");
                    currNode = currNode->next;
                    continue;
                }
            }

            //add ghost to a room randomly
            int rand = randInt(0, 2);
            if(rand == 0 ){
                RoomType *newRoom = currRoom;
                RoomType *oldRoom = hunter->currentRoom;
                printf("%s is currently in  (%s)\n",hunter->hunterName, hunter->currentRoom->roomName);
                printf("Trying to move to  (%s)\n",newRoom->roomName);
                
                //CHANGE CODE TO AVOID DEADLOCK
                sem_wait(&oldRoom->room_mutex);
                sem_wait(&newRoom->room_mutex);

                removeHunterFromRoom(oldRoom, hunter);
                addHunterToRoom(newRoom, hunter);

                sem_post(&oldRoom->room_mutex);
                sem_post(&newRoom->room_mutex);

                // sem_wait(&hunter->currentRoom->room_mutex);
                // removeHunterFromRoom(hunter->currentRoom, hunter);
                // sem_post(&hunter->currentRoom->room_mutex);
                
                // sem_wait(&newRoom->room_mutex);
                // addHunterToRoom(newRoom, hunter);
                // sem_post(&newRoom->room_mutex);

                hunterAdded = C_TRUE;

                l_hunterMove(hunter->hunterName, currRoom->roomName);
                 printf("\n");
                break;
            }
            currNode = currNode->next;
        }

        //if hunter is succesfully added
        if(hunterAdded) break;
    }
}

void reviewEvidence(HunterType* hunter, EvidenceListType* sharedEvList){
    
    EvidenceNodeType* currNode = sharedEvList->head;
    int evArr[EV_COUNT]= {-1, -1, -1, -1};
    int count = 0;

    while (currNode != NULL){
        if(evArr[currNode->data->evidenceType] == -1){
            evArr[currNode->data->evidenceType] = 1;
            count ++;
        }
        if(count == 3){
            break;
        }
        currNode = currNode->next;
    }
}