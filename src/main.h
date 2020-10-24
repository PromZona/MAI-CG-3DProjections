#include "app.h"

#define MAX_POINTS_COUNT 1024
#define UNIT 10

typedef struct app_s
{
    int width;  // Screen wide
    int height; // Screen high

    Vector2 shift;

    int points_count;
    Vector2 points[MAX_POINTS_COUNT];
} app_t;

void Init(app_t *app, int w, int h)
{

}

void Input(app_t *app)
{

}

void Update(app_t *app)
{

}

void Render(app_t *app)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    
    EndDrawing();
}