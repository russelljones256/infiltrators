//
// Created by russell on 16/04/2021.
//

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "maps.h"
struct actions{
    std::vector<int> walk;
    bool shout = false;
};

struct character{
    std::string name;
    int loc_x = 10;
    int loc_y = 10;
    bool isFriend = true;
    std::unique_ptr<olc::Decal> decal;
    std::vector<std::vector<int>> path_list;
    int step = 0;
    float speed = 2;
    int facing = 1;
    float owntime = 0;
    std::vector<actions*> action;
    float sight_range = 3;
    bool seen = true;

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

void do_actions(character* person, map& current_map, entities& people) {
    if (person->owntime > person->speed and !person->action.empty()) {
        person->owntime = 0;

        if (!person->action[0]->shout){
            std::cout<< "Over here!!!" << std::endl;
        }
        if (!person->action[0]->walk.empty()) {
            if (person->loc_x > person->action[0]->walk[0] and person->loc_y == person->action[0]->walk[1]) {
                person->facing = 3;
                if (isWalkable(person->action[0]->walk[0], person->action[0]->walk[1], current_map, people)) {
                    person->loc_x = person->action[0]->walk[0];
                    person->step++;
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
                    person->action.erase(person->action.begin());
                    if (person->step == 2) {
                        person->step = 0;
                    }
                    return;
                }
            }
        }
    }
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

void set_walking(character* person, std::vector<std::vector<int>> path){
    for (int i = 0; i < person->speed * 10; i++){
        actions* act = new actions;
        act->walk = path[i];
        person->action.push_back(act);
    }
}



#endif //CHARACTERS_H
