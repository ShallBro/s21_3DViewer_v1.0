#include <check.h>

#include "../base/affine.h"
#include "./unit_tests.h"

START_TEST(test1) {
  Vertex vertex;
  Polygons *polygons = NULL;
  int polygons_size = 0;
  char *file = "../data-samples/cube.obj";
  ck_assert_int_eq(parser(file, &vertex, &polygons, &polygons_size), 0);
  center(&vertex);
  normalize_size(&vertex);
  rotateOX(&vertex, 1);
  rotateOY(&vertex, 1);
  rotateOZ(&vertex, 1);
  kill_structs(vertex, polygons, polygons_size);
}

Suite *suite_affine(void) {
  Suite *s = suite_create("suite_affine");

  TCase *tc = tcase_create("test_case_affine");

  tcase_add_test(tc, test1);

  suite_add_tcase(s, tc);
  return s;
}
