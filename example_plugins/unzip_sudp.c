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



#include <zlib.h>



static z_stream    z;



int sudp_init(u_char *data) {
    int     winbits = -15;

    printf("- PLUGIN: unzip\n");            // plugin name

    if(data && !data[0]) {                  // help
        printf("- support for windowBits field: -15 (default) to -8 and 8 to 15\n");
        return(1);
    }

    if(data) {
        winbits = atoi(data);
    }

    z.zalloc = (alloc_func)0;
    z.zfree  = (free_func)0;
    z.opaque = (voidpf)0;

    if(inflateInit2(&z, winbits)) {
        fputs("\nError: zlib initialization error\n", stdout);
        exit(1);
    }
    return(0);                              // 0 = ok
}



int sudp_pck(u_char *data, int len) {
    return(len);
}



int sudp_vis(u_char *data, int len) {
    static u_char   out[0xffff];

    inflateReset(&z);
    z.next_in   = data;
    z.avail_in  = len;
    z.next_out  = out;
    z.avail_out = sizeof(out);
    inflate(&z, Z_FINISH);

    len = z.total_out;
// NO FINISH    inflateEnd(&z);
    return(len);
}


