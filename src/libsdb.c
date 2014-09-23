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

// #############################################################################
// External defined functions
// ##########################

// in database.c
extern void db_read(const char * filename);
extern unsigned long db_getsequencecount();
extern p_seqinfo db_getseqinfo(unsigned long seqno);
extern void db_free();

int seq_index;

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
    db_read(fasta_file_name);

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
