/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"
#include "../src/libsdb.h"
#include "../src/sdb_datatypes.h"

extern unsigned long db_getsequencecount();

START_TEST (test_one_sequence)
    {
        sdb_init_out(NULL);

        sdb_init_fasta("tests/testdata/one_seq.fas");

        sdb_init_symbol_handling(NUCLEOTIDE, FORWARD_STRAND, 3, 3);

        p_sdb_sequence seq = sdb_next_sequence();
        ck_assert_str_eq("97485665bcded44c4d86c131ca714848", seq->info->header);
        ck_assert_int_eq(54, seq->len);

        ck_assert_ptr_eq(NULL, sdb_next_sequence());

        sdb_free_db();
    }END_TEST

START_TEST (test_multiple_sequences_translated)
    {
        sdb_init_out(NULL);

        sdb_init_fasta("tests/testdata/AF091148.fas");

        ck_assert_int_eq(1403, db_getsequencecount());

        sdb_init_symbol_handling(TRANS_DB, BOTH_STRANDS, 3, 3);

        // checks if it can read all sequences and its translations
        for (int i = 0; i < 1403; i++) {
            p_sdb_sequence ref_seq = sdb_next_sequence();
            ck_assert_ptr_ne(NULL, ref_seq);

            // 5 translations
            for (int j = 0; j < 5; j++) {
                p_sdb_sequence trans_seq = sdb_next_sequence();
                ck_assert_ptr_ne(NULL, trans_seq);

                ck_assert_ptr_eq(ref_seq->info, trans_seq->info);
            }
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

