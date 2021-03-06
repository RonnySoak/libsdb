/*
 * database.c
 *
 *  Created on: Sep 2, 2014
 *      Author: Jakob Frielingsdorf
 */

/*
 * Reads a database in FASTA format and stores it in memory.
 *
 * This implementation stores only the sequences and lengths. The headers are omitted.
 *
 * This implementation was taken from the SWARM project:
 * https://github.com/torognes/swarm/blob/master/src/db.cc
 */


/*
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

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

/*
 * This can used, to read only the first x sequences with a total of MAX_READ_RESIDUES
 * amount of residues.
 *
 * This can be used, to test the library with large databases on system with to
 * little memory, to hold the whole database.
 */
#define MAX_READ_RESIDUES 4000000000

static unsigned long sequences = 0;
static unsigned long residues = 0;
static unsigned long longest = 0;
//static unsigned long longestheader = 0;

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
static inline void adjust_data_alloc( unsigned long* current, unsigned long new_size ) {
    while( new_size >= *current ) {
        *current += MEMCHUNK;
        seqdata = sdb_realloc( seqdata, *current );
    }
}

static int create_index() {
    seqindex = sdb_malloc( sequences * sizeof(seqinfo_t) );
    if( !seqindex ) {
        return -1;
    }
    p_seqinfo seq_iterator = seqindex;

    char * data_iterator = seqdata;
    for( unsigned long i = 0; i < sequences; i++ ) {
//        seq_iterator->header = data_iterator;
//        seq_iterator->headerlen = strlen( seq_iterator->header );

//        data_iterator += seq_iterator->headerlen + 1;

        seq_iterator->ID = i;

        seq_iterator->seq = data_iterator;
        seq_iterator->seqlen = strlen( data_iterator );

        data_iterator += seq_iterator->seqlen + 1;

        seq_iterator++;
    }

    return 0;
}

static int read_header( char line[LINEALLOC], unsigned long * dataalloc, unsigned long * datalen ) {
    /* read header */
    if( line[0] != '>' ) {
        sdb_add_error( DB_ILLEGAL_HEADER );
        return 1;
    }

    int headerlen = strlen( line );

    // fgets stops at \n
    if( line[headerlen - 1] == '\n' ) {
        line[headerlen - 1] = 0;
        headerlen--;
    }
    headerlen--; // without the leading '>'

//    if( headerlen > longestheader )
//        longestheader = headerlen;

    /* store the header */
//    adjust_data_alloc( dataalloc, (*datalen + headerlen + 1) );
//
//    memcpy( seqdata + *datalen, line + 1, headerlen );
//    *(seqdata + *datalen + headerlen) = 0; // set NUL terminator
//    *datalen += headerlen + 1;
    return 0;
}

void db_open( const char * filename ) {
    if( filename ) {
        fp = fopen( filename, "r" );
        if( !fp ) {
            sdb_add_error( DB_NOT_FOUND );
            return;
        }
    }
    else {
        sdb_add_error( DB_NOT_FOUND );
        return;
    }
}

/*
 * here we do not check for double sequence-headers
 */
void db_read() {
    if( fp == NULL ) {
        sdb_add_error( DB_NOT_OPEN );
        return;
    }

    /* allocate space */
    unsigned long dataalloc = MEMCHUNK;
    seqdata = sdb_malloc( dataalloc );
    unsigned long datalen = 0;

    longest = 0;
//    longestheader = 0;
    sequences = 0;
    residues = 0;

    char line[LINEALLOC];
    line[0] = 0;
    if( NULL == fgets( line, LINEALLOC, fp ) ) {
        sdb_add_error( DB_LINE_NOT_READ );
        return;
    }

    while( line[0] && (residues < MAX_READ_RESIDUES)  ) {
        if( read_header( line, &dataalloc, &datalen ) ) {
            return;
        }

        /* get next line */
        line[0] = 0;
        if( NULL == fgets( line, LINEALLOC, fp ) ) {
            sdb_add_error( DB_LINE_NOT_READ );
            break;
        }

        /* read sequence */

        unsigned long seqbegin = datalen;

        // reads until the next sequence header is found
        while( line[0] && (line[0] != '>') ) {
            char c;
            char * p = line;
            while( (c = *p++) ) {
                // check for illegal characters
                if( c != '\n' ) {
                    // TODO add checking of illegal symbols

                    adjust_data_alloc( &dataalloc, datalen );

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
            if( NULL == fgets( line, LINEALLOC, fp ) ) {
                break;
            }
        }

        adjust_data_alloc( &dataalloc, datalen );

        unsigned long length = datalen - seqbegin;

        residues += length;

        if( length > longest )
            longest = length;

        *(seqdata + datalen) = 0; // set NUL terminator
        datalen++;

        sequences++;
    }

    /* create indices */
    create_index();

    fclose( fp );
    fp = NULL;
}

void db_free() {
    if( seqdata )
        free( seqdata );
    seqdata = 0;
    if( seqindex )
        free( seqindex );
    seqindex = 0;
}

unsigned long db_getsequencecount() {
    return sequences;
}

unsigned long db_getnucleotidecount() {
    return residues;
}

//unsigned long db_getlongestheader() {
//    return longestheader;
//}

/**
 * Returns the length of the longest sequence.
 */
unsigned long db_getlongestsequence() {
    return longest;
}

p_seqinfo db_getseqinfo( unsigned long seqno ) {
    if( seqno >= db_getsequencecount() ) {
        return NULL;
    }
    return seqindex + seqno;
}

char * db_getsequence( unsigned long seqno ) {
    return seqindex[seqno].seq;
}

void db_getsequenceandlength( unsigned long seqno, char ** address, unsigned long * length ) {
    *address = seqindex[seqno].seq;
    *length = (unsigned long) (seqindex[seqno].seqlen);
}

unsigned long db_getsequencelen( unsigned long seqno ) {
    return seqindex[seqno].seqlen;
}

//char * db_getheader( unsigned long seqno ) {
//    return seqindex[seqno].header;
//}
//
//unsigned long db_getheaderlen( unsigned long seqno ) {
//    return seqindex[seqno].headerlen;
//}

