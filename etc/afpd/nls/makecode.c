/*
 * $Id: makecode.c,v 1.6 2002-05-23 15:54:27 rufustfirefly Exp $
 *
 * quick-and-dirty way of creating code pages
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netatalk/endian.h>
#include "codepage.h"

/* map of cp10000 (mac roman) to iso-latin-1 */
static const unsigned char mac_to_isolatin1_map[] = {
    0xC4, 0xC5, 0xC7, 0xC9, 0xD1, 0xD6, 0xDC, 0xE1,
    0xE0, 0xE2, 0xE4, 0xE3, 0xE5, 0xE7, 0xE9, 0xE8,
    0xEA, 0xEB, 0xED, 0xEC, 0xEE, 0xEF, 0xF1, 0xF3,
    0xF2, 0xF4, 0xF6, 0xF5, 0xFA, 0xF9, 0xFB, 0xFC,
    0x00, 0xB0, 0xA2, 0xA3, 0xA7, 0xB7, 0xB6, 0xDF,
    0xAE, 0xA9, 0x00, 0xB4, 0xA8, 0x00, 0xC6, 0xD8,
    0x00, 0xB1, 0x00, 0x00, 0xA5, 0xB5, 0xF0, 0x00,
    0x00, 0x00, 0x00, 0xAA, 0xBA, 0x00, 0xE6, 0xF8,
    0xBF, 0xA1, 0xAC, 0x00, 0x00, 0x00, 0x00, 0xAB,
    0xBB, 0x00, 0xA0, 0xC0, 0xC3, 0xD5, 0x00, 0x00,
    0xAD, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x00,
    0xFF, 0x00, 0x00, 0xA4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xB8, 0x00, 0x00, 0xC2, 0xCA, 0xC1,
    0xCB, 0xC8, 0xCD, 0xCE, 0xCF, 0xCC, 0xD3, 0xD4,
    0x00, 0xD2, 0xDA, 0xDB, 0xD9
    /*, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00 */
};

/* map of cp10000 (mac roman) to iso-latin-1 adapted */
static const unsigned char mac_to_isolatin1_adapted_map[] = {
    0xC4, 0xC5, 0xC7, 0xC9, 0xD1, 0xD6, 0xDC, 0xE1,
    0xE0, 0xE2, 0xE4, 0xE3, 0xE5, 0xE7, 0xE9, 0xE8,
    0xEA, 0xEB, 0xED, 0xEC, 0xEE, 0xEF, 0xF1, 0xF3,
    0xF2, 0xF4, 0xF6, 0xF5, 0xFA, 0xF9, 0xFB, 0xFC,
    0x00, 0xB0, 0xA2, 0xA3, 0xA7, 0xB7, 0xB6, 0xDF,
    0xAE, 0xA9, 0xFE, 0xB4, 0xA8, 0x00, 0xC6, 0xD8,
    0x00, 0xB1, 0x00, 0x00, 0xA5, 0xB5, 0xF0, 0x00,
    0x00, 0x00, 0x00, 0xAA, 0xBA, 0x00, 0xE6, 0xF8,
    0xBF, 0xA1, 0xAC, 0x00, 0xFD, 0x00, 0x00, 0xAB,
    0xBB, 0x00, 0xA0, 0xC0, 0xC3, 0xD5, 0x00, 0x00,
    0xAD, 0xAF, 0xB2, 0xB3, 0xDD, 0xDE, 0xF7, 0xB0,
    0xFF, 0x00, 0x00, 0xA4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xB8, 0x00, 0x00, 0xC2, 0xCA, 0xC1,
    0xCB, 0xC8, 0xCD, 0xCE, 0xCF, 0xCC, 0xD3, 0xD4,
    0x00, 0xD2, 0xDA, 0xDB, 0xD9
    /*, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00 */
};

/* Map of CP10000 (Mac Roman) to MSDOS CP 437 */
static const unsigned char mac_to_cp437_map[] = {
    0x8e, 0x8f, 0x80, 0x90, 0xa5, 0x99, 0x9a, 0xa0,
    0x85, 0x83, 0x84, 0xe0, 0x86, 0x87, 0x82, 0x8a,
    0x88, 0x89, 0xa1, 0x8d, 0x8c, 0x8b, 0xa4, 0xa2,
    0x95, 0x93, 0x94, 0xe5, 0xa3, 0x97, 0x96, 0x81,
    0xc5, 0xf8, 0x9b, 0x9c, 0xb6, 0xf9, 0xb8, 0xe1,
    0xc9, 0xe9, 0xcb, 0xd9, 0xc0, 0xd8, 0x92, 0xe8,
    0xec, 0xf1, 0xf3, 0xf2, 0x9d, 0xe6, 0xeb, 0xe4,
    0xef, 0xe3, 0xf4, 0xa6, 0xa7, 0xea, 0x91, 0xed,
    0xa8, 0xad, 0xaa, 0xfb, 0x9f, 0xf7, 0xdd, 0xae,
    0xaf, 0xb0, 0xff, 0xd6, 0xd2, 0xb1, 0xb2, 0xb7,
    0xb4, 0xc4, 0xb5, 0xb9, 0xba, 0xbb, 0xf6, 0xbc,
    0x98, 0xbd, 0xf5, 0xfe, 0xbe, 0xbf, 0xc1, 0xc2,
    0xce, 0xfa, 0xc3, 0xc6, 0xc7, 0xc8, 0xee, 0xca,
    0xcc, 0xcd, 0xb3, 0xd7, 0xcf, 0xd0, 0xd1, 0xd3,
    0xdb, 0xda, 0xa9, 0xab, 0xac, 0xd4, 0xd5, 0xdc,
    0xde, 0xdf, 0xfc, 0xfd, 0xe7, 0xe2, 0xf0, 0x9e
};

