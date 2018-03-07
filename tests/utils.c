#include <string.h>

#include "tests/utils.h"

#include "cii/mem.h"
#include "cii/assert.h"


char *
path_join(const char *path1, const char *path2)
{
    int l;
    int l1;
    char *path;
    
    assert(path1);
    assert(path2);
    assert(path2[0] != '/');

    l1 = strlen(path1);
    l = l1 + strlen(path2) + 10;
    path = CALLOC(l, sizeof(char));
    
    if (path1[l1-1] == '/')
        l1--;

    strncpy(path, path1, l1);
    path[l1] = '/';
    strcat(path, path2);
    
    return path;
}
