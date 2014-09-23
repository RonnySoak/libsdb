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
 *
 * add functions, to get chunks of the DB
 */

// #############################################################################
// Data types
// ##########

typedef struct seqinfo {
    char * header;
    char * seq;
    unsigned long ID;
    unsigned long headerlen;
    unsigned long seqlen;
} seqinfo;

typedef struct seqinfo* p_seqinfo;

// #############################################################################
// Constants
// #########
#define VERBOSE_ON 1 // TODO
#define VERBOSE_OFF 0

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
