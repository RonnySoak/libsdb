/*
 * libssa_extern_db.c
 *
 * Implementation of the adaptor to libssa
 *
 *  Created on: Sep 26, 2014
 *      Author: Jakob Frielingsdorf
 */

#include "util.h"
#include "libssa_extern_db.h"
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

// #############################################################################
// Initialisation
// ##############
void ssa_db_init_fasta(const char* fasta_file_name) {
    db_open(fasta_file_name);

    db_read();

    ssa_db_reset_sequence_counter();
}

// #############################################################################
// Accessors
// #########
void ssa_db_reset_sequence_counter() {
    seq_index = 0;
}

unsigned long ssa_db_get_sequence_count() {
    return db_getsequencecount();
}

p_seqinfo ssa_db_get_sequence(unsigned long seqno) {
    return db_getseqinfo(seqno);
}

long ssa_db_get_longest_sequence() {
    return db_getlongestsequence();
}

/**
 * TODO doc
 */
p_seqinfo ssa_db_next_sequence() {
    return db_getseqinfo(seq_index++);
}

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void ssa_db_free() {
    db_free();

    close_out();
}
