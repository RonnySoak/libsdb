/*
 * libssa_example.c
 *
 *  Created on: Sep 4, 2014
 *      Author: Jakob Frielingsdorf
 */

#include <stdio.h>

#include "libsdb.h"

int main(int argc, char**argv) {
//    sdb_init_fasta("tests/testdata/test.fas");
    sdb_init_fasta("tests/testdata/Rfam_11_0.fasta");

    p_seqinfo info = sdb_next_sequence();
    long count = 0;
    while (info) {
//        printf("sequence: %ld - header: %s\n", info->ID, info->header);

        info = sdb_next_sequence();
        count++;
    }

    printf("Nr of sequences read: %ld\n", count);

    sdb_free_db();

    return 0;
}
