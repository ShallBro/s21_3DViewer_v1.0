#include "parsing_obj.h"

#include <locale.h>

#include "affine.h"

int parser(char *filename, Vertex *vertex, Polygons **polygons,
           int *polygons_size) {
  setlocale(LC_ALL, "en_US.UTF-8");
  int error = 0;
  Vertex vertex_local;
  vertex_local.amount_vert = 0;
  int amount_rows = 0;
  int allocated_rows = 0;
  struct Polygons *polygons_local = NULL;
  int polygons_count = 0;
  vertex_local.coord = NULL;

  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    error = 1;
  } else {
    char line[1024] = {'\0'};
    while (fgets(line, 1024, file) != NULL && !error) {
      if (line[0] == '#') {
        continue;
      } else if (line[0] == 'v' && line[1] == ' ') {
        error = parse_v(line, &vertex_local, &amount_rows);
      } else if (line[0] == 'f') {
        error = parse_f(&polygons_local, line, &polygons_count, vertex_local,
                        &allocated_rows);
      }
    }
    fclose(file);
  }
  *vertex = vertex_local;
  *polygons = polygons_local;
  *polygons_size = polygons_count;
  if (vertex_local.amount_vert == 0) error = 1;
  return error;
}

int parse_v(char *line, Vertex *vertex, int *line_counter) {
  int error = 0;
  line += strcspn(line, "-0123456789");
  int vertices_count = 0;
  int local_i = 0;
  while (line != NULL && *line != '\0') {
    local_i++;
    vertex->amount_vert++;
    vertices_count++;
    int shift = strspn(line, "-0123456789.eE");
    char str_val[shift + 1];
    str_val[shift] = '\0';
    strncpy(str_val, line, shift);
    double num_val = atof(str_val);
    line += shift;
    line = strchr(line, ' ');
    if (line != NULL) line += strcspn(line, "-0123456789");
    if (vertex->amount_vert > *line_counter) {
      *line_counter += 1000;
      vertex->coord =
          (double *)realloc(vertex->coord, *line_counter * sizeof(double));
    }
    vertex->coord[vertex->amount_vert - 1] = num_val;
    set_extrema(vertex, local_i, num_val);
  }
  if (vertices_count != 3) error = 1;
  return error;
}

void set_extrema(Vertex *vertex, int local_i, double num_val) {
  if (vertex->amount_vert <= 3) {
    if (local_i % 3 == 1) {
      vertex->minMaxX[0] = num_val;
      vertex->minMaxX[1] = num_val;
    }
    if (local_i % 3 == 2) {
      vertex->minMaxY[0] = num_val;
      vertex->minMaxY[1] = num_val;
    }
    if (local_i % 3 == 0) {
      vertex->minMaxZ[0] = num_val;
      vertex->minMaxZ[1] = num_val;
    }
  } else {
    if (local_i % 3 == 1) {
      if (num_val < vertex->minMaxX[0]) vertex->minMaxX[0] = num_val;
      if (num_val > vertex->minMaxX[1]) vertex->minMaxX[1] = num_val;
    }
    if (local_i % 3 == 2) {
      if (num_val < vertex->minMaxY[0]) vertex->minMaxY[0] = num_val;
      if (num_val > vertex->minMaxY[1]) vertex->minMaxY[1] = num_val;
    }
    if (local_i % 3 == 0) {
      if (num_val < vertex->minMaxZ[0]) vertex->minMaxZ[0] = num_val;
      if (num_val > vertex->minMaxZ[1]) vertex->minMaxZ[1] = num_val;
    }
  }
}

int parse_f(struct Polygons **polygons_arr, char *line, int *polygons_count,
            Vertex vertex, int *allocated_rows) {
  (*polygons_count)++;
  line += strcspn(line, "-0123456789");
  int size = 0;
  int *p = NULL;
  int error = 0;

  while (line != NULL && *line != '\0') {
    int shift = strspn(line, "-0123456789");
    char str_val[shift + 1];
    strncpy(str_val, line, shift);
    str_val[shift] = '\0';
    int num_val = atoi(str_val);
    int *tmp = (int *)realloc(p, (size + 1) * sizeof(int));
    if (tmp)
      p = tmp;
    else {
      error = 1;
      free(p);
      break;
    }
    if (num_val < 0) {
      p[size++] = vertex.amount_vert / 3 + 1 + num_val;
    } else
      p[size++] = num_val - 1;
    line += shift;
    line = strchr(line, ' ');
    if (line != NULL) line += strcspn(line, "-0123456789");
  }
  if (!error) {
    size *= 2;
    int *p2 = NULL;
    p2 = (int *)calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
      if (i == 0 || i == size - 1)
        p2[i] = p[0];
      else {
        p2[i] = p[(i + 1) / 2];
      }
    }
    if (*polygons_count > *allocated_rows) {
      *allocated_rows += 1000;
      *polygons_arr = (struct Polygons *)realloc(
          (*polygons_arr), *allocated_rows * sizeof(struct Polygons));
    }
    (*polygons_arr)[*polygons_count - 1].amount_p = size;
    (*polygons_arr)[*polygons_count - 1].p = p2;
    free(p);
  }
  return error;
}

void kill_structs(Vertex vertex, Polygons *polygons, int polygons_size) {
  free(vertex.coord);
  vertex.coord = NULL;
  for (int i = 0; i < polygons_size; i++) {
    free(polygons[i].p);
  }
  free(polygons);
}
