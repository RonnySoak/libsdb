/*
 * sdb_error.h
 *
 *  Created on: Sep 24, 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef SDB_ERROR_H_
#define SDB_ERROR_H_

enum sdb_error {
    DB_NOT_FOUND,
    DB_NOT_CLOSED,
    DB_LINE_NOT_READ,
    DB_ILLEGAL_HEADER,
    OUT_FILE_NOT_FOUND,
    OUT_FILE_NOT_CLOSED,
    MALLOC_NOT_ENOUGH_MEM,
    REALLOC_NOT_ENOUGH_MEM
};

void set_error(enum sdb_error err);

#endif /* SDB_ERROR_H_ */
