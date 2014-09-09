/*
 * db_iterator.c
 *
 *  Created on: Sep 8, 2014
 *      Author: Jakob Frielingsdorf
 */

#include "libsdb.h"
#include "util.h"
#include "sdb_datatypes.h"

extern unsigned long db_getsequencecount();
extern p_seqinfo db_getseqinfo(unsigned long seqno);

unsigned long seq_index = 0;

int buffer_max = 1;
p_sequence* buffer = 0;
int buffer_p = 0;

void init_iterator() {
    // set buffer size according symtype: 1, 2 oder 6
}

p_sequence getnext() {
    if(buffer_p < buffer_max) {
        return buffer[buffer_p++];
    }

    if(seq_index >= db_getsequencecount()) {
        // last sequence read
        return NULL;
    }

    // get sequence from DB

    p_seqinfo seqinfo = db_getseqinfo(seq_index++);

    char* seq = seqinfo->seq;
    long len = (long) (seqinfo->seqlen);

    // translate sequence according sym-type
    // TODO

    // return current version
    buffer_p = 0;
    return buffer[buffer_p++];
}
