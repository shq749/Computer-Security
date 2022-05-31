#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

// Name: Sihui Qi
// netID: sq77
// RUID: 187002625
// My code for readdir goes here

#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

struct dirent *(*real_readdir)(DIR *);

struct dirent *readdir(DIR *dirp) {
    real_readdir = dlsym(RTLD_NEXT, "readdir");
    struct dirent * pDirEnt = (*real_readdir)(dirp);
    char * szHidden = getenv("HIDDEN");
    while(pDirEnt != NULL) {
        if(szHidden == NULL) break;
        char *pFind = strstr(szHidden, pDirEnt->d_name);
        if(pFind == NULL) break;
        char cEnding = *(pFind+strlen(pDirEnt->d_name));
        if(cEnding != ':' && cEnding != '\0') break;
        pDirEnt = (*real_readdir)(dirp);
    }
    return pDirEnt;
}