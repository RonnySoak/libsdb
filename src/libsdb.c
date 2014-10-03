/*
 * libsdb.c
 *
 * Library for Sequence Databases
 *
 *  Created on: Sep 2, 2014
 *      Author: Jakob Frielingsdorf
 */

#include "util.h"
#include "libsdb.h"
#include "sdb_error.h"

// #############################################################################
// External defined functions
// ##########################

// in database.c
extern void db_open(const char * filename);
extern void db_read();
extern unsigned long db_getsequencecount();
extern p_seqinfo db_getseqinfo(unsigned long seqno);
extern void db_free();

int seq_index;
sdb_error last_error = 0;

// #############################################################################
// Error handling
// ##############
/*
 * TODO Error handling is currently not used/only roughly implemented
 *
 * It has to be evaluated later
 */
/**
 * Sets the last error, but does not override it, if there is already an error set
 */
void set_error(sdb_error err) {
    if (!last_error)
        last_error = err;
}

char* sdb_get_error_desc(sdb_error err) {
    switch (err) {
    case DB_NOT_FOUND:
        return "Unable to open input data file";
    case DB_NOT_CLOSED:
        return "Unable to close input data file";
    case DB_LINE_NOT_READ:
        return "Could not read line";
    case DB_ILLEGAL_HEADER:
        return "Illegal header line";
    case OUT_FILE_NOT_FOUND:
        return "Unable to open output file for writing";
    case OUT_FILE_NOT_CLOSED:
        return "Could not close output file";
    case MALLOC_NOT_ENOUGH_MEM:
        return "Unable to allocate enough memory";
    case REALLOC_NOT_ENOUGH_MEM:
        return "Unable to reallocate enough memory";
    }
    return "No description found";
}

/**
 * Returns the last error and resets it.
 */
sdb_error sdb_last_error() {
    sdb_error tmp = last_error;
    last_error = 0;
    return tmp;
}

// #############################################################################
// Technical initialisation
// ########################
void sdb_init_out(const char* filename) {
    init_out(filename);
}

// #############################################################################
// Initialisation
// ##############
void sdb_init_fasta(const char* fasta_file_name) {
    db_open(fasta_file_name);
//    sdb_error err = sdb_last_error();
//    if(err) {
//        printf("Error: %s\n", sdb_get_error_desc(err));
//        return;
//    }

    db_read();

    seq_index = 0;
}

// #############################################################################
// Accessors
// #########
/**
 * TODO doc
 */
p_seqinfo sdb_next_sequence() {
    if (seq_index >= db_getsequencecount()) {
        // last sequence read
        return NULL;
    }

    return db_getseqinfo(seq_index++);
}

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db() {
    db_free();

    close_out();
}
