/*
 * test_search63.
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"
#include "../src/libsdb.h"

START_TEST (test_error)
    {
        ck_assert_int_eq( 0, sdb_has_errors() );

        sdb_add_error( DB_NOT_FOUND );

        ck_assert_int_eq( 1, sdb_has_errors() );

        ck_assert_str_eq( "Unable to open input data file", sdb_get_error_desc( DB_NOT_FOUND ) );

        p_sdb_error_list e_list = sdb_get_error_list();

        ck_assert_int_eq( 1, e_list->count );
        ck_assert_int_eq( DB_NOT_FOUND, e_list->codes[0] );
    }END_TEST

START_TEST (test_more_errors)
    {
        sdb_add_error( DB_NOT_FOUND );
        sdb_add_error( DB_LINE_NOT_READ );
        sdb_add_error( REALLOC_NOT_ENOUGH_MEM );

        ck_assert_int_eq( 1, sdb_has_errors() );

        p_sdb_error_list e_list = sdb_get_error_list();

        ck_assert_int_eq( 3, e_list->count );
        ck_assert_int_eq( DB_NOT_FOUND, e_list->codes[0] );
        ck_assert_int_eq( DB_LINE_NOT_READ, e_list->codes[1] );
        ck_assert_int_eq( REALLOC_NOT_ENOUGH_MEM, e_list->codes[2] );
    }END_TEST

void addSdbErrorTC( Suite *s ) {
    TCase *tc_core = tcase_create( "sdb_error" );
    tcase_add_test( tc_core, test_error );
    tcase_add_test( tc_core, test_more_errors );

    suite_add_tcase( s, tc_core );
}

