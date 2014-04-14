/* Replaces all occurrences of FROM with TO in a UDP packet.
 *
 * Based on example_plugins/rep_sudp.c, with the following differences:
 *
 * 1) Comparisons are case-sensitive.
 * 2) FROM and TO must be the same length.
 * 3) No win32 support.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int original_len;           /* same as modified_len */
static const char *original;
static const char *modified;


const static char help[] = 
   "USAGE: This plugin requires two text strings separated by a comma.\n"
   "Pass -L <original>,<modified> to sudppipe to substitute all occurrences\n"
   "of <original> in the packet with <modified>.\n"
   "   - This comparison is case-sensitive.\n"
   "   - <original> and <modified> must have the same length.\n";


int sudp_init(char *data) {
    char *p;

    printf("- PLUGIN: String replace plugin\n");
    if (!data) {
        printf("\nFATAL ERROR: no -L flag specified.\n%s\n", help);
        return 1;
    }
    if (!data[0]) {
        printf("\nFATAL ERROR: -L flag has empty value.\n%s\n", help);
        return 1;
    }

    p = strchr(data, ',');
    if (!p) {
        printf("\nFATAL ERROR: no comma found in argument to -L (%s).\n%s\n",
               data, help);
        return 1;
    }
    *p = '\0';

    original = strdup(data);
    modified = strdup(p + 1);
    original_len = strlen(original);

    if (strlen(original) != strlen(modified)) {
        printf("\nFATAL ERROR: length of original (%s) and modified (%s)"
               " do not match.\n%s\n",
               original, modified, help);
        return 1;
    }

    return 0;
}


int sudp_pck(char *data, int len) {
    char *p = data;

    while ( (p=strstr(p, original)) ) {
       memcpy(p, modified, original_len);
       p += original_len;
    }
    return len;
}


int sudp_vis(u_char *data, int len) {
    return len;
}


