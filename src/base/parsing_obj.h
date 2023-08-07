#ifndef SRC_BASE_PARSING_OBJ_H
#define SRC_BASE_PARSING_OBJ_H

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vertex {
  int amount_vert;
  double *coord;
  double minMaxX[2];
  double minMaxY[2];
  double minMaxZ[2];
} Vertex;

typedef struct Polygons {
  int *p;
  int amount_p;
} Polygons;

typedef struct Main_parser {
  Vertex *v;
  Polygons *p;
  int *amount_polygons;
} Main_parser;

int parser(char *file, struct Vertex *v_out, struct Polygons **polygons_arr1,
           int *parr_size);

int parse_v(char *line, Vertex *v, int *amount_rows);
void set_extrema(Vertex *vertex, int local_i, double num_val);
int parse_f(struct Polygons **polygons_arr, char *line, int *polygons_count,
             Vertex v, int *allocated_rows);
void kill_structs(Vertex vertex, Polygons *polygons, int polygons_size);

#endif  // SRC_BASE_PARSING_OBJ_H
