#include "affine.h"

void center(Vertex *inp) {
  double xShift = (inp->minMaxX[0] + inp->minMaxX[1]) / 2;
  double yShift = (inp->minMaxY[0] + inp->minMaxY[1]) / 2;
  double zShift = (inp->minMaxZ[0] + inp->minMaxZ[1]) / 2;
  moveOX(inp, -xShift);
  moveOY(inp, -yShift);
  moveOZ(inp, -zShift);
}

void moveOX(Vertex *inp, double value) {
  for (int i = 0; i < inp->amount_vert; i++) {
    if (i % 3 == 0) inp->coord[i] += value;
  }
}

void moveOY(Vertex *inp, double value) {
  for (int i = 0; i < inp->amount_vert; i++) {
    if (i % 3 == 1) inp->coord[i] += value;
  }
}

void moveOZ(Vertex *inp, double value) {
  for (int i = 0; i < inp->amount_vert; i++) {
    if (i % 3 == 2) inp->coord[i] += value;
  }
}

void normalize_size(Vertex *inp) {
  double scope = 1;
  scope = fabs(inp->minMaxX[0]);
  if (fabs(inp->minMaxX[1]) > scope) scope = inp->minMaxX[1];
  if (fabs(inp->minMaxY[0]) > scope) scope = inp->minMaxY[0];
  if (fabs(inp->minMaxY[1]) > scope) scope = inp->minMaxY[1];
  if (fabs(inp->minMaxZ[0]) > scope) scope = inp->minMaxZ[0];
  if (fabs(inp->minMaxZ[1]) > scope) scope = inp->minMaxZ[1];
  scale(inp, 2.5 / scope);
}

void scale(Vertex *inp, double value) {
  if (value == 0) value = 1;
  for (int i = 0; i < inp->amount_vert; i++) {
    inp->coord[i] *= value;
  }
  inp->minMaxX[0] *= value;
  inp->minMaxX[1] *= value;
  inp->minMaxY[0] *= value;
  inp->minMaxY[1] *= value;
  inp->minMaxZ[0] *= value;
  inp->minMaxZ[1] *= value;
}

void rotateOX(Vertex *inp, double angle) {
  for (int i = 0; i < inp->amount_vert - 2; i += 3) {
    double temp_y = inp->coord[i + 1];
    double temp_z = inp->coord[i + 2];
    inp->coord[i + 1] = (cos(angle) * temp_y - sin(angle) * temp_z);
    inp->coord[i + 2] = (sin(angle) * temp_y + cos(angle) * temp_z);
  }
}

void rotateOY(Vertex *inp, double angle) {
  for (int i = 1; i < inp->amount_vert - 1; i += 3) {
    double temp_x = inp->coord[i - 1];
    double temp_z = inp->coord[i + 1];
    inp->coord[i - 1] = (cos(angle) * temp_x - sin(angle) * temp_z);
    inp->coord[i + 1] = (sin(angle) * temp_x + cos(angle) * temp_z);
  }
}

void rotateOZ(Vertex *inp, double angle) {
  for (int i = 2; i < inp->amount_vert; i += 3) {
    double temp_x = inp->coord[i - 2];
    double temp_y = inp->coord[i - 1];
    inp->coord[i - 2] = (cos(angle) * temp_x - sin(angle) * temp_y);
    inp->coord[i - 1] = (sin(angle) * temp_x + cos(angle) * temp_y);
  }
}
