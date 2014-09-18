/*
 * sdb_datatypes.h
 *
 *  Created on: Sep 8, 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef SDB_DATATYPES_H_
#define SDB_DATATYPES_H_

struct seqinfo {
    char * header;
    char * seq;
    unsigned long headerlen;
    unsigned long headeridlen;
    unsigned long seqlen;
};

typedef struct seqinfo* p_seqinfo;

/** @typedef    structure of a sequence
 *
 * @field seq   the sequence
 * @field len   length of the sequence
 */
struct sdb_sequence {
    p_seqinfo* info;
    char* seq;
    long len;
};

typedef struct sdb_sequence* p_sdb_sequence;

#endif /* SDB_DATATYPES_H_ */
