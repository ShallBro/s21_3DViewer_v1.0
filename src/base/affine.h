#ifndef SRC_BASE_AFFINE_H
#define SRC_BASE_AFFINE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "parsing_obj.h"

void center(Vertex *inp);
void moveOX(Vertex *inp, double value);
void moveOY(Vertex *inp, double value);
void moveOZ(Vertex *inp, double value);
void normalize_size(Vertex *inp);
void scale(Vertex *inp, double value);
void rotateOX(Vertex *inp, double angle);
void rotateOY(Vertex *inp, double angle);
void rotateOZ(Vertex *inp, double angle);

#endif  // SRC_BASE_AFFINE_H