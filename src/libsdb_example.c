/*
 * libssa_example.c
 *
 *  Created on: Sep 4, 2014
 *      Author: Jakob Frielingsdorf
 */

#include <stdio.h>

#include "libsdb.h"

int main(int argc, char**argv) {
    sdb_init_fasta("tests/testdata/test.fas");

    p_seqinfo info = sdb_next_sequence();
    while (info) {
        printf("sequence: %ld - header: %s\n", info->ID, info->header);

        info = sdb_next_sequence();
    }

    sdb_free_db();

    return 0;
}
