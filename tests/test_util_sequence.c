/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"
#include "../src/util.h"

typedef struct seqinfo_s seqinfo_t;

extern char map_ncbi_nt16[256];
extern const char * sym_ncbi_aa;

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
        char ntcompl[16] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7,
                15 };

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

void ck_converted_prot_eq(char* ref, char* protp, int plen) {
    char conv_aa[plen + 1];
    for (int i = 0; i < plen; i++) {
        conv_aa[i] = sym_ncbi_aa[(int) protp[i]];
    }
    conv_aa[plen] = 0;

    ck_assert_str_eq(ref, conv_aa);
}

START_TEST (test_translate_query_RNA)
    {
        us_init_translation(3, 1);

        char* dna = "AUGCCCAAGCUGAAUAGCGUAGAGGGGUUUUCAUCAUUUGAGGACGAUGUAUAA";
        int dlen = strlen(dna);
        char* protp;
        long plen;

        char conv_dna[dlen + 1];
        for (int i = 0; i < dlen; i++) {
            conv_dna[i] = map_ncbi_nt16[(int) dna[i]];
        }
        conv_dna[dlen] = 0;

        us_translate_sequence(0, conv_dna, dlen, 0, 0, &protp, &plen);

        ck_assert_int_eq(18, plen);
        ck_converted_prot_eq("MPKTNSVEGFSSFEDDV*", protp, plen);
        free(protp);
    }END_TEST

START_TEST (test_translate_query)
    {
        us_init_translation(3, 1);

        char* dna = "ATGCCCAAGCTGAATAGCGTAGAGGGGTTTTCATCATTTGAGGACGATGTATAA";
        int dlen = strlen(dna);
        char* protp;
        long plen;

        char conv_dna[dlen + 1];
        for (int i = 0; i < dlen; i++) {
            conv_dna[i] = map_ncbi_nt16[(int) dna[i]];
        }
        conv_dna[dlen] = 0;

        // forward strand
        // frame 1
        us_translate_sequence(0, conv_dna, dlen, 0, 0, &protp, &plen);

        ck_assert_int_eq(18, plen);
        ck_converted_prot_eq("MPKTNSVEGFSSFEDDV*", protp, plen);
        free(protp);

        // frame 2
        us_translate_sequence(0, conv_dna, dlen, 0, 1, &protp, &plen);

        ck_assert_int_eq(17, plen);
        ck_converted_prot_eq("CPSWMA*RGFHHLRTMY", protp, plen);
        free(protp);

        // frame 3
        us_translate_sequence(0, conv_dna, dlen, 0, 2, &protp, &plen);
        ck_assert_int_eq(17, plen);
        ck_converted_prot_eq("AQAE*RRGVFIIWGRCM", protp, plen);
        free(protp);

        // complementary strand
        // frame 1
        us_translate_sequence(0, conv_dna, dlen, 1, 0, &protp, &plen);

        ck_assert_int_eq(18, plen);
        ck_converted_prot_eq("LYIVTKWWKPTYAIQTGH", protp, plen);
        free(protp);

        // frame 2
        us_translate_sequence(0, conv_dna, dlen, 1, 1, &protp, &plen);

        ck_assert_int_eq(17, plen);
        ck_converted_prot_eq("YTSSSNDENPSTTFSLG", protp, plen);
        free(protp);

        // frame 3
        us_translate_sequence(0, conv_dna, dlen, 1, 2, &protp, &plen);
        ck_assert_int_eq(17, plen);
        ck_converted_prot_eq("MHRPQMMKTPTRYSAWA", protp, plen);

        free(protp);
    }END_TEST

START_TEST (test_translate_query_DNA)
    {
        us_init_translation(3, 1);

        char* dna = "ATGCCCAAGCTGAATAGCGTAGAGGGGTTTTCATCATTTGAGGACGATGTATAA";
        int dlen = strlen(dna);
        char* protp;
        long plen;

        char conv_dna[dlen + 1];
        for (int i = 0; i < dlen; i++) {
            conv_dna[i] = map_ncbi_nt16[(int) dna[i]];
        }
        conv_dna[dlen] = 0;

        us_translate_sequence(0, conv_dna, dlen, 0, 0, &protp, &plen);

        ck_assert_int_eq(18, plen);
        ck_converted_prot_eq("MPKTNSVEGFSSFEDDV*", protp, plen);
        free(protp);
    }END_TEST

START_TEST (test_translate_db)
    {
        us_init_translation(1, 3);

        char* dna = "ATGCCCAAGCTGAATAGCGTAGAGGGGTTTTCATCATTTGAGGACGATGTATAA";
        int dlen = strlen(dna);
        char* protp;
        long plen;

        char conv_dna[dlen + 1];
        for (int i = 0; i < dlen; i++) {
            conv_dna[i] = map_ncbi_nt16[(int) dna[i]];
        }
        conv_dna[dlen] = 0;

        // translate it as a db sequence, using the db translation table
        us_translate_sequence(1, conv_dna, dlen, 0, 0, &protp, &plen);

        ck_assert_int_eq(18, plen);
        ck_converted_prot_eq("MPKTNSVEGFSSFEDDV*", protp, plen);
        free(protp);
    }END_TEST

void addUtilSequenceTC(Suite *s) {
    TCase *tc_core = tcase_create("util_sequence");
    tcase_add_test(tc_core, test_revcompl);
    tcase_add_test(tc_core, test_translate_query);
    tcase_add_test(tc_core, test_translate_query_DNA);
    tcase_add_test(tc_core, test_translate_query_RNA);
    tcase_add_test(tc_core, test_translate_db);

    suite_add_tcase(s, tc_core);
}

