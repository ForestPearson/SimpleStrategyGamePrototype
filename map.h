#ifndef SNAKE_H
#define SNAKE_H

#include "ENGINE/bahamut.h"
#include "utils.h"
#include <vector>
#define TILE_SIZE 32
#define OFFSET 4

enum UnitType {
    MELEE,
    RANGED,
    MILITIA
}; 

struct Unit {
    int x;
    int y;
    int dmg;
    int hp;
    int range;
    UnitType type;
};

enum TileType {
    EMPTY,
    BLOCKED,
    CRYSTAL,
    TREE,
    HQ,
    FACTORY,
    TOWER
};

struct Tile {
    int owner;
    TileType type;
};

struct Map {
    int width;
    int height;
    Tile* grid;
    std::vector<Unit> units;
};

struct MapScene {
    Texture factory;
    Texture HQ;
    Texture melee;
    Texture militia;
    Texture offload;
    Texture ranged;
    Texture tower;

    Texture axe;
    Texture miner;
    Texture tree;
    Texture crystal;
};

#include "pathfinding.h"

static inline
MapScene load_scene() {
    MapScene scene = {0};
    scene.factory = load_texture("data/art/factory.png", GL_LINEAR);
    scene.HQ = load_texture("data/art/HQ.png", GL_LINEAR);
    scene.melee = load_texture("data/art/melee.png", GL_LINEAR);
    scene.militia = load_texture("data/art/militia.png", GL_LINEAR);
    scene.offload = load_texture("data/art/offload.png", GL_LINEAR);
    scene.ranged = load_texture("data/art/ranged.png", GL_LINEAR);
    scene.tower = load_texture("data/art/tower.png", GL_LINEAR);

    scene.axe = load_texture("data/art/axe.png", GL_LINEAR);
    scene.crystal = load_texture("data/art/crystal.png", GL_LINEAR);
    scene.miner = load_texture("data/art/miner.png", GL_LINEAR);
    scene.tree = load_texture("data/art/tree.png", GL_LINEAR);
    return scene;
}

static inline
Map create_map(int width, int height) {
    Map map = {0};
    map.width = width;
    map.height = height;
    map.grid = new Tile[width * height];
    for(int i = 0; i < width * height; ++i)
        map.grid[i] = {0};

    map.grid[5].type = HQ;
    map.grid[5].owner = 1;

    map.grid[88].type = FACTORY;
    map.grid[88].owner = 2;

    map.grid[52].type = CRYSTAL;
    map.grid[102].type = TREE;

    map.grid[108].type = TOWER;
    map.grid[108].owner = 1;

    map.grid[122].type = BLOCKED;
    map.grid[121].type = BLOCKED;
    map.grid[120].type = BLOCKED;
    return map;
}

static inline
void draw_map(RenderBatch* batch, Map* map, MapScene* scene) {
    for(int y = 0; y < map->height; ++y) {
        for(int x = 0; x < map->width; ++x) {
            Tile current = map->grid[x + y * map->width];
            if(current.owner == 0)
                draw_rectangle(batch, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET), TILE_SIZE, TILE_SIZE, 50, 50, 50, 255);
            if(current.owner == 1)
                draw_rectangle(batch, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET), TILE_SIZE, TILE_SIZE, 50, 150, 50, 255);
            if(current.owner == 2)
                draw_rectangle(batch, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET), TILE_SIZE, TILE_SIZE, 150, 50, 50, 255);

            if(current.type == BLOCKED)
                draw_rectangle(batch, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET), TILE_SIZE, TILE_SIZE, 0, 0, 0, 255);

            if(current.type == HQ)
                draw_texture(batch, scene->HQ, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));
            if(current.type == FACTORY)
                draw_texture(batch, scene->factory, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));
            if(current.type == CRYSTAL)
                draw_texture(batch, scene->crystal, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));
            if(current.type == TREE)
                draw_texture(batch, scene->tree, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));
            if(current.type == TOWER)
                draw_texture(batch, scene->tower, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));

            if(x - 1 >= 0 && map->grid[(x-1) + y * map->width].type == HQ)
                draw_texture(batch, scene->offload, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));
            if(x - 1 >= 0 && map->grid[(x-1) + y * map->width].type == FACTORY)
                draw_texture(batch, scene->offload, x*(TILE_SIZE+OFFSET), y*(TILE_SIZE+OFFSET));
        }
    }

    for(int i = 0; i < map->units.size(); ++i) {
        Unit current = map->units[i];
        if(current.type == MELEE)
            draw_texture(batch, scene->melee, current.x*(TILE_SIZE+OFFSET), current.y*(TILE_SIZE+OFFSET));
        if(current.type == RANGED)
            draw_texture(batch, scene->ranged, current.x*(TILE_SIZE+OFFSET), current.y*(TILE_SIZE+OFFSET));
        if(current.type == MILITIA)
            draw_texture(batch, scene->militia, current.x*(TILE_SIZE+OFFSET), current.y*(TILE_SIZE+OFFSET));
    }
}

#endif
