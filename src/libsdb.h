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
#include "sdb_error.h"

#define SDB_OK 0
#define SDB_ERROR 1

// #############################################################################
// Data types
// ##########

p_sdb_error_list sdb_get_errorlist();

// #############################################################################
int sdb_init_fasta(const char* fasta_file_name);

unsigned long sdb_get_sequence_count();

/**
 * Returns the DB sequence of the specified ID.
 */
p_seqinfo sdb_get_sequence( size_t id );

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_close();

#endif /* LIBSDB_H_ */
