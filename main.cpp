//
//
//                bahamut.h is the graphics library. Include it in all header files
//
//

#include "ENGINE/bahamut.h"
#include "map.h"
#include <time.h>
#include <stdlib.h>

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#define RENDER_WIDTH 1015.0f
#define RENDER_HEIGHT 795.0f

bool initialize(int argc, char* argv[], int* width, int* height);
int heuristic(Map* map, int x, int y);

int main(int argc, char* argv[]) {
    int mapWidth;
    int mapHeight;
    bool edit = initialize(argc, argv, &mapWidth, &mapHeight);

    RenderBatch* batch = &create_batch();
    batch->shader = load_default_shader_2D();
    start_shader(batch->shader);
    upload_mat4(batch->shader, "projection", orthographic_projection(0, 0, RENDER_WIDTH, RENDER_HEIGHT, -1, 1));

    MapScene* scene = &load_scene();
    Map* map = &create_map(15, 15); //MAX SIZE = (28, 22)

    if(edit)
        map = &create_map(mapWidth, mapHeight);

    std::vector<vec2> path;
    /*
    if( !pathfind(map, {0.0f, 0.0f}, {0.0f, 9.0f}, heuristic, &path) ) {
        printf("Pathfind failed!\n");
    }
    */
    path = floodfill(map, {2, 7}, 3, heuristic);

    while(window_open()) {
        Rect view = fit_aspect_ratio(RENDER_WIDTH / RENDER_HEIGHT);
        vec2 mouse = get_mouse_pos(view);

        begin_drawing();
        set_viewport(view.x, view.y, view.width, view.height);

        begin2D(batch);

        if(edit) {
            
        }
        draw_map(batch, map, scene);
        for(vec2 v : path)
            draw_rectangle(batch, v.x * (TILE_SIZE+OFFSET), v.y * (TILE_SIZE+OFFSET), TILE_SIZE, TILE_SIZE, {50, 150, 50, 50});

        end2D(batch);

        end_drawing();
    }

    dispose_batch(batch);
    dispose_window();

    return 0;
}

int heuristic(Map* map, int x, int y) {
    TileType type = map->grid[x + y * map->width].type;
    int owner = map->grid[x + y * map->width].owner;

    if(type != EMPTY)
        return -1;
    return 1;
}

bool initialize(int argc, char* argv[], int* width, int* height) {
    bool edit = false;
    if(argc == 4) {
        if(argv[1][0] == '-' && argv[1][1] == 'e') {
            *width = atoi(argv[2]);
            *height = atoi(argv[3]);
            edit = true;
        }
    }
    srand(time(NULL));
    printf("\n/////////////////////////////////\n        PROGRAM STARTING\n/////////////////////////////////\n\n");
    init_window(RENDER_WIDTH, RENDER_HEIGHT, "Simple Strategy Game", false, true, true);
    init_audio();

    set_fps_cap(60);
    set_master_volume(100); //0-255
    set_vsync(true);

    return edit;
}
