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

/**
 * TODO
 * Merge libsdb and libssa
 *
 * specify functions, which enable the caller, to use an own implementation fo the database
 *  -> e.g. a function pointer as a parameter
 *
 * add functions, to get chunks of the DB
 */

#include "sdb_datatypes.h"

// #############################################################################
// Constants
// #########
#define VERBOSE_ON 1
#define VERBOSE_OFF 0

#define NUCLEOTIDE 0 // compare a nucleotide sequence against a nucleotide DB
#define AMINOACID 1 // compare a protein sequence against a protein DB
#define TRANS_QUERY 2 // compare the nucleotide 6-frame translation against a protein DB
#define TRANS_DB 3 // compare a protein sequence against a nucleotide 6-frame translation DB
#define TRANS_BOTH 4 // compare the nucleotide 6-frame translation against a nucleotide 6-frame translation DB

#define FORWARD_STRAND 1 // use only the provided nucleic strand
#define COMPLEMENTARY_STRAND 2 // uses both strands, but translates only the complementary one
#define BOTH_STRANDS 3 // uses both strands and translates both to amino acids

// #############################################################################
// Technical initialisation
// ########################
void sdb_init_out(const char* filename);

// #############################################################################
// Initialisation
// ##############
void sdb_init_fasta(char* fasta_file_name);

/**
 * Initialise the database lib, to use a third party implementation.
 */
void sdb_init_external(p_sdb_sequence (*extern_next_sequence)());

/*
 * TODO the symbol translation is done on the fly on both sides
 *
 * the DB lib return 1, 2 or 6 sequences depending on the translation
 *
 * or some kind of iterator on db side
 *
 * TODO add doc + list of constants
 */
void sdb_init_symbol_handling(int type, int strands, int db_gencode,
        int q_gencode);

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
/**
 * Returns the next sequence available in the database.
 *
 * The memory of the returned sequence struct might be reused during the next
 * call of sdb_next_sequence! All data of it must be saved elsewhere!
 */
p_sdb_sequence sdb_next_sequence();

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db();

#endif /* LIBSDB_H_ */
