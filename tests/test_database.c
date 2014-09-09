/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"

typedef struct seqinfo_s seqinfo_t;

extern void db_read(const char * filename);
extern unsigned long db_getsequencecount();
extern unsigned long db_getnucleotidecount();
extern unsigned long db_getlongestheader();
extern unsigned long db_getlongestsequence();
extern seqinfo_t * db_getseqinfo(unsigned long seqno);
extern char * db_getsequence(unsigned long seqno);
extern void db_getsequenceandlength(unsigned long seqno, char ** address,
        long * length);
extern unsigned long db_getsequencelen(unsigned long seqno);
extern char * db_getheader(unsigned long seqno);
extern unsigned long db_getheaderlen(unsigned long seqno);
extern void db_free();

START_TEST (test_database_read)
    {
        db_read("tests/testdata/AF091148.fas");

        ck_assert_int_eq(1403, db_getsequencecount());

        db_free();
    }END_TEST

START_TEST (test_database_read_double)
    {
        db_read("tests/testdata/double.fas");
    }END_TEST

START_TEST (test_database_read_longest)
    {
        db_read("tests/testdata/test.fas");

        ck_assert_int_eq(5, db_getsequencecount());

        ck_assert_int_eq(232, db_getlongestsequence());
        ck_assert_int_eq(44, db_getlongestheader());

        db_free();
    }END_TEST

START_TEST (test_database_read_nuc_count)
    {
        db_read("tests/testdata/nuc_count.fas");

        ck_assert_int_eq(2, db_getsequencecount());

        ck_assert_int_eq(361, db_getnucleotidecount());

        db_free();
    }END_TEST

START_TEST (test_database_read_seq_data)
    {
        db_read("tests/testdata/test.fas");

        ck_assert_int_eq(5, db_getsequencecount());

        ck_assert_str_eq("188ee1b70ecd645cde302ae9c41d36d4", db_getheader(3));
        ck_assert_str_eq(
                "gtcgctcctaccgattgaatgcgttggtgattgaattggataaagagatatcatcttaaatgatagcaaagcggtaaacatttgtaaactagattatttagaggaaggagaagtcgtaacaaggtttcc",
                db_getsequence(3));
        ck_assert_int_eq(32, db_getheaderlen(3));
        ck_assert_int_eq(120, db_getsequencelen(4));

        long length = 0;
        char* sequence = 0;
        db_getsequenceandlength(4, &sequence, &length);

        ck_assert_int_eq(120, length);
        ck_assert_str_eq(sequence, db_getsequence(4));

//        // TODO shows the addresses of the symbols in sym_nt???
//        showseq("gtcgctcctaccgattg");
//        outf("\n");

        db_free();
    }END_TEST

START_TEST (test_database_read_seq_Aminoacid)
    {
        db_read("tests/testdata/NP_009305.1.fas");
        ck_assert_str_eq(
                "gi|6226519|ref|NP_009305.1| cytochrome-c oxidase subunit I; Cox1p",
                db_getheader(0));
        ck_assert_int_eq(65, db_getheaderlen(0));
        ck_assert_int_eq(316, db_getsequencelen(0));

        db_free();
    }END_TEST

void addDatabaseTC(Suite *s) {
    init_out(NULL);

    TCase *tc_core = tcase_create("database");
    tcase_add_test(tc_core, test_database_read);
    tcase_add_test(tc_core, test_database_read_longest);
    tcase_add_test(tc_core, test_database_read_nuc_count);
    tcase_add_test(tc_core, test_database_read_seq_data);
    tcase_add_test(tc_core, test_database_read_seq_Aminoacid);
    tcase_add_test(tc_core, test_database_read_double);

    suite_add_tcase(s, tc_core);
}

