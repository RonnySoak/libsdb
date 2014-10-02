/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"
#include "../src/libssa_extern_db.h"

extern unsigned long db_getsequencecount();

START_TEST (test_one_sequence)
    {
        ssa_db_init_fasta("tests/testdata/one_seq.fas");

        p_seqinfo seq = ssa_db_next_sequence();
        ck_assert_str_eq("97485665bcded44c4d86c131ca714848", seq->header);
        ck_assert_int_eq(54, seq->seqlen);

        ck_assert_ptr_eq(NULL, ssa_db_next_sequence());

        ssa_db_free();
    }END_TEST

START_TEST (test_multiple_sequences)
    {
        ssa_db_init_fasta("tests/testdata/AF091148.fas");

        ck_assert_int_eq(1403, db_getsequencecount());

        // checks if it can read all sequences
        for (int i = 0; i < 1403; i++) {
            p_seqinfo ref_seq = ssa_db_next_sequence();
            ck_assert_ptr_ne(NULL, ref_seq);

            ck_assert_int_eq(i, (int)ref_seq->ID);
        }

        // end reached?
        ck_assert_ptr_eq(NULL, ssa_db_next_sequence());

        ssa_db_free();
    }END_TEST

void addLibSSAExternDBTC(Suite *s) {
    TCase *tc_core = tcase_create("libssa_extern_db");
    tcase_add_test(tc_core, test_one_sequence);
    tcase_add_test(tc_core, test_multiple_sequences);

    suite_add_tcase(s, tc_core);
}
