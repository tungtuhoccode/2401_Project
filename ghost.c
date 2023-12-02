#include "defs.h"


void initGhost(GhostType **ghost){
    GhostType *newGhost = calloc(1, sizeof(GhostType));

    newGhost->ghostClass =  randomGhost();
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
            ghost->ghostEvidenceTypes[0] = ghost->ghostEvidenceTypes[1] = ghost->ghostEvidenceTypes[2] = (EvidenceType) GH_UNKNOWN;
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
            int rand = randInt(0, 2);
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


void identifyGhost(HouseType* house){
    EvidenceNodeType* currNode = house->sharedEvList.head;
    int evArr[EV_COUNT]= {-1, -1, -1, -1};
    int count = 0;

    while (currNode != NULL){
        if(evArr[currNode->data->evidenceType] == -1){
            evArr[currNode->data->evidenceType] = 1;
            count ++;
        }
        if(count == 3){
            printf("It seems that the ghost has been discovered!\n");
            printf("The hunters have won the game!\n");
            char ghost[MAX_STR];
            if(evArr[EMF] > 0 && evArr[TEMPERATURE] > 0 && evArr[FINGERPRINTS] > 0){
                ghostToString(POLTERGEIST, ghost);  
            } else if(evArr[EMF] > 0 && evArr[TEMPERATURE] > 0 && evArr[SOUND] > 0){
                ghostToString(BANSHEE, ghost);
            } else if(evArr[EMF] > 0 && evArr[FINGERPRINTS] > 0 && evArr[SOUND] > 0){
                ghostToString(BULLIES, ghost); 
            } else if(evArr[TEMPERATURE] > 0 && evArr[FINGERPRINTS] > 0 && evArr[SOUND] > 0){
                ghostToString(PHANTOM, ghost);
            }
            printf("Using the evidences they found, they correctly determined that the ghost is a %s.\n", ghost); 
            break;
        }
        currNode = currNode->next;
    }

    printf("The hunters collected the following evidences:\n");
    char ev[MAX_STR];
    for(int i = 0; i < EV_COUNT; i++){
        if(evArr[i] > 0){
            evidenceToString(i, ev);
            printf("* %s\n", ev);
        }
    }
}

void freeGhost(GhostType *ghost){
    free(ghost);
}