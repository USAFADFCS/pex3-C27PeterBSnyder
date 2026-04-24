/** pagequeue.c
 * ===========================================================
 * Name: Brody Snyder, 23 April 2026
 * Section: CS483 / M4
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue *pq = malloc(sizeof(PageQueue));
    if (pq == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    pq->maxSize = maxSize;

    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode *current = pq->tail;
    long depth = 0;

    // Search from tail to head
    while (current != NULL) {
        if (current->pageNum == pageNum) {
            // HIT path

            // Nothing to move
            if (current == pq->tail) {
                return 0;
            }

            PqNode *prevNode = current->prev;
            PqNode *nextNode = current->next;

            if (current == pq->head) {
                pq->head = nextNode;
                pq->head->prev = NULL;
            } else {
                prevNode->next = nextNode;
                nextNode->prev = prevNode;
            }

            // Insert current back at tail
            current->prev = pq->tail;
            current->next = NULL;
            pq->tail->next = current;
            pq->tail = current;

            return depth;
        }

        current = current->prev;
        depth++;
    }
    
    // MISS path

    // Allocate new node and place at tail
    PqNode *newNode = malloc(sizeof(PqNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->pageNum = pageNum;
    newNode->next = NULL;

    if (pq->size == 0) {
        newNode->prev = NULL;
        pq->head = newNode;
        pq->tail = newNode;
    } else {
        newNode->prev = pq->tail;
        pq->tail->next = newNode;
        pq->tail = newNode;
    }

    pq->size++;

    // Checks still under max size and rids the head if so
    if (pq->size > pq->maxSize) {
        PqNode *oldHead = pq->head;
        pq->head = pq->head->next;
        pq->head->prev = NULL;
        free(oldHead);
        pq->size--;
    }

    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode *current = pq->head;
    PqNode *next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}
