/*
 * util_sequence.c
 *
 *  Created on: Sep 18, 2014
 *      Author: Jakob Frielingsdorf
 */
#include "util.h"

const char map_ncbi_aa[256] =
        {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, 0, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 27, 10, 11, 12, 13, 26,
                14, 15, 16, 17, 18, 24, 19, 20, 21, 22, 23, -1, -1, -1, -1, -1,
                -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 27, 10, 11, 12, 13, 26,
                14, 15, 16, 17, 18, 24, 19, 20, 21, 22, 23, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };
const char map_ncbi_nt4[256] =
        {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, 0, -1, 1, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, 3, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, 0, -1, 1, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, 3, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };

const char map_ncbi_nt16[256] =
        {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, 1, 14, 2, 13, -1, -1, 4, 11, -1, -1, 12, -1, 3, 15, -1,
                -1, -1, 5, 6, 8, 8, 7, 9, -1, 10, -1, -1, -1, -1, -1, -1,
                -1, 1, 14, 2, 13, -1, -1, 4, 11, -1, -1, 12, -1, 3, 15, -1,
                -1, -1, 5, 6, 8, 8, 7, 9, -1, 10, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };

const char * gencode_names[23] =
        {
                "Standard Code",
                "Vertebrate Mitochondrial Code",
                "Yeast Mitochondrial Code",
                "Mold, Protozoan, and Coelenterate Mitochondrial Code and Mycoplasma/Spiroplasma Code",
                "Invertebrate Mitochondrial Code",
                "Ciliate, Dasycladacean and Hexamita Nuclear Code",
                NULL,
                NULL,
                "Echinoderm and Flatworm Mitochondrial Code",
                "Euplotid Nuclear Code",
                "Bacterial, Archaeal and Plant Plastid Code",
                "Alternative Yeast Nuclear Code",
                "Ascidian Mitochondrial Code",
                "Alternative Flatworm Mitochondrial Code",
                "Blepharisma Nuclear Code",
                "Chlorophycean Mitochondrial Code",
                NULL,
                NULL,
                NULL,
                NULL,
                "Trematode Mitochondrial Code",
                "Scenedesmus obliquus Mitochondrial Code",
                "Thraustochytrium Mitochondrial Code"
        };

const char * code[23] =
        {
                "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSS**VVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CCWWTTTTPPPPHHQQRRRRIIMMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSSSSVVVVAAAADDEEGGGG",
                "FFLLSSSSYYQQCC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                NULL,
                NULL,
                "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNNKSSSSVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CCCWLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CC*WLLLSPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSSGGVVVVAAAADDEEGGGG",
                "FFLLSSSSYYY*CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNNKSSSSVVVVAAAADDEEGGGG",
                "FFLLSSSSYY*QCC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FFLLSSSSYY*LCC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                NULL,
                NULL,
                NULL,
                NULL,
                "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNNKSSSSVVVVAAAADDEEGGGG",
                "FFLLSS*SYY*LCC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
                "FF*LSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG"
        };

const char * sym_ncbi_nt4 = "acgt############################";
const char * sym_ncbi_nt16 = "-acmgrsvtwyhkdbn################";
const char * sym_ncbi_nt16u = "-ACMGRSVTWYHKDBN################";
const char * sym_ncbi_aa = "-ABCDEFGHIKLMNPQRSTVWXYZU*OJ####";

static const char ntcompl[16] =
        { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };

static char q_translate[16 * 16 * 16];
static char d_translate[16 * 16 * 16];

static const char remap[] = { 2, 1, 3, 0 };

