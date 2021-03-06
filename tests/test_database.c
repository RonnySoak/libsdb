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

START_TEST (test_database_read)
    {
        db_open( "tests/testdata/AF091148.fas" );
        db_read();

        ck_assert_int_eq( 1403, db_getsequencecount() );

        db_free();
    }END_TEST

START_TEST (test_database_not_found)
    {
        db_open( "tests/testdata/some_impossible_name_foo.bar" );
        ck_assert_int_eq( 1, sdb_has_errors() );

        db_open( NULL );
        ck_assert_int_eq( 1, sdb_has_errors() );

        db_read();

        ck_assert_int_eq( 0, db_getsequencecount() );

        ck_assert_int_eq( 1, sdb_has_errors() );

        p_sdb_error_list e_list = sdb_get_error_list();

        ck_assert_ptr_eq( 3, e_list->count );
        ck_assert_str_eq( "Unable to open input data file", sdb_get_error_desc( e_list->codes[0] ) );
        ck_assert_str_eq( "Unable to open input data file", sdb_get_error_desc( e_list->codes[1] ) );
        ck_assert_str_eq( "Database was not opened", sdb_get_error_desc( e_list->codes[2] ) );
        db_free();
    }END_TEST

START_TEST (test_database_read_double)
    {
        db_open( "tests/testdata/double.fas" );
        db_read();
    }END_TEST

START_TEST (test_database_seqinfo)
    {
        db_open( "tests/testdata/test.fas" );
        db_read();

        ck_assert_int_eq( 5, db_getsequencecount() );

        p_seqinfo info = db_getseqinfo( 0 );

        ck_assert_int_eq( db_getsequence( 0 ), info->seq );
        ck_assert_int_eq( db_getsequencelen( 0 ), info->seqlen );

        for( int i = 0; i < db_getsequencecount(); i++ ) {
            for( int j = 0; j < db_getsequencecount(); j++ ) {
                if( i != j ) {
                    ck_assert_uint_ne( db_getseqinfo( i )->ID, db_getseqinfo( j )->ID );
                }
            }
        }

        db_free();
    }END_TEST

START_TEST (test_database_read_longest)
    {
        db_open( "tests/testdata/test.fas" );
        db_read();

        ck_assert_int_eq( 5, db_getsequencecount() );

        ck_assert_int_eq( 232, db_getlongestsequence() );

        db_free();
    }END_TEST

START_TEST (test_database_read_nuc_count)
    {
        db_open( "tests/testdata/nuc_count.fas" );
        db_read();

        ck_assert_int_eq( 2, db_getsequencecount() );

        ck_assert_int_eq( 361, db_getnucleotidecount() );

        db_free();
    }END_TEST

START_TEST (test_database_read_seq_data)
    {
        db_open( "tests/testdata/test.fas" );
        db_read();

        ck_assert_int_eq( 5, db_getsequencecount() );

        ck_assert_str_eq(
                "gtcgctcctaccgattgaatgcgttggtgattgaattggataaagagatatcatcttaaatgatagcaaagcggtaaacatttgtaaactagattatttagaggaaggagaagtcgtaacaaggtttcc",
                db_getsequence( 3 ) );
        ck_assert_int_eq( 120, db_getsequencelen( 4 ) );

        unsigned long length = 0;
        char* sequence = 0;
        db_getsequenceandlength( 4, &sequence, &length );

        ck_assert_int_eq( 120, length );
        ck_assert_str_eq( sequence, db_getsequence( 4 ) );

        db_free();
    }END_TEST

START_TEST (test_database_read_seq_Aminoacid)
    {
        db_open( "tests/testdata/NP_009305.1.fas" );
        db_read();
        ck_assert_int_eq( 534, db_getsequencelen( 0 ) );

        db_free();
    }END_TEST

void addDatabaseTC( Suite *s ) {
    TCase *tc_core = tcase_create( "database" );
    tcase_add_test( tc_core, test_database_read );
    tcase_add_test( tc_core, test_database_not_found );
    tcase_add_test( tc_core, test_database_seqinfo );
    tcase_add_test( tc_core, test_database_read_longest );
    tcase_add_test( tc_core, test_database_read_nuc_count );
    tcase_add_test( tc_core, test_database_read_seq_data );
    tcase_add_test( tc_core, test_database_read_seq_Aminoacid );
    tcase_add_test( tc_core, test_database_read_double );

    suite_add_tcase( s, tc_core );
}

