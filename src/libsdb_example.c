/*
 * libssa_example.c
 *
 *  Created on: Sep 4, 2014
 *      Author: Jakob Frielingsdorf
 */

#include <stdio.h>

#include "libssa_extern_db.h"

void run_example( char * db_file ) {
    ssa_db_init( db_file );

    p_seqinfo info;

    size_t seq_count = ssa_db_get_sequence_count();
    size_t res_count = 0;
    for( size_t i = 0; i < seq_count; ++i ) {
        info = ssa_db_get_sequence( i );

        res_count += info->seqlen;
    }

    printf( "Nr of sequences read: %ld\n", seq_count );
    printf( "Nr of residues read: %ld\n", res_count );

    ssa_db_close();
}

int main( int argc, char**argv ) {
//    run_example("tests/testdata/test.fas");
    run_example( "tests/testdata/uniprot_sprot.fasta" );
    run_example( "tests/testdata/Rfam_11_0.fasta" );

    run_example( "../uniprot_trembl.fasta" );

    return 0;
}
