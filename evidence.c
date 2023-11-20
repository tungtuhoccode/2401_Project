#include "defs.h"

void initEvidenceList(EvidenceListType* evList){
    evList->head = NULL;
    evList->tail = NULL;
}

void addEvidenceToList(EvidenceListType *list, EvidenceStructType *evidence){
    // NodeType *listTail = list->tail;
    // NodeType *listHead = list->head;
    
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