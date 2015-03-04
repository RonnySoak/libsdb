/*
 * libssa_extern_db.c
 *
 * Implementation of the adaptor to libssa
 *
 *  Created on: Sep 26, 2014
 *      Author: Jakob Frielingsdorf
 */

#include "libssa_extern_db.h"

#include "libsdb.h"
#include "util.h"
#include "database.h"
#include "sdb_error.h"

// #############################################################################
// Initialisation
// ##############
int ssa_db_init( const char* db_name ) {
    db_open( db_name );

    if( sdb_has_errors() ) {
        return SDB_ERROR;
    }

    db_read();
    if( sdb_has_errors() ) {
        return SDB_ERROR;
    }
    return SDB_OK;
}

// #############################################################################
// Accessors
// #########
unsigned long ssa_db_get_sequence_count() {
    return db_getsequencecount();
}

p_seqinfo ssa_db_get_sequence( unsigned long seqno ) {
    return db_getseqinfo( seqno );
}

/**
 * Releases the memory allocated by the function sdb_init_fasta.
 *
 * @see sdb_init_fasta
 */
void ssa_db_close() {
    db_free();
}
