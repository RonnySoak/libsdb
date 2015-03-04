/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"
#include "../src/libsdb.h"
#include "../src/database.h"

START_TEST (test_one_sequence)
    {
        sdb_init_fasta( "tests/testdata/one_seq.fas" );

        p_seqinfo seq = sdb_get_sequence( 0 );
        ck_assert_int_eq( 54, seq->seqlen );

        ck_assert_ptr_eq( NULL, sdb_get_sequence( 1 ) );

        sdb_close();
    }END_TEST

START_TEST (test_multiple_sequences_translated)
    {
        sdb_init_fasta( "tests/testdata/AF091148.fas" );

        ck_assert_int_eq( 1403, sdb_get_sequence_count() );

        // checks if it can read all sequences
        for( int i = 0; i < 1403; i++ ) {
            p_seqinfo ref_seq = sdb_get_sequence( i );
            ck_assert_ptr_ne( NULL, ref_seq );

            ck_assert_int_eq( i, (int )ref_seq->ID );
        }

        // end reached?
        ck_assert_ptr_eq( NULL, sdb_get_sequence( 1403 ) );

        sdb_close();
    }END_TEST

START_TEST (test_error_no_db)
    {
        sdb_init_fasta( NULL );

        ck_assert_int_eq( 1, sdb_has_errors() );

        ck_assert_str_eq( "Unable to open input data file", sdb_get_error_desc( DB_NOT_FOUND ) );

        p_sdb_error_list e_list = sdb_get_errorlist();

        ck_assert_int_eq( 1, e_list->count );
        ck_assert_int_eq( DB_NOT_FOUND, e_list->codes[0] );

        sdb_close();
    }END_TEST

void addLibSDBTC( Suite *s ) {
    TCase *tc_core = tcase_create( "libsdb" );
    tcase_add_test( tc_core, test_one_sequence );
    tcase_add_test( tc_core, test_multiple_sequences_translated );
    tcase_add_test( tc_core, test_error_no_db );

    suite_add_tcase( s, tc_core );
}

