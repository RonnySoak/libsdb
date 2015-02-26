/*
 * database.h
 *
 *  Created on: Oct 12, 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef DATABASE_H_
#define DATABASE_H_

void db_open( const char * filename );

/*
 * here we do not check for double sequence-headers
 */
void db_read();

void db_free();

unsigned long db_getsequencecount();

unsigned long db_getnucleotidecount();

//unsigned long db_getlongestheader();

/**
 * Returns the length of the longest sequence.
 */
unsigned long db_getlongestsequence();

p_seqinfo db_getseqinfo( unsigned long seqno );

char * db_getsequence( unsigned long seqno );

void db_getsequenceandlength( unsigned long seqno, char ** address, unsigned long * length );

unsigned long db_getsequencelen( unsigned long seqno );

//char * db_getheader( unsigned long seqno );
//
//unsigned long db_getheaderlen( unsigned long seqno );

#endif /* DATABASE_H_ */
