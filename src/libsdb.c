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
#include "sdb_error.h"
#include "database.h"

p_sdb_error_list sdb_get_errorlist() {
    return sdb_get_error_list();
}

int sdb_init_fasta( const char* fasta_file_name ) {
    db_open( fasta_file_name );

    if( sdb_has_errors() ) {
        return SDB_ERROR;
    }

    db_read();
    if( sdb_has_errors() ) {
        return SDB_ERROR;
    }
    return SDB_OK;
}

p_seqinfo sdb_get_sequence( size_t id ) {
    return db_getseqinfo( id );
}

unsigned long sdb_get_sequence_count() {
    return db_getsequencecount();
}

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void sdb_close() {
    db_free();
}
