#ifndef APP_H
#define APP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

struct app_s;
typedef struct app_s app_t;

void Input(app_t *app);
void Update(app_t *app);
void Render(app_t *app);

#endif