/*
Zdaemon huffman 0.1
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    http://aluigi.altervista.org

  source code from Doom (X-Doom/old Zdaemon code) with modified HuffFreq for Zdaemon

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <ctype.h>
#include <math.h>

#define MAX_UDP_PACKET  1400    // modify it if you want

int LastCompMessageSize = 0;

typedef struct
{
    struct huffnode_s *zero;
    struct huffnode_s *one;
    unsigned char val;
    float freq;
} huffnode_s, huffnode_t;

typedef struct
{
    unsigned int bits;
    int len;
} hufftab_t;
static huffnode_t *HuffTree=0;
static hufftab_t HuffLookup[256];

static float HuffFreq[256]=
{
    0.46947443, 0.03744229, 0.02009356, 0.00707818, 
    0.00728153, 0.02111485, 0.00445556, 0.00229655, 
    0.01265820, 0.00241106, 0.00299206, 0.00265591, 
    0.00284209, 0.00339483, 0.00203944, 0.00192218, 
    0.01423221, 0.00194756, 0.00250942, 0.00224109, 
    0.00264493, 0.00185304, 0.00164660, 0.00135765, 
    0.00238693, 0.00155698, 0.00132108, 0.00136161, 
    0.00168719, 0.00133527, 0.00138385, 0.00133988, 
    0.00255865, 0.00167663, 0.00142282, 0.00185067, 
    0.00179926, 0.00132452, 0.00107309, 0.00524015, 
    0.00246438, 0.00096060, 0.00734912, 0.00102914, 
    0.00170869, 0.00128854, 0.00097614, 0.00113916, 
    0.00186316, 0.01077324, 0.00102875, 0.00354735, 
    0.00125545, 0.00114088, 0.00103111, 0.00132834, 
    0.00206495, 0.00107727, 0.00099150, 0.00454583, 
    0.00144284, 0.00102763, 0.00105168, 0.00117436, 
    0.00197808, 0.00103239, 0.00325006, 0.00115212, 
    0.00125261, 0.00100999, 0.00100125, 0.00112827, 
    0.00129156, 0.00098112, 0.00095781, 0.00098035, 
    0.00117501, 0.00103275, 0.00111885, 0.00110044, 
    0.00820434, 0.00091584, 0.00098619, 0.00089814, 
    0.00135214, 0.00088258, 0.00095421, 0.00088725, 
    0.00213761, 0.00099426, 0.00102234, 0.00098324, 
    0.00116107, 0.00090388, 0.00085447, 0.00094951, 
    0.00157389, 0.00110302, 0.00089621, 0.00094432, 
    0.00813035, 0.00961901, 0.00476472, 0.00097480, 
    0.00135165, 0.00093732, 0.00086517, 0.00092980, 
    0.00543320, 0.00116154, 0.00099676, 0.00192325, 
    0.00154463, 0.00087127, 0.00106448, 0.00113199, 
    0.00343841, 0.00469290, 0.00088090, 0.00102023, 
    0.00131824, 0.00091547, 0.00088527, 0.00090882, 
    0.00113292, 0.00088162, 0.00088137, 0.00112587, 
    0.00328156, 0.00088004, 0.00102551, 0.00086177, 
    0.00107408, 0.00116912, 0.00084356, 0.00081423, 
    0.00125029, 0.00079219, 0.00079757, 0.00089434, 
    0.00107415, 0.00083609, 0.00078779, 0.00084491, 
    0.00148952, 0.00085361, 0.00079014, 0.00082267, 
    0.00103773, 0.00083512, 0.00081632, 0.00081653, 
    0.00123191, 0.00079775, 0.00080431, 0.00081823, 
    0.00122765, 0.00080738, 0.00080918, 0.00084970, 
    0.00235248, 0.00079080, 0.00078659, 0.00081687, 
    0.00107209, 0.00078719, 0.00078745, 0.00097451, 
    0.00122689, 0.00078703, 0.00079399, 0.00076628, 
    0.00119746, 0.00076251, 0.00079380, 0.00086241, 
    0.00132236, 0.00077717, 0.00087071, 0.00080988, 
    0.00099657, 0.00075881, 0.00077917, 0.00080354, 
    0.00112704, 0.00078615, 0.00078358, 0.00077879, 
    0.00109891, 0.00095691, 0.00080671, 0.00079289, 
    0.00244628, 0.00080076, 0.00075714, 0.00079463, 
    0.00122710, 0.00075707, 0.00073999, 0.00079447, 
    0.00113160, 0.00095707, 0.00075825, 0.00074562, 
    0.00105562, 0.00075720, 0.00079450, 0.00360570, 
    0.00135354, 0.00077605, 0.00076658, 0.00081708, 
    0.00138878, 0.00079424, 0.00083277, 0.00076214, 
    0.00117659, 0.00598942, 0.00082706, 0.00081629, 
    0.00132374, 0.00075337, 0.00077039, 0.00113487, 
    0.00163245, 0.00090353, 0.00086913, 0.00091947, 
    0.00125829, 0.00079054, 0.00079913, 0.00082155, 
    0.00124808, 0.00079468, 0.00126046, 0.00086568, 
    0.00130345, 0.00104526, 0.00107681, 0.00118829, 
    0.00178076, 0.00132801, 0.00150205, 0.00155763, 
    0.00225596, 0.00221831, 0.00161586, 0.00166614, 
    0.00217488, 0.00195445, 0.00243183, 0.00258366, 
    0.00355952, 0.00450725, 0.00615087, 0.02669448
};

void I_FatalError (const char *error, ...)
{
    va_list argptr;
    va_start (argptr, error);
    vprintf (error, argptr);
    va_end (argptr);
    exit(-1);
}

static void FindTab(huffnode_t *tmp,int len,unsigned int bits)
{
    if(!tmp)
        I_FatalError("no huff node");
    if (tmp->zero)
    {
        if(!tmp->one)
            I_FatalError("no one in node");
        if(len>=32)
            I_FatalError("compression screwd");
        FindTab((huffnode_t *)tmp->zero,len+1,bits<<1);
        FindTab((huffnode_t *)tmp->one,len+1,(bits<<1)|1);
        return;
    }
    HuffLookup[tmp->val].len=len;
    HuffLookup[tmp->val].bits=bits;
}

static unsigned char Masks[8]=
{
    0x01,   0x02,   0x04,   0x08,
    0x10,   0x20,   0x40,   0x80
};

static void PutBit(unsigned char *buf,unsigned pos,unsigned bit)
{
    if (bit)
        buf[pos >> 3] |= Masks[pos & 7];
    else
        buf[pos >> 3] &= ~Masks[pos & 7];
}

static unsigned GetBit(unsigned char *buf,unsigned pos)
{
    return ( buf[pos >> 3] & Masks[pos & 7] );
}

static void BuildTree(float *freq)
{
    float min1,min2;
    int i,j,minat1,minat2;
    huffnode_t *work[256];  
    huffnode_t *tmp;    

    for (i=0;i<256;i++)
    {
        work[i]=(huffnode_s *)malloc(sizeof(huffnode_t));
        work[i]->val=(unsigned char)i;
        work[i]->freq=freq[i];
        work[i]->zero=0;
        work[i]->one=0;
        HuffLookup[i].len=0;
    }
    for (i=0;i<255;i++)
    {
        minat1=-1;
        minat2=-1;
        min1=1E30;
        min2=1E30;
        for (j=0;j<256;j++)
        {
            if (!work[j])
                continue;
            if (work[j]->freq<min1)
            {
                minat2=minat1;
                min2=min1;
                minat1=j;
                min1=work[j]->freq;
            }
            else if (work[j]->freq<min2)
            {
                minat2=j;
                min2=work[j]->freq;
            }
        }
        if (minat1<0)
            I_FatalError("minatl: %d",minat1);
        if (minat2<0)
            I_FatalError("minat2: %d",minat2);
        
        tmp = (huffnode_s *)malloc(sizeof(huffnode_t));
        tmp->zero=(void *)work[minat2];
        tmp->one=(void *)work[minat1];
        tmp->freq=work[minat2]->freq+work[minat1]->freq;
        tmp->val=0xff;
        work[minat1]=tmp;
        work[minat2]=0;
    }       
    HuffTree=tmp;
    FindTab(HuffTree,0,0);
}

void HuffDecode(unsigned char *in,unsigned char *out,int inlen,int *outlen)
{
    int             bits,nbits;
    huffnode_t      *tmp;   
    unsigned char   *pout, *plast;

    pout = out;
    plast = out + 8*(MAX_UDP_PACKET+32)-1;

    if (*in==0xff)
    {
        if (inlen>1)
            memcpy(out,in+1,inlen-1);
        *outlen = inlen-1;
        return;
    }

    nbits = (inlen-1)*8 - (int)(unsigned) *in++;
    for (bits=0;  bits<nbits;   )
    {
        tmp = HuffTree;
        do
        {
            tmp = (GetBit(in,bits)) ? (huffnode_s *)tmp->one : (huffnode_s *)tmp->zero;
            bits++;
        }
        while (tmp->zero);
        *pout++ = tmp->val;
        if (pout>=plast)
        {
            printf("HuffDecode: overflow\n");
            break;
        }
    }

    *outlen = (int)(pout - out);
}

void HuffEncode(unsigned char *in,unsigned char *out,int inlen,int *outlen)
{
    int i,j,bitat;
    unsigned int t;
    bitat=0;
    for (i=0;i<inlen;i++)
    {
        t=HuffLookup[in[i]].bits;
        for (j=0;j<HuffLookup[in[i]].len;j++)
        {
            PutBit(out+1,bitat+HuffLookup[in[i]].len-j-1,t&1);
            t>>=1;
        }
        bitat+=HuffLookup[in[i]].len;
    }
    *outlen=1+(bitat+7)/8;
    *out=8*((*outlen)-1)-bitat;
	if(*outlen >= inlen+1)
	{
		*out=0xff;
		memcpy(out+1,in,inlen);
		*outlen=inlen+1;
	}
}

void HuffInit(void)
{
    BuildTree(HuffFreq);
}

