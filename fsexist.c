#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <cutils/log.h>

#include "fsexist.h"

bool has_filesystem(const char *str) {
    FILE *fptr;
    bool ret = false;
    int size = strlen(str) + 3;
    char buffer[size];
    char *fstype = malloc(size);
    strcpy(fstype, "\t");
    strcat(fstype, str);
    strcat(fstype, "\n");
    buffer[size - 1] = fstype[size - 1];
    fptr = fopen("/proc/filesystems", "r");
    if (fptr == NULL)
    {
        SLOGE("Unable to open /proc/filesystems (%s)", strerror(errno));
        goto exit;
    }
    signed char c = fgetc(fptr);
    while (c != EOF)
    {
        int i;
        for (i = 0;i < size - 2;i ++)
            buffer[i] = buffer[i + 1];
        buffer[size - 2] = (char)c;
        c = fgetc(fptr);
        if (!strcmp(buffer, fstype))
        {
            ret = true;
            SLOGE("%s kernel space support detected!", str);
        }
    }
exit:
    fclose(fptr);
    free(fstype);
    return ret;
} 
