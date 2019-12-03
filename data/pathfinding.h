#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "map.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>

#define NodePtr std::shared_ptr<Node>
#define NodeList std::vector<NodePtr>

struct Node {
    vec2 pos;
    NodePtr parent;
    float cost;
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
void process_successor(vec2 pos, float cost, NodePtr parent, NodeList& open, NodeList& closed) {
    for(int i = 0; i < open.size(); ++i)
        if(open[i].pos == pos)
            return;
    for(int i = 0; i < closed.size(); ++i)
        if(closed[i].pos == pos)
            return;

    open.push_back(NodePtr(create_node(pos, parent, cost)));
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

#endif
