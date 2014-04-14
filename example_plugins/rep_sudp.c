#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
    #include <windows.h>
    typedef unsigned char   u_char;
    typedef unsigned short  u_short;
    typedef unsigned int    u_int;
    typedef unsigned long   u_long;

    BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved) {
        return(TRUE);
    }
    BOOL WINAPI _DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved) {
        return(TRUE);
    }
#else
    // gcc -shared -fPIC -o FILE.SO FILE.C
    #define strnicmp    strncasecmp
#endif



static int      original_len,
                modified_len;
static u_char   *original,
                *modified;



const static char   help[] =
            "this plugin requires two text strings separated by a comma\n"
            "Use -L original,modified for substituiting all the occurrences of the string\n"
            "\"original\" in the packet with \"modified\".\n"
            "You can also use -L original,new so will be replaced only the first 3 chars\n"
            "The comparison is case insensitive so ORIGINAL is the same of original\n";



int sudp_init(u_char *data) {
    u_char  *p;

    printf("- PLUGIN: String replace plugin\n");

    if(!data) {
        printf("\nError: %s\n", help);
        return(1);
    }
    if(data && !data[0]) {
        printf("- %s\n", help);
        return(1);
    }

    p = strchr(data, ',');
    if(!p) {
        printf("\nError: no comma found between the two text strings\n");
        return(1);
    }
    *p = 0;

    original = strdup(data);
    modified = strdup(p + 1);

    original_len = strlen(original);
    modified_len = strlen(modified);

    return(0);
}



int sudp_pck(u_char *data, int len) {
    u_char  *l;

    for(l = data + len - 1; data < l;) {
        if(!strnicmp(data, original, original_len)) {
            memcpy(data, modified, modified_len);
            data += modified_len;
        } else {
            data++;
        }
    }
    return(len);
}



int sudp_vis(u_char *data, int len) {
    return(len);
}


