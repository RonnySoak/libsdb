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

START_TEST (test_multiple_sequences_translated)
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
//
//    p_seqinfo* external_test_sequences;
//int idx = 0;
//
//p_seqinfo test_external_next() {
//    return external_test_sequences[idx++];
//}
//
//START_TEST (test_external_next_function)
//    {
//        external_test_sequences = (p_sdb_sequence*) xmalloc(
//                3 * sizeof(sdb_sequence));
//
//        external_test_sequences[0]->info = 0;
//        external_test_sequences[0]->len = 5;
//        external_test_sequences[0]->seq = "ATCGA";
//        external_test_sequences[0]->strand = 0;
//        external_test_sequences[0]->frame = 0;
//
//        external_test_sequences[0]->info = 0;
//        external_test_sequences[0]->len = 4;
//        external_test_sequences[0]->seq = "AAGC";
//        external_test_sequences[0]->strand = 1;
//        external_test_sequences[0]->frame = 0;
//
//        external_test_sequences[0]->info = 0;
//        external_test_sequences[0]->len = 6;
//        external_test_sequences[0]->seq = "AAGGCT";
//        external_test_sequences[0]->strand = 0;
//        external_test_sequences[0]->frame = 0;
//
//        sdb_init_out(NULL);
//
//        sdb_init_external(test_external_next);
//
//        // checks if it can read all sequences and its translations
//        for (int i = 0; i < 3; i++) {
//            ck_assert_ptr_eq(external_test_sequences[i], sdb_next_sequence());
//        }
//
//        // end reached?
//        ck_assert_ptr_eq(NULL, sdb_next_sequence());
//
//        sdb_free_db();
//    }END_TEST

void addLibSSAExternDBTC(Suite *s) {
    TCase *tc_core = tcase_create("libssa_extern_db");
    tcase_add_test(tc_core, test_one_sequence);
    tcase_add_test(tc_core, test_multiple_sequences_translated);

    suite_add_tcase(s, tc_core);
}

