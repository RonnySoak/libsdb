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

p_sdb_sequence* external_test_sequences;
int idx = 0;

p_sdb_sequence test_external_next() {
    return external_test_sequences[idx++];
}

START_TEST (test_external_next_function)
    {
        external_test_sequences = (p_sdb_sequence*) xmalloc(
                3 * sizeof(sdb_sequence));

        external_test_sequences[0]->info = 0;
        external_test_sequences[0]->len = 5;
        external_test_sequences[0]->seq = "ATCGA";
        external_test_sequences[0]->strand = 0;
        external_test_sequences[0]->frame = 0;

        external_test_sequences[0]->info = 0;
        external_test_sequences[0]->len = 4;
        external_test_sequences[0]->seq = "AAGC";
        external_test_sequences[0]->strand = 1;
        external_test_sequences[0]->frame = 0;

        external_test_sequences[0]->info = 0;
        external_test_sequences[0]->len = 6;
        external_test_sequences[0]->seq = "AAGGCT";
        external_test_sequences[0]->strand = 0;
        external_test_sequences[0]->frame = 0;

        sdb_init_out(NULL);

        sdb_init_external(test_external_next);

        // checks if it can read all sequences and its translations
        for (int i = 0; i < 3; i++) {
            ck_assert_ptr_eq(external_test_sequences[i], sdb_next_sequence());
        }

        // end reached?
        ck_assert_ptr_eq(NULL, sdb_next_sequence());

        sdb_free_db();
    }END_TEST

void addLibSDBTC(Suite *s) {
    TCase *tc_core = tcase_create("libsdb");
    tcase_add_test(tc_core, test_one_sequence);
    tcase_add_test(tc_core, test_multiple_sequences_translated);
    tcase_add_test(tc_core, test_external_next_function);

    suite_add_tcase(s, tc_core);
}

