/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"

typedef struct seqinfo_s seqinfo_t;

extern char* us_revcompl(char* seq, long len);
extern void us_translate_sequence(int db_sequence, char * dna, long dlen,
        long strand, long frame, char ** protp, long * plenp);
extern void us_init_translation(long qtableno, long dtableno);

START_TEST (test_revcompl)
    {
        // test always the same
        int len = 4;
        char seq[] = { 1, 2, 3, 4 };

        ck_assert_str_eq(us_revcompl(seq, len), us_revcompl(seq, len));

        // test rev compl calculation
        char ntcompl[16] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };

        char rev_seq[len];

        rev_seq[0] = ntcompl[(int) (seq[3])];
        rev_seq[1] = ntcompl[(int) (seq[2])];
        rev_seq[2] = ntcompl[(int) (seq[1])];
        rev_seq[3] = ntcompl[(int) (seq[0])];

        ck_assert_str_eq(rev_seq, us_revcompl(seq, len));

        // empty
        char* seq2 = 0;

        ck_assert_ptr_eq(0, us_revcompl(seq2, 0));
    }END_TEST

START_TEST (test_translate_query)
    {

    }END_TEST

START_TEST (test_translate_db)
    {

    }END_TEST

START_TEST (test_init_translate)
    {

    }END_TEST

void addUtilSequenceTC(Suite *s) {
    init_out(NULL);

    TCase *tc_core = tcase_create("util_sequence");
    tcase_add_test(tc_core, test_revcompl);
    tcase_add_test(tc_core, test_translate_query);
    tcase_add_test(tc_core, test_translate_db);
    tcase_add_test(tc_core, test_init_translate);

    suite_add_tcase(s, tc_core);
}

