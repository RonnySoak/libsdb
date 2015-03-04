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
#include "database.h"

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


/**
 * Returns the last error and resets it.
 */
sdb_error sdb_last_error() {
    sdb_error tmp = last_error;
    last_error = 0;
    return tmp;
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
}
