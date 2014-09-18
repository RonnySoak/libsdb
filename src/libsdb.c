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

// in util_sequence.c
extern char* us_revcompl(char* seq, long len);
extern void us_translate_sequence(int db_sequence, char * dna, long dlen,
        long strand, long frame, char ** protp, long * plenp);

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

void sdb_init_symbol_translation(int type, int nr_strands) {
    symtype = type;
    query_strands = nr_strands;
}

p_sdb_sequence sdb_getnextsequence() {
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
// Utility functions
// #################
char* sdb_u_revcompl(char* seq, long len) {
    return us_revcompl(seq, len);
}

void sdb_u_translate_sequence(char * dna, long dlen, long strand, long frame,
        char ** protp, long * plenp) {
    return us_translate_sequence(0, dna, dlen, strand, frame, protp, plenp);
}

// #############################################################################
// Accessors
// #########
