/*
EXAMPLE PLUGIN FOR SUDPPIPE
READ THE COMMENTS
*/

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
#endif



// when you create a new plugin copy all the above stuff which is usually ever the same
// this is just a dll so if you have programming knowledge you already know everything



// prototypes
int sudp_init(      // returns 0 if ok or any other value if initialization failed
    u_char *data    // this is the data passed with the -L parameter
);                  // it's NULL if the -L option has not been used

int sudp_pck(       // returns the length of the modified packet
    u_char *data,   // the packet
    int len         // the size of the packet
);

int sudp_vis(       // returns the length of the modified packet
    u_char *data,   // the packet
    int len         // the size of the packet
);



static int  xor;                                            // global variables
                                                            // static is probably better



int sudp_init(u_char *data) {
    printf("- PLUGIN: XOR example (not for real usage)\n"); // plugin name

    if(data && !data[0]) {                                  // help
        printf("- no plugin's options\n");
        return(1);                                          // return different than 0!
    }

    xor = 0x80;                                             // initialization

    return(0);                                              // 0 = ok, other = failed
}



int sudp_pck(u_char *data, int len) {             // this is used for modifying packets
    int     i;                                              // for example you can use this
                                                            // function to replace a specific
    for(i = 0; i < len; i++) data[i] ^= xor;                // text string with another one
    return(len);
}



int sudp_vis(u_char *data, int len) {             // this is used only for visualization
    int     i;                                              // in short the packet is modified
                                                            // after having sent it to the other
    for(i = 0; i < len; i++) data[i] ^= xor;                // end points and so only the visualization
    return(len);                                            // function will show the new content
}


