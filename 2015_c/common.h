#ifndef COMMON_H
#define COMMON_H

#include <raylib.h>

#define BACKGROUND    CLITERAL(Color){ 0, 20, 5, 255 }   // My own White (raylib logo)
#define FOREGROUND1   CLITERAL(Color){ 0, 245, 0, 255 }   // My own White (raylib logo)
#define FOREGROUND2   CLITERAL(Color){ 245, 245, 0, 255 }   // My own White (raylib logo)

#define AOC_BEGIN_FRAME() BeginDrawing(); ClearBackground(BACKGROUND);
#define AOC_END_FRAME(man) if(man) draw_messages(man); EndDrawing();

#endif
