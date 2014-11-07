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

#include "libssa_extern_db.h"
/**
 * TODO
 *
 * add functions, to get chunks of the DB
 */

// #############################################################################
// Data types
// ##########

typedef enum sdb_error sdb_error;

// #############################################################################
// Constants
// #########
#define VERBOSE_ON 1 // TODO
#define VERBOSE_OFF 0

// #############################################################################
// Error handling
// ##############
char* sdb_get_error_desc(sdb_error err);

sdb_error sdb_last_error();

// #############################################################################
// Technical initialisation
// ########################
void sdb_init_out(const char* filename);

// #############################################################################
// Initialisation
// ##############
void sdb_init_fasta(const char* fasta_file_name);

// #############################################################################
// Accessors
// #########
/**
 * Returns the next sequence available in the database.
 *
 * TODO better to copy the data to a new instance, to prevent alteration of the
 *      DB from the outside
 */
p_seqinfo sdb_next_sequence();

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_free_db();

#endif /* LIBSDB_H_ */
