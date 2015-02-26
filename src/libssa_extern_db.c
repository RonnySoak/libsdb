/*
 * libssa_extern_db.c
 *
 * Implementation of the adaptor to libssa
 *
 *  Created on: Sep 26, 2014
 *      Author: Jakob Frielingsdorf
 */

#include "util.h"
#include "libssa_extern_db.h"
#include "database.h"
#include "sdb_error.h"

// #############################################################################
// Initialisation
// ##############
void ssa_db_init( const char* db_name ) {
    if( ssa_db_get_sequence_count() > 0 ) {
        db_free();
    }

    db_open( db_name );

    db_read();
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

    sdb_close_out();
}
