/*
 * database.c
 *
 *  Created on: Sep 2, 2014
 *      Author: Jakob Frielingsdorf
 */

/*
 * we store the sequences in ASCII code instead of the mapped values
 *
 * stores headers and sequences in memory, as they are in the file
 *
 * TODO
 * doc
 *
 * and use mapping for checking for illegal characters
 *
 * report sequences with illegal characters and remove some characters without reporting
 * (additional space or newline, etc)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libsdb.h"
#include "util.h"
#include "sdb_error.h"

#include "database.h"

static unsigned long sequences = 0;
static unsigned long nucleotides = 0;
static unsigned long longest = 0;
static unsigned long longestheader = 0;

/**
 * Stores the indices, where the data and header of the sequences in
 * @see seqdata are located.
 */
static p_seqinfo seqindex = 0;
/**
 * Stores the raw header and sequence data as found in the database file.
 */
static char * seqdata = 0;

static FILE * fp = NULL;

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

static int create_index() {
    seqindex = xmalloc(sequences * sizeof(seqinfo));
    if (!seqindex) {
        return -1;
    }
    p_seqinfo seq_iterator = seqindex;

    char * data_iterator = seqdata;
    for (unsigned long i = 0; i < sequences; i++) {
        seq_iterator->header = data_iterator;
        seq_iterator->headerlen = strlen(seq_iterator->header);

        data_iterator += seq_iterator->headerlen + 1;

        seq_iterator->ID = i;

        seq_iterator->seq = data_iterator;
        seq_iterator->seqlen = strlen(data_iterator);

        data_iterator += seq_iterator->seqlen + 1;

        seq_iterator++;
    }

    return 0;
}

static int read_header(char line[LINEALLOC],
        unsigned long * dataalloc, unsigned long * datalen) {
    /* read header */
    if (line[0] != '>') {
        ffatal("Illegal header line in fasta file.");
//        set_error(DB_ILLEGAL_HEADER);
//        return -1;
    }

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
    adjust_data_alloc(dataalloc, (*datalen + headerlen + 1));

    memcpy(seqdata + *datalen, line + 1, headerlen);
    *(seqdata + *datalen + headerlen) = 0; // set NUL terminator
    *datalen += headerlen + 1;

    return 0;
}

void db_open(const char * filename) {
    if (filename) {
        fp = fopen(filename, "r");
        if (!fp) {
//            set_error(DB_NOT_FOUND);
//            return;
            ffatal("Could not read DB");
        }
    }
    else {
        fp = stdin;
    }
}

/*
 * here we do not check for double sequence-headers
 */
void db_read() {
    /* allocate space */
    unsigned long dataalloc = MEMCHUNK;
    seqdata = xmalloc(dataalloc);
    unsigned long datalen = 0;

    longest = 0;
    longestheader = 0;
    sequences = 0;
    nucleotides = 0;

    char line[LINEALLOC];
    line[0] = 0;
    if ( NULL == fgets(line, LINEALLOC, fp)) {
//        set_error(DB_LINE_NOT_READ);
//        return;
        ffatal("Could not read query sequence");
    }

    while (line[0]) {
        read_header(line, &dataalloc, &datalen);

        /* get next line */
        line[0] = 0;
        if ( NULL == fgets(line, LINEALLOC, fp)) {
//            set_error(DB_LINE_NOT_READ);
//            break;
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
                if (c != '\n') {
                    // TODO add checking of illegal symbols

                    adjust_data_alloc(&dataalloc, datalen);

                    *(seqdata + datalen) = c;
                    datalen++;
                }

//                if (map[(int) c] >= 0) {
//                }
//                else if (c != '\n') {
//                    // TODO some characters (like '\n') should only be removed, but the sequence in total might be fine
//                    ffatal("Illegal character in sequence: '%c'%d", c, c);
//                }
            }

            /* get next line */
            line[0] = 0;
            if ( NULL == fgets(line, LINEALLOC, fp)) {
                break;
            }
        }

        adjust_data_alloc(&dataalloc, datalen);

        unsigned long length = datalen - seqbegin;

        nucleotides += length;

        if (length > longest)
            longest = length;

        *(seqdata + datalen) = 0; // set NUL terminator
        datalen++;

        sequences++;
    }

    /* create indices */
    create_index();
}

void db_free() {
    if (fp) {
        if (!fclose(fp)) {
            set_error(DB_NOT_CLOSED);
        }
    }

    if (seqdata)
        free(seqdata);
    seqdata = 0;
    if (seqindex)
        free(seqindex);
    seqindex = 0;
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
        unsigned long * length) {
    *address = seqindex[seqno].seq;
    *length = (unsigned long) (seqindex[seqno].seqlen);
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

