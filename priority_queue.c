#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <assert.h>

#include "priority_queue.h"

typedef struct Node_t* Node;

struct Node_t
{
    PQElement pq_element;
    PQElementPriority priority;
    struct Node_t* next;

    CopyPQElement copy_element;
    FreePQElement free_element;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;

};


static Node createNewNode(PQElement pq_element, PQElementPriority priority,
                        CopyPQElement copy_element,
                        FreePQElement free_element,
                        CopyPQElementPriority copy_priority,
                        FreePQElementPriority free_priority){
    if(copy_element == NULL || free_element == NULL || copy_priority == NULL || free_priority == NULL || pq_element == NULL || priority == NULL){
        return NULL;
    }   
    
    Node node = (struct Node_t*)malloc(sizeof(struct Node_t));
    if (node == NULL) {
        return NULL;
    }

    node->copy_element = copy_element;
    node->free_element = free_element;
    node->copy_priority = copy_priority;
    node->free_priority = free_priority;

    node->pq_element = copy_element(pq_element);
    node->priority = copy_priority(priority);
    node->next = NULL;

    return node;
}


static Node copyNode(Node node)
{
    if (node == NULL) {
        return NULL;
    }

    Node new_node = createNewNode(node->pq_element, node->priority, node->copy_element, node->free_element, node->copy_priority, node->free_priority);
    if(new_node == NULL){
        return NULL;
    }
    return new_node;
}


void freeNode(Node node){
    if(node != NULL){
        if(node->pq_element != NULL ) {
            node->free_element(node->pq_element);
        }
        if(node->priority != NULL) {
            node->free_priority(node->priority);
        }
        free(node);
    }
}


void freeAllNodes(Node node){
    if(node != NULL){
        if(node->pq_element != NULL) {
            node->free_element(node->pq_element);
        }
        if(node->priority  != NULL) {
            node->free_priority(node->priority);
        }
        if(node->next != NULL) {
            freeAllNodes(node->next);
        }
        free(node);
    }
}


struct PriorityQueue_t {
    Node first_element;  // first node in th elements linked list
    Node iterator;   // the internal iterator
    int size;       // the current number of elements in the set

    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priorities;
};


PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities){
    assert(copy_element != NULL && free_element != NULL && equal_elements != NULL && copy_priority != NULL && free_priority != NULL && compare_priorities != NULL);

    PriorityQueue priority_queue = malloc(sizeof(struct PriorityQueue_t));
    if (priority_queue == NULL) {
        return NULL;
    }

    priority_queue->first_element = NULL;
    priority_queue->size = 0;
    priority_queue->iterator = NULL;

    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;
    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priorities = compare_priorities;

    return priority_queue;
}


void pqDestroy(PriorityQueue queue){
    if (queue == NULL) {
        return;
    }

    pqClear(queue);
    free(queue);
}


PriorityQueue pqCopy(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    queue->iterator = NULL;

    PriorityQueue new_priority_queue = malloc(sizeof(struct PriorityQueue_t));
    if (new_priority_queue == NULL) {
        return NULL;
    }

    // if the original queue is empty - return an empty queue
    if(queue->first_element == NULL){
        new_priority_queue->first_element = NULL;
        new_priority_queue->size = 0;
        new_priority_queue->iterator = NULL;
        return new_priority_queue;
    }

    // set the first node int the queue and initialize the iterators
    new_priority_queue->first_element = copyNode(queue->first_element);
    queue->iterator = queue->first_element;
    new_priority_queue->iterator = new_priority_queue->first_element;

    // set all other nodes
    while(queue->iterator->next != NULL){
        Node temp_next_node = copyNode(queue->iterator->next);
        if(temp_next_node == NULL){
            queue->iterator = NULL;
            new_priority_queue->iterator = NULL;
            return NULL;
        }
        new_priority_queue->iterator->next = temp_next_node;
        new_priority_queue->iterator = new_priority_queue->iterator->next;
        queue->iterator = queue->iterator->next;
    }
    queue->iterator = NULL;
    new_priority_queue->iterator = NULL;
    return new_priority_queue;
}


int pqGetSize(PriorityQueue queue) {
    if (queue == NULL){
        return -1;
    }
    return queue->size;
}


bool pqContains(PriorityQueue queue, PQElement element) {
    if (queue == NULL || element == NULL){
        return false;
    }
    Node temp_iterator = queue->iterator;
    PQ_FOREACH(PQElement, foreach_iterator, queue){
        if (queue->equal_elements(foreach_iterator, element)){
            queue->iterator = temp_iterator;
            return true;
        }
    }
    queue->iterator = temp_iterator;
    return false;
}


PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority) {
    queue->iterator = NULL;
    if (element == NULL || priority == NULL || queue == NULL){
        return PQ_NULL_ARGUMENT;
    }

    Node new_node = createNewNode(element, priority, queue->copy_element, queue->free_element, queue->copy_priority, queue->free_priority);
    if(new_node == NULL){
        return PQ_OUT_OF_MEMORY;
    }
    queue->size++;

    if (queue->first_element == NULL){
        queue->first_element = new_node;
        return PQ_SUCCESS;
    }

    queue->iterator = queue->first_element;
    if(queue->compare_priorities(queue->first_element->priority, new_node->priority) < 0){
        new_node->next = queue->first_element;
        queue->first_element = new_node;
        queue->iterator = NULL;
        return PQ_SUCCESS;
    }

    while(queue->iterator->next != NULL){
        if(queue->compare_priorities(queue->iterator->next->priority, new_node->priority) < 0){
            new_node->next = queue->iterator->next;
            queue->iterator->next = new_node;
            queue->iterator = NULL;
            return PQ_SUCCESS;
        }
        queue->iterator = queue->iterator->next;
    }

    // adds new node to end of queue if it wasn't added already
    queue->iterator->next = new_node;
    queue->iterator = NULL;
    return PQ_SUCCESS;
}


PriorityQueueResult pqChangePriority(PriorityQueue queue,
                                     PQElement element,
                                     PQElementPriority old_priority,
                                     PQElementPriority new_priority){

    if(element == NULL || queue == NULL || old_priority == NULL || new_priority == NULL){
        queue->iterator = NULL;
        return PQ_NULL_ARGUMENT;
    }

    if(queue->equal_elements(queue->first_element->pq_element, element) &&
       queue->compare_priorities(queue->first_element->priority, old_priority) == 0) {
        queue->iterator = queue->first_element->next;
        freeNode(queue->first_element);
        queue->first_element = queue->iterator;
        queue->iterator = NULL;
        return pqInsert(queue, element, new_priority); //returns PQ_SUCCESS if works
    }

    queue->iterator = queue->first_element;
    while(queue->iterator->next != NULL){
        if(queue->equal_elements(queue->iterator->next->pq_element, element) &&
                queue->compare_priorities(queue->iterator->next->priority, old_priority) == 0){
            Node temp = queue->iterator->next;
            queue->iterator->next = queue->iterator->next->next;
            freeNode(temp);
            queue->iterator = NULL;
            return pqInsert(queue, element, new_priority); //returns PQ_SUCCESS if works
        }
        queue->iterator = queue->iterator->next;
    }
    queue->iterator = NULL;
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}


PriorityQueueResult pqRemove(PriorityQueue queue){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT;
    }
    if(queue->first_element == NULL){
        queue->iterator = NULL;
        return PQ_SUCCESS;
    }
    queue->size--;
    queue->iterator = queue->first_element->next;
    queue->free_priority(queue->first_element->priority);
    queue->free_element(queue->first_element->pq_element);
    free(queue->first_element);
    queue->first_element = queue->iterator;
    queue->iterator = NULL;
    return PQ_SUCCESS;
}


PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element){
    if(element == NULL || queue == NULL){
        queue->iterator = NULL;
        return PQ_NULL_ARGUMENT;
    }

    if(queue->equal_elements(queue->first_element->pq_element, element)) {
        queue->iterator = queue->first_element->next;
        freeNode(queue->first_element);
        queue->first_element = queue->iterator;
        queue->iterator = NULL;
        queue->size--;
        return PQ_SUCCESS;
    }
    queue->iterator = queue->first_element;
    while(queue->iterator->next != NULL){
        if(queue->equal_elements(queue->iterator->next->pq_element, element)){
            Node temp_node = queue->iterator->next;
            queue->iterator->next = queue->iterator->next->next;
            freeNode(temp_node);
            queue->iterator = NULL;
            queue->size--;
            return PQ_SUCCESS;
        }
        queue->iterator = queue->iterator->next;
    }
    queue->iterator = NULL;
    return PQ_ELEMENT_DOES_NOT_EXISTS;
}


PQElement pqGetFirst(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }
    queue->iterator = queue->first_element;
    if(queue->first_element == NULL){
        return NULL;
    }
    return queue->first_element->pq_element;
}


PQElement pqGetNext(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }
    if(queue->iterator == NULL){
        return NULL;
    }
    queue->iterator = queue->iterator->next;
    if(queue->iterator == NULL){
        return NULL;
    }
    return queue->iterator->pq_element;
}


PriorityQueueResult pqClear(PriorityQueue queue){
    if(queue == NULL){
        return PQ_NULL_ARGUMENT;
    }

    if(queue->first_element == NULL){
        return PQ_SUCCESS;
    }
    freeAllNodes(queue->first_element);
    queue->first_element = NULL;
    queue->size = 0;
    return PQ_SUCCESS;
}