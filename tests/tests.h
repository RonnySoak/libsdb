/*
 * check_liborsa.h
 *
 *  Created on: 20 Jul 2014
 *      Author: kaos
 */

#ifndef CHECK_LIBORSA_H_
#define CHECK_LIBORSA_H_

#include <check.h>

void addSdbErrorTC( Suite *s );
void addUtilTC(Suite *s);
void addDatabaseTC(Suite *s);
void addLibSDBTC(Suite *s);
void addLibSSAExternDBTC(Suite *s);

#endif /* CHECK_LIBORSA_H_ */
