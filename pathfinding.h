//
// Created by russell on 17/04/2021.
//

#ifndef PATHFINDING_H
#define PATHFINDING_H
#include "vector"
#include "maps.h"
#include <algorithm>

struct node{
    bool IsVisited = false;
    double GlobalGoal = 1000;
    float LocalGoal = 1000;
    std::vector<node*> vecNeighbours = {};
    node* Parent;
    int x;
    int y;
};


std::vector<node*> Generate_nodes_map(const map& current_map){
    node dummy_1_node;
    std::vector<node*> node_map = {&dummy_1_node};


    for (int i = 0; i < current_map.map_width * current_map.map_height; i++){
        node* a_node = new node;
        node_map[i] = a_node;
        a_node->x = i % current_map.map_width;
        a_node->y = std::floor(i / current_map.map_width);
        node_map.push_back(a_node);
    }
    node_map.erase(node_map.begin());

    for (int i = current_map.map_width; i < node_map.size() - current_map.map_width; i++) {

        node dummy_node;
        node_map[i]->vecNeighbours.push_back(&dummy_node);

        if (!current_map.map[i + 1].hard) {
            node_map[i]->vecNeighbours.push_back(node_map[i + 1]);
        }
        if (!current_map.map[i - 1].hard) {
            node_map[i]->vecNeighbours.push_back(node_map[i - 1]);
        }
        if (!current_map.map[i - current_map.map_width].hard) {
            node_map[i]->vecNeighbours.push_back(node_map[i - current_map.map_width]);
        }
        if (!current_map.map[i + current_map.map_width].hard) {
            node_map[i]->vecNeighbours.push_back(node_map[i + current_map.map_width]);
        }
        node_map[i]->vecNeighbours.erase(node_map[i]->vecNeighbours.begin());
    }
    return node_map;
}

bool comparePtrToNode(node* a, node* b) { return (a->GlobalGoal < b->GlobalGoal); }

std::vector<std::vector<int>> A_star(int x1, int y1, int x2, int y2, std::vector<node*> node_map,const map& current_map){

    std::cout << "A star begin" << std::endl;
    int len = node_map.size();
    for (int i = 0; i < len; i++){
        node_map[i]->GlobalGoal = 1000;
        node_map[i]->LocalGoal = 1000;
    }

    node* current_node = node_map[(current_map.map_width) * y1 + x1];
    std::cout << current_node->x << " " << current_node->y << std::endl;

    current_node->Parent = current_node;
    std::vector<node*> nodes_to_test = {current_node};

    while (true) {
        for (int i = 0; i < (int) (current_node->vecNeighbours.size()); i++) {
            if (!current_node->vecNeighbours[i]->IsVisited) {
                if (x2 != current_node->vecNeighbours[i]->x or y2 != current_node->vecNeighbours[i]->y) {

                    if (current_node->vecNeighbours[i]->LocalGoal == 1000) {
                        nodes_to_test.push_back(current_node->vecNeighbours[i]);
                    }

                    if (current_node->LocalGoal + 1 < current_node->vecNeighbours[i]->LocalGoal){
                        current_node->vecNeighbours[i]->LocalGoal = current_node->LocalGoal + 1;
                        current_node->vecNeighbours[i]->Parent = current_node;
                        current_node->vecNeighbours[i]->GlobalGoal =
                                current_node->vecNeighbours[i]->LocalGoal + sqrt(
                                        pow((y2 - current_node->vecNeighbours[i]->y), 2) +
                                        pow((x2 - current_node->vecNeighbours[i]->x), 2));
                    }
                }
            }
        }
        current_node->IsVisited = true;
        std::sort(nodes_to_test.begin(), nodes_to_test.end(), comparePtrToNode);
        while (nodes_to_test[0]->IsVisited && !nodes_to_test.empty()) {
            nodes_to_test.erase(nodes_to_test.begin());
        }

        if (nodes_to_test.empty()) {
            break;
        } else {
            current_node = nodes_to_test[0];
        }
    }



    std::vector<std::vector<int>> path;
    current_node = node_map[(current_map.map_width - 1) * y2 + x2];
    std::cout << current_node->x << " " << current_node->y << std::endl;
    while (true){
        path.push_back({current_node->x, current_node->y});
        if (current_node == current_node->Parent or current_node->Parent == nullptr){
            break;
        }else{
            current_node = current_node->Parent;
        }
    }

    return path;
}

#endif // PATHFINDING_H

