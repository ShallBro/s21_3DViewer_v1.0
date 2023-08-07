#ifndef SRC_TESTS_UNIT_TEST_H
#define SRC_TESTS_UNIT_TEST_H

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../base/affine.h"
#include "../base/parsing_obj.h"

#define SUCCESS 1
#define FAILURE 0

Suite *suite_parsing_obj(void);
Suite *suite_affine(void);

void run_testcase(Suite *testcase);

#endif  // SRC_TESTS_UNIT_TEST_H
