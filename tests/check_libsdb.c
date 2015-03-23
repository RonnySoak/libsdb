/*
 * check_libssa.c
 *
 *  Created on: 27 Jun 2014
 *      Author: kaos
 */

#include "tests.h"

#include <stdlib.h>
#include <stdio.h>

#include "../src/libsdb.h"

Suite* libsdb_suite( void ) {
    Suite *s = suite_create( "libsdb" );

    /* Core test case */
    addSdbErrorTC( s );
    addUtilTC( s );
    addDatabaseTC( s );
    addLibSDBTC( s );
    addLibSSAExternDBTC( s );

    return s;
}

int main( void ) {
    printf( "Using Check unit testing framework version %d.%d.%d\n", CHECK_MAJOR_VERSION, CHECK_MINOR_VERSION,
            CHECK_MICRO_VERSION );

    int number_failed;
    Suite *s = libsdb_suite();
    SRunner *sr = srunner_create( s );
    srunner_run_all( sr, CK_NORMAL );
    number_failed = srunner_ntests_failed( sr );
    srunner_free( sr );
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
