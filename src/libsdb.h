/*
 * libsdb.h
 *
 * Library for Sequence Databases
 *
 *  Created on: Sep 2, 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef LIBSDB_H_
#define LIBSDB_H_

#include "sdb_datatypes.h"

// #############################################################################
// Constants
// #########
#define VERBOSE_ON 1
#define VERBOSE_OFF 0

// TODO have same constants for both in one file!!!
#define NUCLEOTIDE 0 // compare a nucleotide sequence against a nucleotide DB
#define AMINOACID 1 // compare a protein sequence against a protein DB
#define TRANS_QUERY 2 // compare the nucleotide 6-frame translation against a protein DB
#define TRANS_DB 3 // compare a protein sequence against a nucleotide 6-frame translation DB
#define TRANS_BOTH 4 // compare the nucleotide 6-frame translation against a nucleotide 6-frame translation DB

// TODO evaluate
#define PLUS_STRAND 1 // use only the provided nucleic strand
#define MINUS_STRAND 2 // uses both strands, but translates only the complementary one
#define BOTH_STRANDS 3 // uses both strands and translates both to amino acids

// #############################################################################
// Data types
// ##########

/** @typedef    structure of the database */
struct _db;
typedef struct _db* p_db;

// #############################################################################
// Technical initialisation
// ########################

// #############################################################################
// Initialisation
// ##############
void sdb_init_fasta(char* fasta_file_name);

/*
 * TODO the symbol translation is done on the fly on both sides
 *
 * the DB lib return 1, 2 or 6 sequences depending on the translation
 *
 * or some kind of iterator on db side
 *
 * TODO add doc + list of constants
 */
void sdb_init_symbol_translation(int type, int strands, int db_gencode,
        int q_gencode);

p_sdb_sequence sdb_getnextsequence();

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db();

// #############################################################################
// Utility functions
// #################
/**
 * TODO computes the reverse complement
 */
char* sdb_u_revcompl(char* seq, long len);

/**
 * Translates a DNA sequence into a protein sequence.
 *
 * TODO doc
 */
void sdb_u_translate_sequence(char * dna, long dlen, long strand, long frame,
        char ** protp, long * plenp);

// #############################################################################
// Accessors
// #########

#endif /* LIBSDB_H_ */
