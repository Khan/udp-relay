/*

Half-Life packets decoding function 0.1.2
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    http://aluigi.altervista.org


How to use:

mybuff  = buffer containg the packet
bufflen = size of the packet

hldec(mybuff, bufflen);


Now mybuff contains the decoded buffer 8-)


    Copyright 2004 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl.txt

*/



void hldec(unsigned char *buff, unsigned long pcksz) {
    #define HL_NTOHL(x)  \
       ((((x) & 0xff000000) >> 24) | \
        (((x) & 0x00ff0000) >>  8) | \
        (((x) & 0x0000ff00) <<  8) | \
        (((x) & 0x000000ff) << 24))

    const static unsigned char  hlhash[] =
                    "\x05\x61\x7A\xED\x1B\xCA\x0D\x9B\x4A\xF1\x64\xC7\xB5\x8E\xDF\xA0";
    unsigned char   *ptrebpc;
    unsigned long   *lbuff = (unsigned long *)buff,
                    pcknum,
                    invnum,
                    ebpc;
    int             cont,
                    i;

    if(pcksz < 9) return;
    pcknum = *buff;
    invnum = ~pcknum;
    pcksz = (pcksz - 8) >> 2;
    lbuff += 2;
    cont = 0;

    while(pcksz--) {
        ebpc = *lbuff ^ pcknum;

        ptrebpc = (unsigned char *)&ebpc;
        for(i = 0; i < 4; i++) {
            *ptrebpc ^= (((hlhash[(cont + i) & 0xf] | (i << i)) | i) | 0xA5);
            ptrebpc++;
        }

        *lbuff = HL_NTOHL(ebpc) ^ invnum;
        lbuff++;
        cont++;
    }
}


