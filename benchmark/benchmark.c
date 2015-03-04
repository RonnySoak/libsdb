/*
 * libssa_example.c
 *
 *  Created on: Sep 4, 2014
 *      Author: Jakob Frielingsdorf
 */

#include <stdio.h>

#include "../src/libsdb.h"

int main(int argc, char**argv) {
    /*
     * Using UniProtKB/Swiss-Prot database.
     *
     * Can be downloaded from here: http://www.uniprot.org/downloads
     *
     * Compile it with:
     * gcc -O3 -o benchmark benchmark.c --coverage -L.. -lsdb TODO remove --coverage from libsdb to remove it here as well
     */
    sdb_init_fasta("uniprot_sprot.fasta");

    p_seqinfo info = sdb_next_sequence();
    long count = 0;
    while (info) {
//        printf("sequence: %ld - header: %s\n", info->ID, info->header);

        info = sdb_next_sequence();
        count++;
    }

    printf("Nr of sequences read: %ld\n", count);

    sdb_close();

    return 0;
}
