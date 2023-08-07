#include "./unit_tests.h"

START_TEST(test1) {
  Vertex vertex;
  Polygons *polygons = NULL;
  int polygons_size = 0;
  char *file = "../data-samples/cube.obj";
  ck_assert_int_eq(parser(file, &vertex, &polygons, &polygons_size), 0);
  kill_structs(vertex, polygons, polygons_size);
}
END_TEST

Suite *suite_parsing_obj(void) {
  Suite *s = suite_create("suite_parsing_obj");

  TCase *tc = tcase_create("test_case_parsing_obj");
  tcase_add_test(tc, test1);

  suite_add_tcase(s, tc);
  return s;
}
