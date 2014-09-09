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
 */
void sdb_init_symbol_translation(int type);

p_sequence sdb_getnextsequence();

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db();

// #############################################################################
// Accessors
// #########

#endif /* LIBSDB_H_ */
