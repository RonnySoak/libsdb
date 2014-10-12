/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"
#include "../src/libsdb.h"
#include "../src/sdb_error.h"
#include "../src/database.h"

START_TEST (test_database_read)
    {
        db_open("tests/testdata/AF091148.fas");
        db_read();

        ck_assert_int_eq(1403, db_getsequencecount());

        db_free();
    }END_TEST

//START_TEST (test_database_not_found)
//    {
//        db_read("tests/testdata/some_impossible_name_foo.bar");
//
//        db_read();
//
//        ck_assert_int_eq(0, db_getsequencecount());
//
//        sdb_error err = sdb_last_error();
//        ck_assert_ptr_eq(NULL, &err);
//        ck_assert_str_eq("Unable to open input data file", sdb_get_error_desc(err));
//
//        db_free();
//    }END_TEST

START_TEST (test_database_read_double)
    {
        db_open("tests/testdata/double.fas");
        db_read();
    }END_TEST

START_TEST (test_database_seqinfo)
    {
        db_open("tests/testdata/test.fas");
        db_read();

        ck_assert_int_eq(5, db_getsequencecount());

        p_seqinfo info = db_getseqinfo(0);

        ck_assert_int_eq(db_getsequence(0), info->seq);
        ck_assert_int_eq(db_getsequencelen(0), info->seqlen);
        ck_assert_int_eq(db_getheader(0), info->header);
        ck_assert_int_eq(db_getheaderlen(0), info->headerlen);

        db_free();
    }END_TEST

START_TEST (test_database_read_longest)
    {
        db_open("tests/testdata/test.fas");
        db_read();

        ck_assert_int_eq(5, db_getsequencecount());

        ck_assert_int_eq(232, db_getlongestsequence());
        ck_assert_int_eq(44, db_getlongestheader());

        db_free();
    }END_TEST

START_TEST (test_database_read_nuc_count)
    {
        db_open("tests/testdata/nuc_count.fas");
        db_read();

        ck_assert_int_eq(2, db_getsequencecount());

        ck_assert_int_eq(361, db_getnucleotidecount());

        db_free();
    }END_TEST

START_TEST (test_database_read_seq_data)
    {
        db_open("tests/testdata/test.fas");
        db_read();

        ck_assert_int_eq(5, db_getsequencecount());

        ck_assert_str_eq("188ee1b70ecd645cde302ae9c41d36d4", db_getheader(3));
        ck_assert_str_eq(
                "gtcgctcctaccgattgaatgcgttggtgattgaattggataaagagatatcatcttaaatgatagcaaagcggtaaacatttgtaaactagattatttagaggaaggagaagtcgtaacaaggtttcc",
                db_getsequence(3));
        ck_assert_int_eq(32, db_getheaderlen(3));
        ck_assert_int_eq(120, db_getsequencelen(4));

        unsigned long length = 0;
        char* sequence = 0;
        db_getsequenceandlength(4, &sequence, &length);

        ck_assert_int_eq(120, length);
        ck_assert_str_eq(sequence, db_getsequence(4));

        db_free();
    }END_TEST

START_TEST (test_database_read_seq_Aminoacid)
    {
        db_open("tests/testdata/NP_009305.1.fas");
        db_read();
        ck_assert_str_eq(
                "gi|6226519|ref|NP_009305.1| cytochrome-c oxidase subunit I; Cox1p",
                db_getheader(0));
        ck_assert_int_eq(65, db_getheaderlen(0));
        ck_assert_int_eq(534, db_getsequencelen(0));

        db_free();
    }END_TEST

void addDatabaseTC(Suite *s) {
    TCase *tc_core = tcase_create("database");
    tcase_add_test(tc_core, test_database_read);
//    tcase_add_test(tc_core, test_database_not_found);
    tcase_add_test(tc_core, test_database_seqinfo);
    tcase_add_test(tc_core, test_database_read_longest);
    tcase_add_test(tc_core, test_database_read_nuc_count);
    tcase_add_test(tc_core, test_database_read_seq_data);
    tcase_add_test(tc_core, test_database_read_seq_Aminoacid);
    tcase_add_test(tc_core, test_database_read_double);

    suite_add_tcase(s, tc_core);
}

