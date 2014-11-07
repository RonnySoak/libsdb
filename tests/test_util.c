/*
 * test_search63.
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "tests.h"

#include "../src/util.h"
#include "../src/libsdb.h"

START_TEST (test_xmalloc)
    {
        uint8_t * arr = xmalloc(5);
        for (int i = 0; i < 5; ++i) {
            arr[i] = 1;
        }

        p_seqinfo * arr2 = xmalloc(5 * sizeof(p_seqinfo));
        for (int i = 0; i < 5; ++i) {
            arr2[i] = xmalloc(sizeof(seqinfo));
            arr2[i]->ID = i;
        }

        for (int i = 0; i < 5; ++i) {
            free(arr2[i]);
        }
        free(arr2);
        free(arr);
    }END_TEST


START_TEST (test_xrealloc)
    {
        p_seqinfo * arr = xmalloc(5 * sizeof(p_seqinfo));
        for (int i = 0; i < 5; ++i) {
            arr[i] = xmalloc(sizeof(seqinfo));
            arr[i]->ID = i;
        }

        arr = xrealloc(arr, 10 * sizeof(p_seqinfo));
        for (int i = 0; i < 10; ++i) {
            if(i >= 5) {
                arr[i] = xmalloc(sizeof(seqinfo));
                arr[i]->ID = i;
            }
        }

        for (int i = 0; i < 10; ++i) {
            free(arr[i]);
        }
        free(arr);
    }END_TEST

void addUtilTC(Suite *s) {
    TCase *tc_core = tcase_create("util");
    tcase_add_test(tc_core, test_xmalloc);
    tcase_add_test(tc_core, test_xrealloc);

    suite_add_tcase(s, tc_core);
}

