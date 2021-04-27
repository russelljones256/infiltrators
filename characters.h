//
// Created by russell on 16/04/2021.
//

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "maps.h"
#include "pathfinding.h"

struct actions{
    std::vector<int> walk;
    bool shout = false;
    bool pass = false;
    bool scry = false;
};

struct character{
    bool on_route = false;
    std::string name;
    int loc_x = 10;
    int loc_y = 10;
    bool isFriend = true;
    std::unique_ptr<olc::Decal> decal;
    std::vector<std::vector<int>> path_list;
    int step = 0;
    float speed = 1;
    int facing = 1;
    float owntime = 0;
    std::vector<actions*> action;
    float sight_range = 30;
    bool seen = true;
    int act_remaining = 0;
    int ghost_loc_x;
    int ghost_loc_y;
    int dest_x;
    int dest_y;
    int pat1_x = 0;
    int pat1_y = 0;
    int pat2_x = 10;
    int pat2_y = 0;
    int dir = 1;


};

struct entities{
    std::vector<character*> all_char_list;
    std::vector<character*> all_allies_list;
    std::vector<character*> all_enemies_list;

};

bool isWalkable(int x , int y, const map& current_map, const entities& people){
    if (x < 0 or y < 0 or x > current_map.map_width - 1 or y > current_map.map_height - 1){return false;}
    if (get_map_site(x, y, current_map).hard){return false;}
    for (auto i : people.all_char_list){
        if (i->loc_x == x and i->loc_y == y){
            return false;
        }
    }
    return true;
}
bool can_A_see_B(character* a, character* b, map main_map){
    bool visible = true;
    double r_dist = sqrt(pow((a->loc_x - b->loc_x),2) + pow((a->loc_y - b->loc_y), 2));
    int x, y;
    float theta =  atan2(a->loc_y - b->loc_y, a->loc_x - b->loc_x);
    if (r_dist > a->sight_range) {
        visible = false;
        return visible;
    }
    for (int i = 0; i <= r_dist; i++){
        x = (int) round(i * cos((theta)) + b->loc_x);
        y = (int) round(i * sin((theta)) + b->loc_y);
        if (get_map_site(x, y, main_map).opaque) {
            visible = false;
            return visible;
        }
    }
    return visible;
}


bool can_A_hear_B(character* a, character* b, map main_map){
    bool audible = true;
    double r_dist = sqrt(pow((a->loc_x - b->loc_x),2) + pow((a->loc_y - b->loc_y), 2));
    int x, y;
    float theta =  atan2(a->loc_y - b->loc_y, a->loc_x - b->loc_x);
    if (r_dist >  4 * a->sight_range) {
        audible = false;
        return audible;
    }
    for (int i = 0; i <= r_dist; i++){
        x = (int) round(i * cos((theta)) + b->loc_x);
        y = (int) round(i * sin((theta)) + b->loc_y);
        if (get_map_site(x, y, main_map).opaque) {
            audible = false;
            return audible;
        }
    }
    return audible;
}


void set_walking(character* person, std::vector<std::vector<int>> path){
    for (int i = 0; i < person->action.size(); i++){
        delete person->action[i];
    }
    person->action = {};

    for (auto &i : path){
        actions* act = new actions;
        act->walk = i;
        person->action.push_back(act);
    }
}

void patrol(int x1, int y1, int x2, int y2, character* person, std::vector<node*> node_map, map& current_map){
    if ((person->loc_x == person->pat1_x and person->loc_y == person->pat1_y) or (person->loc_x == person->pat2_x and person->pat2_y)){
        if (person->on_route) {
            person->dir = person->dir * -1;
            std::cout << "test1" << std::endl;
        }
        if (person->action.size() == 0){person->on_route = true;}
    }

    if (!person->on_route){
        if (person->action.size() == 0){
            std::cout << "test2" << std::endl;
            set_walking(person, PATHFINDING_H::A_star(person->loc_x, person->loc_y, person->pat1_x, person->pat1_y, node_map, current_map));
        }
    }else if (person->dir == 1){
        std::cout << "test3" << std::endl;

        set_walking(person, PATHFINDING_H::A_star(person->loc_x, person->loc_y, person->pat2_x, person->pat2_y, node_map, current_map));
    }else if (person->dir == -1){
        std::cout << "test4" << std::endl;

        set_walking(person, PATHFINDING_H::A_star(person->loc_x, person->loc_y, person->pat1_x, person->pat1_y, node_map, current_map));
    }
}
void do_actions(character* person, map& current_map, entities& people, std::vector<node*> node_map) {
    if (person->owntime > person->speed and !person->action.empty()) {
        person->owntime = 0;

        if (person->action[0]->pass){
            delete person->action[0];
            person->action.erase(person->action.begin());
            return;
        }

        if (person->action[0]->shout){
            for (auto &i : people.all_enemies_list){
                if (can_A_hear_B(i, person, current_map)){
                    std::vector<std::vector<int>> path = {};
                    i->dest_x = person->loc_x;
                    i->dest_y = person->loc_y;
                    path = PATHFINDING_H::A_star(i->loc_x, i->loc_y, i->dest_x, i->dest_y, node_map, current_map);
                    set_walking(i, path);
                    i->on_route = false;
                    break;
                }
            }
            delete person->action[0];
            person->action.erase(person->action.begin());
        }

        if (person->action[0]->scry){
            for (auto &i : people.all_enemies_list){i->seen = true;};
            delete person->action[0];
            person->action.erase(person->action.begin());
        }
        if (!person->action[0]->walk.empty()) {
            if (person->loc_x > person->action[0]->walk[0] and person->loc_y == person->action[0]->walk[1]) {
                person->facing = 3;
                if (isWalkable(person->action[0]->walk[0], person->action[0]->walk[1], current_map, people)) {
                    person->loc_x = person->action[0]->walk[0];
                    person->step++;
                    delete person->action[0];
                    person->action.erase(person->action.begin());
                    if (person->step == 2) {
                        person->step = 0;
                    }
                    return;
                }
            }

            if (person->loc_x < person->action[0]->walk[0] and person->loc_y == person->action[0]->walk[1]) {
                person->facing = 1;
                if (isWalkable(person->action[0]->walk[0], person->action[0]->walk[1], current_map, people)) {
                    person->loc_x = person->action[0]->walk[0];
                    person->step++;
                    delete person->action[0];

                    person->action.erase(person->action.begin());

                    if (person->step == 2) {
                        person->step = 0;
                    }
                    return;
                }
            }

            if (person->loc_x == person->action[0]->walk[0] and person->loc_y > person->action[0]->walk[1]) {
                person->facing = 0;
                if (isWalkable(person->action[0]->walk[0], person->action[0]->walk[1], current_map, people)) {
                    person->loc_y = person->action[0]->walk[1];
                    person->step++;
                    delete person->action[0];

                    person->action.erase(person->action.begin());

                    if (person->step == 2) {
                        person->step = 0;
                    }
                    return;
                }
            }

            if (person->loc_x == person->action[0]->walk[0] and person->loc_y < person->action[0]->walk[1]) {
                person->facing = 2;
                if (isWalkable(person->action[0]->walk[0], person->action[0]->walk[1], current_map, people)) {
                    person->loc_y = person->action[0]->walk[1];
                    person->step++;
                    delete person->action[0];

                    person->action.erase(person->action.begin());
                    if (person->step == 2) {
                        person->step = 0;
                    }
                    return;
                }
            }
            if (person->action.size() == 0){person->on_route = true;}
        }
    }
}
#endif //CHARACTERS_H
