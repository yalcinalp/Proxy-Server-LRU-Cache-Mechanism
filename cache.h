#ifndef THE3
#define THE3

typedef struct Media Media;
typedef struct CacheNode CacheNode;
typedef struct Cache Cache;
typedef struct DomainFreqNode DomainFreqNode;
typedef struct DomainFreqList DomainFreqList;

struct DomainFreqNode
{
    char*             name;
    int               freq;
    DomainFreqNode*   prev;
    DomainFreqNode*   next;
};

struct DomainFreqList
{
    DomainFreqNode*   head;
    DomainFreqNode*   tail;
};

struct Media
{
    char* name;
    int   size;
};

struct CacheNode
{  
    CacheNode*       prev;
    CacheNode*       next;
    DomainFreqList*  domainFreqList;  
    Media            media;
};

struct Cache
{
    CacheNode* head;
    CacheNode* tail;
    int        mediaCount;
    int        currentSize;
    int        cacheLimit;
};


/*
    Create a cache by reading inputs from stdin
*/
Cache* createCache();

/*
    Prints cache information and its contents
*/
void printCache(Cache* cache);

/*
    Adds media to the cache
*/
CacheNode* addMediaRequest(Cache* cache, Media media, char* domain);

/*
    Finds the corresponding media's node in the cache
*/
CacheNode* findMedia(Cache* cache, char* name);

/*
    Finds a media from its name and deletes it's node from the cache
*/
void deleteMedia(Cache* cache, char* name);

#endif
