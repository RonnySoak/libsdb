/*
 * test_search63.
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include <stdint.h>

#include "../src/util.h"
#include "../src/libsdb.h"

START_TEST (test_xmalloc)
    {
        uint8_t * arr = sdb_malloc( 5 );
        for( int i = 0; i < 5; ++i ) {
            arr[i] = 1;
        }

        p_seqinfo * arr2 = sdb_malloc( 5 * sizeof(p_seqinfo) );
        for( int i = 0; i < 5; ++i ) {
            arr2[i] = sdb_malloc( sizeof(struct seqinfo) );
            arr2[i]->ID = i;
        }

        for( int i = 0; i < 5; ++i ) {
            free( arr2[i] );
        }
        free( arr2 );
        free( arr );
    }END_TEST

START_TEST (test_xmalloc_error)
    {
        uint8_t * arr = sdb_malloc( UINT64_MAX );

        ck_assert_ptr_ne( NULL, arr );
        ck_assert_int_eq( 1, sdb_has_errors() );

        arr = sdb_malloc( 5 );

        arr = sdb_realloc( arr, UINT64_MAX );

        p_sdb_error_list e_list = sdb_get_error_list();
        ck_assert_int_eq( 2, e_list->count );

        ck_assert_int_eq( MALLOC_NOT_ENOUGH_MEM, e_list->codes[0] );
        ck_assert_int_eq( REALLOC_NOT_ENOUGH_MEM, e_list->codes[1] );
    }END_TEST

START_TEST (test_xrealloc)
    {
        p_seqinfo * arr = sdb_malloc( 5 * sizeof(p_seqinfo) );
        for( int i = 0; i < 5; ++i ) {
            arr[i] = sdb_malloc( sizeof(struct seqinfo) );
            arr[i]->ID = i;
        }

        arr = sdb_realloc( arr, 10 * sizeof(p_seqinfo) );
        for( int i = 0; i < 10; ++i ) {
            if( i >= 5 ) {
                arr[i] = sdb_malloc( sizeof(struct seqinfo) );
                arr[i]->ID = i;
            }
        }

        for( int i = 0; i < 10; ++i ) {
            free( arr[i] );
        }
        free( arr );
    }END_TEST

void addUtilTC( Suite *s ) {
    TCase *tc_core = tcase_create( "util" );
    tcase_add_test( tc_core, test_xmalloc );
    tcase_add_test( tc_core, test_xmalloc_error );
    tcase_add_test( tc_core, test_xrealloc );

    suite_add_tcase( s, tc_core );
}

