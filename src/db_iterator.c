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

extern void us_translate_sequence(int db_sequence, char * dna, long dlen,
        long strand, long frame, char ** protp, long * plenp);
extern char* us_revcompl(char* seq, long len);

extern char map_ncbi_nt16[256];

static unsigned long seq_index;

static int buffer_max = 0;
static p_sdb_sequence* buffer;
static int buffer_p = 0;

/**
 * Initialises the buffer. Translates the DB sequence and computes the reverse
 * complement of the forward strand, if necessary.
 */
static void fill_buffer(p_seqinfo seqinfo) {
    char* seq = seqinfo->seq;
    long len = (long) (seqinfo->seqlen);

    if (symtype == NUCLEOTIDE) {
        // first element
        buffer[0]->info = seqinfo;
        buffer[0]->len = len;
        buffer[0]->seq = seq;

        if (query_strands & 2) {
            // reverse complement
            buffer[1]->info = seqinfo;
            buffer[1]->seq = us_revcompl(seq, len);
            buffer[1]->len = len;
        }
    }
    else if ((symtype == TRANS_DB) || (symtype == TRANS_BOTH)) {
        char conv_seq[len + 1];
        for (int i = 0; i < len; i++) {
            conv_seq[i] = map_ncbi_nt16[(int) seq[i]];
        }
        conv_seq[len] = 0;

        if (query_strands ^ BOTH_STRANDS) {
            // for forward or complementary strand only
            int s = query_strands - 1;

            for (long f = 0; f < 3; f++) { // frames
                buffer[f]->info = seqinfo;

                us_translate_sequence(1, conv_seq, len, s, f,
                        &buffer[f]->seq,
                        &buffer[f]->len);
            }
        }
        else {
            // for both strands translation
            for (long s = 0; s < 2; s++) { // strands
                for (long f = 0; f < 3; f++) { // frames
                    buffer[3 * s + f]->info = seqinfo;

                    us_translate_sequence(1, conv_seq, len, s, f,
                            &buffer[3 * s + f]->seq,
                            &buffer[3 * s + f]->len);
                }
            }
        }
    }
    else {
        buffer[0]->info = seqinfo;
        buffer[0]->len = len;
        buffer[0]->seq = seq;
    }
}

void it_free() {
    buffer_max = 0;
    buffer_p = 0;

    if (buffer) {
        for (int i = 0; i < buffer_max; i++) {
            if(buffer[i]) {
                /* the first sequence is initialised by the database code,
                 * so it has to be freed there */
                if ((i > 0) && buffer[i]->seq) {
                    free(buffer[i]->seq);
                }
                buffer[i]->seq = 0;
                buffer[i]->len = 0;
                buffer[i]->info = 0;

                free(buffer[i]);
            }
        }
        free(buffer);
    }
}

void it_init() {
    // reset the DB sequence index
    seq_index = 0;

    // set buffer size according symtype: 1, 2 oder 6
    if (symtype == NUCLEOTIDE) {
        if (query_strands & 2)
            buffer_max = 2;
        else
            buffer_max = 1;
    }
    else if ((symtype == TRANS_DB) || (symtype == TRANS_BOTH)) {
        if (query_strands ^ BOTH_STRANDS)
            buffer_max = 3;
        else
            buffer_max = 6;
    }
    else {
        buffer_max = 1;
    }

    // at first we start with a "full" buffer, to force a sequence loading!
    buffer_p = buffer_max;

    buffer = xmalloc(buffer_max * sizeof(sdb_sequence));

    for (int i = 0; i < buffer_max; i++) {
        buffer[i] = (p_sdb_sequence )xmalloc(sizeof(sdb_sequence));

        buffer[i]->info = 0;
        buffer[i]->len = 0;
        buffer[i]->seq = 0;
    }
}

/**
 * Returns the next sequence from the database. Translates the DB sequences, if
 * necessary, and returns the translated sequences one by one.
 *
 * @return the next sequence in the DB, or NULL if none is left
 */
p_sdb_sequence it_next() {
    if (buffer_p < buffer_max) {
        return buffer[buffer_p++];
    }

    if (seq_index >= db_getsequencecount()) {
        // last sequence read
        return NULL;
    }

    // get sequence from DB
    p_seqinfo seqinfo = db_getseqinfo(seq_index++);

    // translate sequence according sym-type
    fill_buffer(seqinfo);

    // return current version
    buffer_p = 0;
    return buffer[buffer_p++];
}
