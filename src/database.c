/*
 * database.c
 *
 *  Created on: Sep 2, 2014
 *      Author: Jakob Frielingsdorf
 */

/*
 * TODO doc
 *
 * we store the sequences in ASCII code instead of the mapped values
 *
 * and use mapping for checking for illegal characters
 *
 * report sequences with illegal characters and remove some characters without reporting
 * (additional space or newline, etc)
 *
 * stores headers and sequences in memory, as they are in the file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libsdb.h"
#include "util.h"
#include "sdb_datatypes.h"

extern const char map_ncbi_nt16[256];
extern const char map_ncbi_aa[256];

/** Used mapping, for reading the DB. one of nt16 or aa*/
const char* map;

unsigned long sequences = 0;
unsigned long nucleotides = 0;
int longest = 0;
int longestheader = 0;

/**
 * Stores the indices, where the data and header of the sequences in
 * @see seqdata are located.
 */
p_seqinfo seqindex = 0;
/**
 * Stores the raw header and sequence data as found in the database file.
 */
char * seqdata = 0;

#define MEMCHUNK 1048576
#define LINEALLOC LINE_MAX

/**
 * Adjusts the memory allocated for storing the sequence data.
 */
static inline void adjust_data_alloc(unsigned long* current,
        unsigned long new_size) {
    while (new_size >= *current) {
        *current += MEMCHUNK;
        seqdata = (char *) xrealloc(seqdata, *current);
    }
}

static void init_mapping() {
    if ((symtype == AMINOACID) || (symtype == TRANS_QUERY)) {
        map = map_ncbi_aa;
    }
    else {
        map = map_ncbi_nt16;
    }
}

/*
 * here we do not check for double sequence-headers
 */
void db_read(const char * filename) {
    init_mapping();

    /* allocate space */
    unsigned long dataalloc = MEMCHUNK;
    seqdata = (char *) xmalloc(dataalloc);
    unsigned long datalen = 0;

    longest = 0;
    longestheader = 0;
    sequences = 0;
    nucleotides = 0;

    FILE * fp = NULL;
    if (filename) {
        fp = fopen(filename, "r");
        if (!fp)
            ffatal("Error: Unable to open input data file (%s).", filename);
    }
    else {
        fp = stdin;
    }

    char line[LINEALLOC];
    line[0] = 0;
    if ( NULL == fgets(line, LINEALLOC, fp)) {
        ffatal("Could not read query sequence");
    }

    while (line[0]) {
        /* read header */
        if (line[0] != '>')
            ffatal("Illegal header line in fasta file.");

        int headerlen = strlen(line);

        // fgets stops at \n
        if (line[headerlen - 1] == '\n') {
            line[headerlen - 1] = 0;
            headerlen--;
        }
        headerlen--; // without the leading '>'

        if (headerlen > longestheader)
            longestheader = headerlen;

        /* store the header */
        adjust_data_alloc(&dataalloc, (datalen + headerlen + 1));

        memcpy(seqdata + datalen, line + 1, headerlen);
        *(seqdata + datalen + headerlen) = 0; // set NUL terminator
        datalen += headerlen + 1;

        /* get next line */
        line[0] = 0;
        if ( NULL == fgets(line, LINEALLOC, fp)) {
            ffatal("Could not read query sequence");
        }

        /* read sequence */

        unsigned long seqbegin = datalen;

        // reads until the next sequence header is found
        while (line[0] && (line[0] != '>')) {
            char c;
            char * p = line;
            while ((c = *p++)) {
                // check for illegal characters
                if (map[(int) c] >= 0) {
                    adjust_data_alloc(&dataalloc, datalen);

                    *(seqdata + datalen) = c;
                    datalen++;
                }
                else if (c != '\n') {
                    // TODO some characters (like '\n') should only be removed, but the sequence in total might be fine
                    ffatal("Illegal character in sequence: '%c'%d", c, c);
                }
            }

            /* get next line */
            line[0] = 0;
            if ( NULL == fgets(line, LINEALLOC, fp)) {
                break;
            }
        }

        adjust_data_alloc(&dataalloc, datalen);

        long length = datalen - seqbegin;

        nucleotides += length;

        if (length > longest)
            longest = length;

        *(seqdata + datalen) = 0; // set NUL terminator
        datalen++;

        sequences++;
    }

    fclose(fp);

    /* create indices */
    seqindex = (p_seqinfo) xmalloc(sequences * sizeof(seqinfo));
    p_seqinfo seq_iterator = seqindex;

    char * data_iterator = seqdata;
    for (unsigned long i = 0; i < sequences; i++) {
        seq_iterator->header = data_iterator;
        seq_iterator->headerlen = strlen(seq_iterator->header);

        data_iterator += seq_iterator->headerlen + 1;

        seq_iterator->headeridlen = xstrchrnul(seq_iterator->header, ' ')
                - seq_iterator->header;

        seq_iterator->seq = data_iterator;
        seq_iterator->seqlen = strlen(data_iterator);

        data_iterator += seq_iterator->seqlen + 1;

        seq_iterator++;
    }
}

unsigned long db_getsequencecount() {
    return sequences;
}

unsigned long db_getnucleotidecount() {
    return nucleotides;
}

unsigned long db_getlongestheader() {
    return longestheader;
}

/**
 * Returns the length of the longest sequence.
 */
unsigned long db_getlongestsequence() {
    return longest;
}

p_seqinfo db_getseqinfo(unsigned long seqno) {
    return seqindex + seqno;
}

char * db_getsequence(unsigned long seqno) {
    return seqindex[seqno].seq;
}

void db_getsequenceandlength(unsigned long seqno, char ** address,
        long * length) {
    *address = seqindex[seqno].seq;
    *length = (long) (seqindex[seqno].seqlen);
}

unsigned long db_getsequencelen(unsigned long seqno) {
    return seqindex[seqno].seqlen;
}

char * db_getheader(unsigned long seqno) {
    return seqindex[seqno].header;
}

unsigned long db_getheaderlen(unsigned long seqno) {
    return seqindex[seqno].headerlen;
}

void db_free() {
    if (seqdata)
        free(seqdata);
    if (seqindex)
        free(seqindex);
}