/* Map of CP10000 (Mac Roman) to MSDOS CP 850 */
static const unsigned char mac_to_cp850_map[] = {
    0x8e, 0x8f, 0x80, 0x90, 0xa5, 0x99, 0x9a, 0xa0,
    0x85, 0x83, 0x84, 0xc6, 0x86, 0x87, 0x82, 0x8a,
    0x88, 0x89, 0xa1, 0x8d, 0x8c, 0x8b, 0xa4, 0xa2,
    0x95, 0x93, 0x94, 0xe4, 0xa3, 0x97, 0x96, 0x81,
    0xc5, 0xf8, 0xbd, 0x9c, 0xf5, 0xb0, 0xf4, 0xe1,
    0xa9, 0xb8, 0xb1, 0xef, 0xf9, 0xca, 0x92, 0x9d,
    0xb2, 0xf1, 0xb3, 0xb4, 0xbe, 0xe6, 0xd0, 0xba,
    0xbb, 0xcb, 0xdd, 0xa6, 0xa7, 0xbc, 0x91, 0x9b,
    0xa8, 0xad, 0xaa, 0xd9, 0x9f, 0xf2, 0xfe, 0xae,
    0xaf, 0xdc, 0xff, 0xb7, 0xc7, 0xe5, 0xc0, 0xc1,
    0xc2, 0xc4, 0xc3, 0x9e, 0xab, 0xac, 0xf6, 0xbf,
    0x98, 0xed, 0xc8, 0xcf, 0xc9, 0xcc, 0xcd, 0xce,
    0xd1, 0xfa, 0xd5, 0xda, 0xdb, 0xb6, 0xd2, 0xb5,
    0xd3, 0xd4, 0xd6, 0xd7, 0xd8, 0xde, 0xe0, 0xe2,
    0xf0, 0xe3, 0xe9, 0xea, 0xeb, 0xfb, 0xdf, 0xe7,
    0xee, 0xe8, 0xec, 0xf3, 0xf7, 0xfc, 0xfd, 0xb9
};



/* Map of CP10000 (Mac Roman) to koi8-r */
static const unsigned char mac_to_koi8_r_map[] = {
    225, 226, 247, 231, 228, 229, 246, 250,
    233, 234, 235, 236, 237, 238, 239, 240,
    242, 243, 244, 245, 230, 232, 227, 254,
    251, 253, 255, 249, 248, 252, 224, 241,
    160, 161, 162, 163, 164, 165, 166, 167,
    168, 191, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183,
    184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215,
    216, 217, 218, 219, 220, 179, 163, 209,
    193, 194, 215, 199, 196, 197, 214, 218,
    201, 202, 203, 204, 205, 206, 207, 208,
    210, 211, 212, 213, 198, 200, 195, 222,
    219, 221, 223, 217, 216, 220, 192, 255
};

struct mac_code {
    char *m_name;
    const unsigned char *m_map;
    u_int16_t m_len;
    const char *m_id;
};

static struct mac_code names[] = {
                                     {"maccode.437", mac_to_cp437_map, sizeof(mac_to_cp437_map), "cp437"},
                                     {"maccode.850", mac_to_cp850_map, sizeof(mac_to_cp850_map), "cp850"},
                                     {"maccode.iso8859-1", mac_to_isolatin1_map, sizeof(mac_to_isolatin1_map), "iso8859-1"},
                                     {"maccode.iso8859-1.adapted", mac_to_isolatin1_adapted_map, sizeof(mac_to_isolatin1_adapted_map), "iso8859-1-adapted"},
                                     {"maccode.koi8-r",mac_to_koi8_r_map,sizeof(mac_to_koi8_r_map),"koi8-r"},
                                     {NULL, NULL, 0, NULL}
                                 };

int main(int argc, char **argv)
{
    unsigned char buf[CODEPAGE_FILE_HEADER_SIZE];
    u_int16_t id;
    int i, j;
    FILE *fp;

    for (i = 0; names[i].m_name; i++) {
        if ((fp = fopen(names[i].m_name, "w")) == NULL) {
            fprintf(stderr, "can't open %s\n", names[i].m_name);
            continue;
        }

        memset(buf, 0, CODEPAGE_FILE_HEADER_SIZE);

        id = htons(CODEPAGE_FILE_ID); /* file id */
        memcpy(buf, &id, sizeof(id));
        *(buf + 2) = CODEPAGE_FILE_VERSION; /* version */
        if ((j = strlen(names[i].m_id)) & 1) /* pad to even boundary */
            j++;
        *(buf + 3) = j; /* length of name */

        *(buf + 4) = 1; /* default quantum. this should be modified to
           * deal with multibyte characters */

        /* rules */
        *(buf + 5) = CODEPAGE_RULE_MTOU | CODEPAGE_RULE_UTOM;

        /* offset to data */
        id = htons(CODEPAGE_FILE_HEADER_SIZE + j);
        memcpy(buf + 6, &id, sizeof(id));

        /* size of data */
        id = htons(names[i].m_len);
        memcpy(buf + 8, &id, sizeof(id));

        /* write it out */
        fwrite(buf, CODEPAGE_FILE_HEADER_SIZE, 1, fp);

        /* we either keep or drop the null byte to keep the name on an
           even boundary */
        fwrite(names[i].m_id, j, 1, fp);

        /* we typically only map characters > 0x7F */
        for (j = 0; j < names[i].m_len; j++) {
            buf[0] = CODEPAGE_RULE_MTOU | CODEPAGE_RULE_UTOM;
            buf[1] = j + 0x80;
            buf[2] = names[i].m_map[j];
            fwrite(buf, 3, 1, fp);
        }
        fclose(fp);
    }
    return 0;
}