static void create_translate_table(int tableno, char * table) {
    /* initialise translation table */

    // TODO understand this !!!!
    for (long a = 0; a < 16; a++) {
        for (long b = 0; b < 16; b++) {
            for (long c = 0; c < 16; c++) {
                char aa = '-';

                for (long i = 0; i < 4; i++) {
                    for (long j = 0; j < 4; j++) {
                        for (long k = 0; k < 4; k++) {
                            if ((a & (1 << i)) && (b & (1 << j))
                                    && (c & (1 << k))) {
                                long codon = remap[i] * 16 + remap[j] * 4
                                        + remap[k];

                                char x = code[tableno - 1][codon];

                                if (aa == '-') {
                                    aa = x;
                                }
                                else if (aa == x) {
                                    // do nothing
                                }
                                else if ((aa == 'B')
                                        && ((x == 'D') || (x == 'N'))) {
                                    // do nothing
                                }
                                else if ((aa == 'D')
                                        && ((x == 'B') || (x == 'N'))) {
                                    aa = 'B';
                                }
                                else if ((aa == 'N')
                                        && ((x == 'B') || (x == 'D'))) {
                                    aa = 'B';
                                }
                                else if ((aa == 'Z')
                                        && ((x == 'Q') || (x == 'E'))) {
                                    // do nothing
                                }
                                else if ((aa == 'E')
                                        && ((x == 'Z') || (x == 'Q'))) {
                                    aa = 'Z';
                                }
                                else if ((aa == 'Q')
                                        && ((x == 'Z') || (x == 'E'))) {
                                    aa = 'Z';
                                }
                                else {
                                    aa = 'X';
                                }
                            }
                        }
                    }
                }

                if (aa == '-')
                    aa = 'X';

                table[256 * a + 16 * b + c] = map_ncbi_aa[(int) aa];
            }
        }
    }

#if 0
    /* dump it */

    outf("          -ACMGRSVTWYHKDBN\n");
    for(long x=0; x<16; x++)
    for(long y=0; y<16; y++) {
        outf("%2ld %2ld %c %c ", x, y, sym_ncbi_nt16[x], sym_ncbi_nt16[y]);
        for(long z=0; z<16; z++) {
            outf("%c", sym_ncbi_aa[table[256*x+16*y+z]]);
        }
        outf("\n");
    }
#endif
}

void us_init_translation(int qtableno, int dtableno) {
    create_translate_table(qtableno, q_translate);
    create_translate_table(dtableno, d_translate);
}

void us_translate_sequence(int db_sequence, char * dna, long dlen,
        long strand, long frame, char ** protp, long * plenp) {
    //  outf("dlen=%ld, strand=%ld, frame=%ld\n", dlen, strand, frame);    printf("1\n");

    char* ttable = (db_sequence) ? d_translate : q_translate;


    long pos, c;
    long ppos = 0;
    long plen = (dlen - frame) / 3;
    char * prot = (char*) xmalloc(1 + plen);

    // forward strand
    if (strand == 0) {
        pos = frame;
        while (ppos < plen) {
            c = dna[pos++];
            c <<= 4;
            c |= dna[pos++];
            c <<= 4;
            c |= dna[pos++];
            prot[ppos++] = ttable[c];
        }
    }
    // complement strand, done in reverse complement
    else {
        pos = dlen - 1 - frame;
        while (ppos < plen) {
            c = ntcompl[(int) (dna[pos--])];
            c <<= 4;
            c |= ntcompl[(int) (dna[pos--])];
            c <<= 4;
            c |= ntcompl[(int) (dna[pos--])];
            prot[ppos++] = ttable[c];
        }
    }

    prot[ppos] = 0;
    *protp = prot;
    *plenp = plen;
}

/**
 * Computes the reverse complement of the input sequence. If the input sequence
 * is empty, the function returns 0.
 *
 * @param seq   the sequence to reverse complement
 * @param len   the length of the sequence
 * @return      the reverse complement or 0 in case of an empty sequence
 */
char* us_revcompl(char* seq, long len) {
    if (!len) {
        return 0;
    }

    char* rc = (char *) xmalloc(len + 1);
    for (long i = 0; i < len; i++)
        rc[i] = ntcompl[(int) (seq[len - 1 - i])];
    rc[len] = 0;
    return rc;
}
