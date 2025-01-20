#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"

char* strdup(const char* s) {
  char* new_str = malloc(strlen(s) + 1);

  if (new_str) {
    strcpy(new_str, s);
  }
  return new_str;
}

Cache* createCache() {
  int i;
  int n;
  char line[256];
  char* token;

  Cache* cache = (Cache*)malloc(sizeof(Cache));

  if (cache == NULL) {
    exit(1);
  }

  scanf("%d %d ", &(cache->cacheLimit), &n);

  cache->mediaCount = n;
  cache->currentSize = 0;
  cache->head = NULL;
  cache->tail = NULL;

  for (i = 0; i < n; i++) {
    CacheNode* node = (CacheNode*)malloc(sizeof(CacheNode));
    fgets(line, sizeof(line), stdin);
    token = (char*)malloc(sizeof(char) * 250);
    token = strtok(line, " ");

    if (node == NULL) {
      exit(1);
    }

    node->media.name = strdup(token);
    token = strtok(NULL, " ");
    node->media.size = atoi(token);
    cache->currentSize += node->media.size;

    node->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));

    if (node->domainFreqList == NULL) {
      exit(1);
    }

    node->domainFreqList->head = NULL;
    node->domainFreqList->tail = NULL;

    while ((token = strtok(NULL, " ")) != NULL) {
      DomainFreqNode* dnsNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));

      DomainFreqNode* current = node->domainFreqList->head;
      DomainFreqNode* previous = NULL;

      if (dnsNode == NULL) {
        exit(1);
      }

      dnsNode->name = strdup(token);
      token = strtok(NULL, " ");
      dnsNode->freq = atoi(token);

      while (current != NULL && (current->freq > dnsNode->freq ||
                                 (current->freq == dnsNode->freq &&
                                  strcmp(current->name, dnsNode->name) < 0))) {
        previous = current;
        current = current->next;
      }

      dnsNode->prev = previous;
      dnsNode->next = current;

      if (previous != NULL) {
        previous->next = dnsNode;
      }

      else {
        node->domainFreqList->head = dnsNode;
      }

      if (current != NULL) {
        current->prev = dnsNode;
      }

      else {
        node->domainFreqList->tail = dnsNode;
      }
    }

    node->prev = cache->tail;
    node->next = NULL;

    if (cache->head == NULL) {
      cache->head = node;
    }

    else {
      cache->tail->next = node;
    }

    cache->tail = node;
  }

  return cache;
}

void printCache(Cache* cache) {
  CacheNode* node = cache->head;

  if (cache->head == NULL) {
    printf("The Cache is Empty\n");
    return;
  }

  printf("-------------- Cache Information --------------\n");
  printf("Cache Limit: %d KB\n", cache->cacheLimit);
  printf("Current Size: %d KB\n", cache->currentSize);
  printf("Media Count: %d\n", cache->mediaCount);
  printf("Cache Media Info:\n");

  while (node != NULL) {
    DomainFreqNode* dnsNode = node->domainFreqList->head;

    printf("\tName: %s\tSize: %d KB\n", node->media.name, node->media.size);
    printf("\t------- Media Domain Frequencies -------\n");

    while (dnsNode != NULL) {
      printf("\t\tName: %s\tFreq: %d\n", dnsNode->name, dnsNode->freq);
      dnsNode = dnsNode->next;
    }

    printf("\t----------------------------------------\n");
    node = node->next;
  }

  printf("----------------------------------------------\n");
}

