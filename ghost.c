#include "defs.h"

//leave evidence

//move 

//init ghost with a random ghost value
void initGhost(GhostType **ghost){
    GhostType *newGhost = calloc(1, sizeof(GhostType));

    newGhost->ghostClass = getRandomInRange(GHOST_COUNT);
    newGhost->inRoom = NULL;
    newGhost->boredomTimer = 0;
    initGhostEvidenceTypeArray(newGhost);
    initEvidenceList(&newGhost->allEvidenceInHouseList);
    (*ghost) = newGhost;
}

void initGhostEvidenceTypeArray(GhostType *ghost){
    switch (ghost->ghostClass) {
        case POLTERGEIST:
            ghost->ghostEvidenceTypes[0] = EMF;
            ghost->ghostEvidenceTypes[1] = TEMPERATURE;
            ghost->ghostEvidenceTypes[2] = FINGERPRINTS;
            break;
        case BANSHEE:
            ghost->ghostEvidenceTypes[0] = EMF;
            ghost->ghostEvidenceTypes[1] = TEMPERATURE;
            ghost->ghostEvidenceTypes[2] = SOUND;
            break;
        case BULLIES:
            ghost->ghostEvidenceTypes[0] = EMF;
            ghost->ghostEvidenceTypes[1] = FINGERPRINTS;
            ghost->ghostEvidenceTypes[2] = SOUND;
            break;
        case PHANTOM:
            ghost->ghostEvidenceTypes[0] = TEMPERATURE;
            ghost->ghostEvidenceTypes[1] = FINGERPRINTS;
            ghost->ghostEvidenceTypes[2] = SOUND;
            break;
        default:
            ghost->ghostEvidenceTypes[0] = ghost->ghostEvidenceTypes[1] = ghost->ghostEvidenceTypes[2] = GH_UNKNOWN;
            break;
    }
}

void placeGhostInRandomRoom(GhostType *ghost, HouseType *house, int firstMove){
    while(C_TRUE){
        int ghostAdded = C_FALSE;

        RoomNodeType *currNode = house->rooms.head;
        while(currNode != NULL){

            RoomType *currRoom = currNode->data;

            if(firstMove == C_TRUE){
                if(strcmp(currRoom->roomName, "Van") == 0){
                    currNode = currNode->next;
                    continue;
                }
            }
            
            //add ghost to a room randomly
            int rand = getRandomInRange(2);
            if(rand == 0){
                ghost->inRoom = currRoom;
                currRoom->roomGhost = ghost;
                ghostAdded = C_TRUE;
                break;
            }
            currNode = currNode->next;
        }

        //if ghost is succesfully added
        if(ghostAdded) break;
    }
}



void freeGhost(GhostType *ghost){
    free(ghost);
}