#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include <unordered_set>
#include <queue>
#include <memory>
#include <algorithm>

#define NodePtr std::shared_ptr<Node>
#define NodeList std::unordered_set<NodePtr>

struct Node {
    vec2 pos;
    float cost;
    NodePtr parent;
};

static inline
Node* create_node(vec2 pos, NodePtr parent, float cost) {
    Node* newnode = new Node;
    newnode->pos = pos;
    newnode->parent = parent;
    newnode->cost = cost;
    return newnode;
}

static inline
void traverse_path(NodePtr current, std::vector<vec2>& path) {
    if(current == NULL)
        return;
    traverse_path(current->parent, path);
    path.push_back(current->pos);
}

static inline
std::vector<vec2> reconstruct_path(NodePtr current) {
    std::vector<vec2> path;
    traverse_path(current, path);
    return path;
}

static inline
NodePtr get_lowest_element(NodeList& open) {
    float min = 99999999;
    NodePtr lowest;
    for(NodePtr n : open) {
        if(n->cost < min) {
            min = n->cost;
            lowest = n;
        }
    }

    return lowest;
}

static inline
void process_successor(float dx, float dy, vec2 end, int(*heuristic)(Map*, int, int), Map* map, NodePtr parent, NodeList& open, NodeList& closed) {
    vec2 pos = {parent->pos.x + dx, parent->pos.y + dy};
    if(pos.x < 0 || pos.y < 0) return;
    if(pos.x >= map->width || pos.y >= map->height) return;

    int h = heuristic(map, pos.x, pos.y);
    if(h == -1) return;
    float cost = parent->cost + getDistanceE(pos.x, pos.y, end.x, end.y) + h;

    for(NodePtr n : open) {
        if(n->pos.x == pos.x && n->pos.y == pos.y) {
            if(n->cost <= cost)
                return;
            break;
        }
    }
    for(NodePtr n : closed) {
        if(n->pos.x == pos.x && n->pos.y == pos.y) {
            if(n->cost <= cost)
                return;
            break;
        }
    }
    
    open.insert(NodePtr(create_node(pos, parent, cost)));
}

static inline
bool pathfind(Map* map, vec2 start, vec2 end, int(*heuristic)(Map*, int, int), std::vector<vec2>* path) {
    NodeList open;
    NodeList closed;
    open.insert(NodePtr(create_node(start, NULL, 0)));
    NodePtr current;

    while(!open.empty()) {
        current = get_lowest_element(open);
        closed.insert(current);

        if(current->pos.x == end.x && current->pos.y == end.y) {
            *path = reconstruct_path(current);
            return true;
        }

        //Nice O(1) operations in unordered_set
        open.erase( open.find(current) );

        //process all 4 tiles surrounding current one
        process_successor(-1,  0, end, heuristic, map, current, open, closed);
        process_successor( 1,  0, end, heuristic, map, current, open, closed);
        process_successor( 0, -1, end, heuristic, map, current, open, closed);
        process_successor( 0,  1, end, heuristic, map, current, open, closed);
    }

    return false;
}

struct FillNode {
    vec2 pos;
    int depth;
};

static inline
void add_fill_node(int dx, int dy, FillNode& n, Map* map, bool* visited, std::queue<FillNode>& Q, std::vector<vec2>& path, int range, int(*heuristic)(Map*, int, int)) {
    int nx = n.pos.x;
    int ny = n.pos.y;
    if(nx + dx < 0 || nx + dx > map->width-1) return;
    if(ny + dy < 0 || ny + dy > map->height-1) return;
    if(n.depth >= range) return;
    int h = heuristic(map, nx+dx, ny+dy);
    if(h == -1) return;

    if(!visited[(nx+dx) + (ny+dy) * map->width]) {
        visited[(nx+dx) + (ny+dy) * map->width] = true;
        Q.push( {{(f32)nx+dx, (f32)ny+dy}, n.depth+h} );
        path.push_back( {(f32)nx+dx, (f32)ny+dy} );
    }
}

static inline
std::vector<vec2> floodfill(Map* map, vec2 start, int range, int(*heuristic)(Map*, int, int)) {
    //set up visited array
    bool* visited = new bool[map->width * map->height];
    memset(visited, false, map->width * map->height);
    visited[(i32)(start.x + start.y * map->width)] = true;

    //set up Queue
    std::queue<FillNode> Q;
    Q.push( {start, 0} );

    //set up path
    std::vector<vec2> path;
    path.push_back(start);

    while(!Q.empty()) {
        FillNode n = Q.front();
        Q.pop();
        
        add_fill_node(-1,  0, n, map, visited, Q, path, range, heuristic);
        add_fill_node( 1,  0, n, map, visited, Q, path, range, heuristic);
        add_fill_node( 0, -1, n, map, visited, Q, path, range, heuristic);
        add_fill_node( 0,  1, n, map, visited, Q, path, range, heuristic);
    }

    return path;
}

#endif
