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
        sdb_init_fasta("tests/testdata/one_seq.fas");

        p_seqinfo seq = sdb_next_sequence();
        ck_assert_int_eq(54, seq->seqlen);

        ck_assert_ptr_eq(NULL, sdb_next_sequence());

        sdb_free_db();
    }END_TEST

START_TEST (test_multiple_sequences_translated)
    {
        sdb_init_fasta("tests/testdata/AF091148.fas");

        ck_assert_int_eq(1403, db_getsequencecount());

        // checks if it can read all sequences
        for (int i = 0; i < 1403; i++) {
            p_seqinfo ref_seq = sdb_next_sequence();
            ck_assert_ptr_ne(NULL, ref_seq);

            ck_assert_int_eq(i, (int)ref_seq->ID);
        }

        // end reached?
        ck_assert_ptr_eq(NULL, sdb_next_sequence());

        sdb_free_db();
    }END_TEST

void addLibSDBTC(Suite *s) {
    TCase *tc_core = tcase_create("libsdb");
    tcase_add_test(tc_core, test_one_sequence);
    tcase_add_test(tc_core, test_multiple_sequences_translated);

    suite_add_tcase(s, tc_core);
}

