#include "defs.h"

void initEvidence(EvidenceStructType **evStruct, EvidenceType evType){
    EvidenceStructType *newNode =  (EvidenceStructType*) calloc(1, sizeof(EvidenceStructType));
    newNode->evidenceType = evType;

    (*evStruct) = newNode;
}
void initEvidenceList(EvidenceListType* evList){
    evList->head = NULL;
    evList->tail = NULL;
    sem_init(&evList->evList_mutex,0,1);
}

void addEvidenceToList(EvidenceListType *list, EvidenceStructType *evidence){
    
    EvidenceNodeType *newNode =  (EvidenceNodeType*) calloc(1, sizeof(EvidenceNodeType));
    newNode->data = evidence;
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

void printEvidenceList(EvidenceListType *list){
    EvidenceNodeType *currNode = list->head;
    printf("(");
    while(currNode != NULL){
        char test[MAX_STR];
        evidenceToString(currNode->data->evidenceType, test);
        printf("%s -> ",test);
        currNode = currNode->next;
    }
    printf(")");
    printf("\n");
}

void removeEvidenceFromList(EvidenceListType *list, EvidenceStructType *evidence){
    if (list->tail == NULL && list->head == NULL){
        return;
    }
    else if(list->head != NULL && list->tail == NULL){
        int evidenceMatch = list->head->data->evidenceType == evidence->evidenceType;
        if(evidenceMatch){
            free(list->head);
            list->head = NULL;
        }
    }
    else {
         
        int removeHead = list->head->data->evidenceType == evidence->evidenceType;
        int removeTail = list->tail->data->evidenceType == evidence->evidenceType;
        if(removeHead){
            EvidenceNodeType *headNode = list->head;
            list->head = list->head->next;
            free(headNode);
        }
        else if(removeTail){
            EvidenceNodeType *curNode = list->head;
            
            while (curNode->next != list->tail){
                curNode= curNode->next;
            }
            curNode->next = NULL;
            free(list->tail);
            list->tail = curNode;
        }
        else {
            EvidenceNodeType *prevNode = list->head;
            EvidenceNodeType *currNode = list->head->next;
            
            while (currNode != NULL){
                if (currNode->data->evidenceType == evidence->evidenceType){
                    prevNode->next = currNode->next;
                    free(currNode);
                    return;
                }

                prevNode = currNode;
                currNode = currNode->next;
            }
        }
    }

}
void freeEvidence(EvidenceListType *list){
    EvidenceNodeType *currNode = list->head;

   
    while (currNode != NULL){
        char str[MAX_STR];
        // evidenceToString(currNode->data->evidenceType, str);
        // printf("%s\n",str);
        free(currNode->data);
        currNode = currNode->next;
    }
}

void freeEvidenceList(EvidenceListType *list){
    EvidenceNodeType *currNode = list->head;
    EvidenceNodeType *prevNode = NULL;
   
    while (currNode != NULL){
        prevNode = currNode; 
        currNode = currNode->next;
        free(prevNode);
    }
}