CacheNode* addMediaRequest(Cache* cache, Media media, char* domain) {
  CacheNode* node = findMedia(cache, media.name);

  if (node != NULL) {
    DomainFreqNode* dnsNode = node->domainFreqList->head;

    while (dnsNode != NULL) {
      DomainFreqNode* current;

      if (strcmp(dnsNode->name, domain) == 0) {
        dnsNode->freq++;

        if (dnsNode->prev != NULL) {
          dnsNode->prev->next = dnsNode->next;
        }

        else {
          node->domainFreqList->head = dnsNode->next;
        }

        if (dnsNode->next != NULL) {
          dnsNode->next->prev = dnsNode->prev;
        }

        else {
          node->domainFreqList->tail = dnsNode->prev;
        }

        current = node->domainFreqList->head;

        while (current != NULL &&
               (current->freq > dnsNode->freq ||
                (current->freq == dnsNode->freq &&
                 strcmp(current->name, dnsNode->name) < 0))) {
          current = current->next;
        }

        if (current == NULL) {
          dnsNode->prev = node->domainFreqList->tail;
          dnsNode->next = NULL;
          node->domainFreqList->tail->next = dnsNode;
          node->domainFreqList->tail = dnsNode;

        }

        else if (current->prev == NULL) {
          dnsNode->prev = NULL;
          dnsNode->next = current;
          current->prev = dnsNode;
          node->domainFreqList->head = dnsNode;
        }

        else {
          dnsNode->prev = current->prev;
          dnsNode->next = current;
          current->prev->next = dnsNode;
          current->prev = dnsNode;
        }

        break;
      }

      dnsNode = dnsNode->next;
    }

    if (dnsNode == NULL) {
      dnsNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));

      if (dnsNode == NULL) {
        exit(1);
      }

      dnsNode->name = strdup(domain);
      dnsNode->freq = 1;
      dnsNode->prev = NULL;
      dnsNode->next = NULL;

      if (node->domainFreqList->head == NULL) {
        node->domainFreqList->head = dnsNode;
        node->domainFreqList->tail = dnsNode;

      }

      else {
        DomainFreqNode* current = node->domainFreqList->head;

        while (current != NULL &&
               (current->freq > dnsNode->freq ||
                (current->freq == dnsNode->freq &&
                 strcmp(current->name, dnsNode->name) < 0))) {
          current = current->next;
        }

        if (current == NULL) {
          dnsNode->prev = node->domainFreqList->tail;
          node->domainFreqList->tail->next = dnsNode;
          node->domainFreqList->tail = dnsNode;
        }

        else if (current->prev == NULL) {
          dnsNode->next = current;
          current->prev = dnsNode;
          node->domainFreqList->head = dnsNode;
        }

        else {
          dnsNode->prev = current->prev;
          dnsNode->next = current;
          current->prev->next = dnsNode;
          current->prev = dnsNode;
        }
      }
    }

    if (cache->head == node) {
      // Do nothing
    }

    else {
      if (node->prev != NULL) {
        node->prev->next = node->next;
      }

      if (node->next != NULL) {
        node->next->prev = node->prev;
      }

      if (node == cache->tail) {
        cache->tail = node->prev;
      }

      node->prev = NULL;
      node->next = cache->head;

      cache->head->prev = node;
      cache->head = node;
    }
  }

  else {
    DomainFreqNode* dnsNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
    node = (CacheNode*)malloc(sizeof(CacheNode));

    if (node == NULL) {
      exit(1);
    }

    node->media.name = strdup(media.name);
    node->media.size = media.size;
    node->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));

    if (node->domainFreqList == NULL) {
      exit(1);
    }

    node->domainFreqList->head = NULL;
    node->domainFreqList->tail = NULL;

    if (dnsNode == NULL) {
      exit(1);
    }

    dnsNode->name = strdup(domain);
    dnsNode->freq = 1;
    dnsNode->prev = NULL;
    dnsNode->next = NULL;

    node->domainFreqList->head = dnsNode;
    node->domainFreqList->tail = dnsNode;

    while (cache->currentSize + node->media.size > cache->cacheLimit) {
      CacheNode* last_node = cache->tail;

      if (last_node == NULL) {
        break;
      }

      cache->tail = last_node->prev;

      if (cache->tail != NULL) {
        cache->tail->next = NULL;
        cache->currentSize -= last_node->media.size;
        cache->mediaCount--;

      }

      else {
        cache->head = NULL;
        cache->currentSize -= last_node->media.size;
        cache->mediaCount--;
      }
    }

    node->prev = NULL;
    node->next = cache->head;

    if (cache->head != NULL) {
      cache->head->prev = node;
    }

    cache->head = node;

    if (cache->tail == NULL) {
      cache->tail = node;
    }

    cache->currentSize += node->media.size;
    cache->mediaCount++;

    return node;
  }
}

CacheNode* findMedia(Cache* cache, char* name) {
  CacheNode* node = cache->head;

  while (node != NULL) {
    if (strcmp(node->media.name, name) == 0) {
      return node;
    }

    node = node->next;
  }

  return NULL;
}

void deleteMedia(Cache* cache, char* name) {
  CacheNode* node = cache->head;
  DomainFreqNode* dnsNode;

  while (node != NULL) {
    if (strcmp(node->media.name, name) == 0) {
      if (node->prev != NULL) {
        node->prev->next = node->next;
      }

      else {
        cache->head = node->next;
      }

      if (node->next != NULL) {
        node->next->prev = node->prev;
      }

      else {
        cache->tail = node->prev;
      }

      cache->currentSize -= node->media.size;
      cache->mediaCount--;

      dnsNode = node->domainFreqList->head;

      while (dnsNode != NULL) {
        DomainFreqNode* next = dnsNode->next;

        free(dnsNode->name);
        free(dnsNode);
        dnsNode = next;
      }

      free(node->domainFreqList);
      free(node->media.name);
      free(node);

      return;
    }

    node = node->next;
  }
}
