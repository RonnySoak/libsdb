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

extern void db_read(const char * filename);
extern unsigned long db_getsequencecount();
extern char * db_getheader(unsigned long seqno);
extern p_seqinfo db_getseqinfo(unsigned long seqno);
extern char * db_getsequence(unsigned long seqno);
extern unsigned long db_getsequencelen(unsigned long seqno);
extern void db_free();

extern char* us_revcompl(char* seq, long len);
extern void us_init_translation(long qtableno, long dtableno);

extern void ck_converted_prot_eq(char* ref, char* protp, int plen);

extern void it_init();
extern void it_free();
extern p_sdb_sequence it_next();

START_TEST (test_init)
    {
        // should return doing nothing
        it_free();

        symtype = NUCLEOTIDE;
        it_init();
        it_free();

        symtype = TRANS_QUERY;
        it_init();
        it_free();

        symtype = TRANS_DB;
        it_init();
        it_free();

        symtype = TRANS_BOTH;
        it_init();
        it_free();

        symtype = AMINOACID;
        it_init();
        it_free();
    }END_TEST

START_TEST (test_next_empty)
    {
        symtype = TRANS_DB;

        it_init();

        ck_assert_ptr_eq(NULL, it_next());

        it_free();
    }END_TEST

START_TEST (test_next_one)
    {
        db_read("tests/testdata/one_seq.fas");
        ck_assert_str_eq("97485665bcded44c4d86c131ca714848", db_getheader(0));
        ck_assert_int_eq(1, db_getsequencecount());

        // test with only forward strand
        symtype = NUCLEOTIDE;
        query_strands = FORWARD_STRAND;

        it_init();

        p_sdb_sequence seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(db_getsequencelen(0), seq->len);
        ck_assert_str_eq(db_getsequence(0), seq->seq);
        ck_assert_int_eq(0, seq->strand);
        ck_assert_int_eq(0, seq->frame);

        // check for the end of sequences
        ck_assert_ptr_eq(NULL, it_next());

        it_free();

        // test both reverse complement strand creation
        symtype = NUCLEOTIDE;
        query_strands = BOTH_STRANDS;

        it_init();

        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(db_getsequencelen(0), seq->len);
        ck_assert_str_eq(db_getsequence(0), seq->seq);
        ck_assert_int_eq(0, seq->strand);
        ck_assert_int_eq(0, seq->frame);

        // get reverse complement strand
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(db_getsequencelen(0), seq->len);
        ck_assert_str_eq(us_revcompl(db_getsequence(0), seq->len), seq->seq);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(0, seq->frame);

        // check for the end of sequences
        ck_assert_ptr_eq(NULL, it_next());

        it_free();

        // test with forward strand and db translation
        symtype = TRANS_DB;
        query_strands = FORWARD_STRAND;

        us_init_translation(1, 3);

        it_init();

        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(18, seq->len);
        ck_converted_prot_eq("MPKTNSVEGFSSFEDDV*", seq->seq, seq->len);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(0, seq->frame);

        // 2.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(17, seq->len);
        ck_converted_prot_eq("CPSWMA*RGFHHLRTMY", seq->seq, seq->len);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(1, seq->frame);

        // 3.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(17, seq->len);
        ck_converted_prot_eq("AQAE*RRGVFIIWGRCM", seq->seq, seq->len);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(2, seq->frame);

        // check for the end of sequences
        ck_assert_ptr_eq(NULL, it_next());

        it_free();

        // test with both strand and db translation
        symtype = TRANS_DB;
        query_strands = BOTH_STRANDS;

        us_init_translation(1, 3);

        it_init();

        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(18, seq->len);
        ck_converted_prot_eq("MPKTNSVEGFSSFEDDV*", seq->seq, seq->len);
        ck_assert_int_eq(0, seq->strand);
        ck_assert_int_eq(0, seq->frame);

        // 2.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(17, seq->len);
        ck_converted_prot_eq("CPSWMA*RGFHHLRTMY", seq->seq, seq->len);
        ck_assert_int_eq(0, seq->strand);
        ck_assert_int_eq(1, seq->frame);

        // 3.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(17, seq->len);
        ck_converted_prot_eq("AQAE*RRGVFIIWGRCM", seq->seq, seq->len);
        ck_assert_int_eq(0, seq->strand);
        ck_assert_int_eq(2, seq->frame);

        // 4.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(18, seq->len);
        ck_converted_prot_eq("LYIVTKWWKPTYAIQTGH", seq->seq, seq->len);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(0, seq->frame);

        // 5.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(17, seq->len);
        ck_converted_prot_eq("YTSSSNDENPSTTFSLG", seq->seq, seq->len);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(1, seq->frame);

        // 6.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        ck_assert_int_eq(17, seq->len);
        ck_converted_prot_eq("MHRPQMMKTPTRYSAWA", seq->seq, seq->len);
        ck_assert_int_eq(1, seq->strand);
        ck_assert_int_eq(2, seq->frame);

        // check for the end of sequences
        ck_assert_ptr_eq(NULL, it_next());

        it_free();

        db_free();
    }END_TEST

START_TEST (test_next)
    {
        db_read("tests/testdata/test.fas");
        ck_assert_int_eq(5, db_getsequencecount());

        // test with only forward strand
        symtype = NUCLEOTIDE;
        query_strands = FORWARD_STRAND;

        it_init();

        // 1.
        p_sdb_sequence seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(0), seq->info);
        // 2.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(1), seq->info);
        // 3.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(2), seq->info);
        // 4.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(3), seq->info);
        // 5.
        seq = it_next();
        ck_assert_ptr_eq(db_getseqinfo(4), seq->info);

        // check for the end of sequences
        ck_assert_ptr_eq(NULL, it_next());

        it_free();

        db_free();
    }END_TEST

void addDBIteratorTC(Suite *s) {
    TCase *tc_core = tcase_create("db_iterator");
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_next_empty);
    tcase_add_test(tc_core, test_next_one);
    tcase_add_test(tc_core, test_next);

    suite_add_tcase(s, tc_core);
}

