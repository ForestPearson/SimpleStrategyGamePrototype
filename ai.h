#ifndef AI_H
#define AI_H

#include "snake.h"
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#define GROUP_SIZE 30

enum Direction;
struct Map;

struct AIEntity {
    Direction bias;
    int greediness;
    int seed;
    int mutability;
    float score;
};
bool move_snake(Map* map, AIEntity* e, Direction dir);

#define GenePool std::vector<AIEntity>

bool survival_of_the_fittest(AIEntity a, AIEntity b) {
    return a.score > b.score;
}

void natural_selection(GenePool* pool) {
    //Sort by how high of a score they got
    std::sort(pool->begin(), pool->end(), survival_of_the_fittest);
    int size = pool->size();

    //Eliminate the worst half
    pool->erase(pool->begin() + pool->size()/2, pool->end());

    printf("\n==Leaderboards==\n");
    for(int i = 0; i < pool->size(); ++i) {
        AIEntity e = pool->at(i);
        printf("Entity %d: (Score: %f, Seed: %d, Mutability: %d, Greediness: %d)\n", i, e.score, e.seed, e.mutability, e.greediness);
    }
    printf("\n");

    //Repopulate the gene pool by mixing traits from remaining species and mutating randomly
    for(int i = 0; i < size/2; ++i) {
        int index1;
        int index2;
        do {
            index1 = random_int(0, size/2);
            index2 = random_int(0, size/2);
        } while(index1 != index2);

        //index1 and index2 will now breed

        AIEntity one = pool->at(index1);
        AIEntity two = pool->at(index2);

        AIEntity child;
        child.mutability = (one.mutability + two.mutability)/2;
        child.greediness = (one.greediness + two.greediness)/2 + random_int(-child.mutability, child.mutability);
        child.seed = (one.seed + two.seed)/2 + random_int(-child.mutability, child.mutability);
        child.bias = (Direction)( (((int)one.bias + (int)two.bias)/2 + random_int(-child.mutability, child.mutability)) % 4 );

        pool->push_back(child);
    }
    for(int i = 0; i < pool->size(); ++i)
        pool->at(i).score = 0;
}

void randomize_gene_pool(GenePool* pool) {
    for(int i = 0; i < GROUP_SIZE; ++i) {
        AIEntity e;
        e.bias = (Direction)(random_int(0, 3));
        e.seed = rand();
        e.mutability = random_int(5, 20);
        e.greediness = random_int(-10, 10);
        e.score = 0;

        pool->push_back(e);
    }
}

bool ai_move_snake(Map* map, AIEntity* e) {
    int rnd = rand() % 3;
    bool done = false;
    if(rnd == 0)
        done = move_snake(map, e, (Direction)0);
    if(rnd == 1)
        done = move_snake(map, e, (Direction)1);
    if(rnd == 2)
        done = move_snake(map, e, (Direction)2);
    if(rnd == 3)
        done = move_snake(map, e, (Direction)3);

    return done;
}

#endif
