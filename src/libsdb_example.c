/*
 * libssa_example.c
 *
 *  Created on: Sep 4, 2014
 *      Author: Jakob Frielingsdorf
 */

#include <stdio.h>

#include "libssa_extern_db.h"

int main( int argc, char**argv ) {
//    sdb_init_fasta("tests/testdata/test.fas");
//    ssa_db_init( "tests/testdata/Rfam_11_0.fasta" );
    ssa_db_init( "tests/testdata/uniprot_sprot.fasta" );

    p_seqinfo info;

    size_t seq_count = ssa_db_get_sequence_count();
    size_t res_count = 0;
    for( size_t i = 0; i < seq_count; ++i ) {
//        printf("sequence: %ld - header: %s\n", info->ID, info->header);

        info = ssa_db_get_sequence( i );

        res_count += info->seqlen;
    }

    printf( "Nr of sequences read: %ld\n", seq_count );
    printf( "Nr of residues read: %ld\n", res_count );

    ssa_db_close();

    return 0;
}
