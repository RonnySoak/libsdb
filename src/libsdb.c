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
extern void us_init_translation(int qtableno, int dtableno);

// in db_iterator.c
extern void it_init();
extern p_sdb_sequence it_next();
extern void it_free();

// in database.c
extern void db_read(const char * filename);
extern void db_free();

// #############################################################################
// Technical initialisation
// ########################

// #############################################################################
// Initialisation
// ##############
void sdb_init_fasta(char* fasta_file_name) {
    db_read(fasta_file_name);
}

void sdb_init_symbol_handling(int type, int nr_strands, int db_gencode,
        int q_gencode) {
    symtype = type;
    query_strands = nr_strands;

    us_init_translation(q_gencode, db_gencode);

    it_init();
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

p_sdb_sequence sdb_next_sequence() {
    return it_next();
}

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db() {
    it_free();

    db_free();
}
