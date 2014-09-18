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

static int buffer_size = 1;
static p_sdb_sequence buffer[6];
static int buffer_p = 0;

void it_init() {
    // set buffer size according symtype: 1, 2 oder 6

    if (symtype == NUCLEOTIDE) {
        buffer_size = 2;
    }
    else if ((symtype == TRANS_QUERY) || (symtype == TRANS_BOTH)) {
        buffer_size = 6;
    }
    else {
        buffer_size = 1;
    }
}

static void translate_sequence(p_seqinfo seqinfo) {
    char* seq = seqinfo->seq;
    long len = (long) (seqinfo->seqlen);

    if ((symtype == NUCLEOTIDE) || (symtype == TRANS_QUERY) || (symtype == TRANS_BOTH)) {
        // first element
        buffer[0]->info = &seqinfo;
        buffer[0]->len = len;
        buffer[0]->seq = seq;

//        if (query_strands & 2) {
//            //      outf("Reverse complement.\n");
//            query.nt[1].seq = revcompl(query.nt[0].seq, query.nt[0].len);
//            query.nt[1].len = query.nt[0].len;
//        }
//
//        if ((symtype == TRANS_QUERY) || (symtype == TRANS_BOTH)) {
//            for (long s = 0; s < 2; s++) {
//                if ((s + 1) & query_strands) {
//                    for (long f = 0; f < 3; f++) {
//                        translate(query.nt[0].seq, query.nt[0].len, s, f, 0,
//                                &query.aa[3 * s + f].seq,
//                                &query.aa[3 * s + f].len);
//                    }
//                }
//            }
//        }
    }
    else {
        buffer[0]->info = &seqinfo;
        buffer[0]->len = len;
        buffer[0]->seq = seq;
    }
}

p_sdb_sequence it_next() {
    if(buffer_p < buffer_size) {
        return buffer[buffer_p++];
    }

    if(seq_index >= db_getsequencecount()) {
        // last sequence read
        return NULL;
    }

    // get sequence from DB

    p_seqinfo seqinfo = db_getseqinfo(seq_index++);

    // translate sequence according sym-type
    translate_sequence(seqinfo);

    // return current version
    buffer_p = 0;
    return buffer[buffer_p++];
}
