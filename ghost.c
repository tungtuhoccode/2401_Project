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

void freeGhost(GhostType *ghost){
    free(ghost);
}