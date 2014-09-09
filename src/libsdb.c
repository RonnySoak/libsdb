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
// Data types
// ##########

// #############################################################################
// Technical initialisation
// ########################


// #############################################################################
// Initialisation
// ##############
void sdb_init_fasta(char* fasta_file_name) {
    // TODO
}

void sdb_init_symbol_translation(int type) {
    symtype = type;
}

p_sequence sdb_getnextsequence() {
    // TODO
    return NULL;
}

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db() {
    // TODO
}

// #############################################################################
// Accessors
// #########
