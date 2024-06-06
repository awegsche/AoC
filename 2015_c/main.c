#include <raylib.h>
#include <stdio.h>

#include "day05.h"
#include "log.h"

const int STEPTIME = 1;

int main(int argc, char **argv) {
    printf("AoC 2015 -- in C\n");

    // check if we are in the right dir
    FILE *file = fopen("2015/input/day01.txt", "r");
    if (!file) {
        fprintf(stderr,
                "Couldn't load input file, are we in the right directory?\n");
        return 1;
    }
    fclose(file);

    const int width  = 1280;
    const int height = 960;

    InitWindow(width, height, "AoC 2015 -- in C");
    LogManager logs = log_manager_init();

    Font font =
        LoadFontEx("resources/fira_code/ttf/FiraCode-SemiBold.ttf", 128, 0, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    log_manager_set_font(font);

    SetTargetFPS(60);

    do_day5(&logs);

    CloseWindow();

    log_manager_cleanup(&logs);
    UnloadFont(font);
    return 0;
}
