#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "the3.h"


int main(int argc, char** argv)
{
    Cache* cache;
    Media media1;

    /* Create the cache */
    cache = createCache();

    /* Add new media */
    media1.name = "newVideo";
    media1.size = 300;

    addMediaRequest(cache, media1, "tr");

    deleteMedia(cache, "video2");

    printCache(cache);
    
    return 0;
}
