//
// Created by russell on 16/04/2021.
//

#ifndef CHARACTERS_H
#define CHARACTERS_H

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
    std::vector<std::vector<int>> path;

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

void walk_path(character* person, map& current_map, entities& people) {
    if (person->owntime > person->speed and !person->path.empty()) {
        person->owntime = 0;

        if (person->loc_x > person->path[0][0] and person->loc_y == person->path[0][1]) {
            person->facing = 3;
            if (isWalkable(person->path[0][0], person->path[0][1], current_map, people)) {
                person->loc_x = person->path[0][0];
                person->step++;
                person->path.erase(person->path.begin());
                if (person->step == 2) {
                    person->step = 0;
                }
                return;
            }
        }

        if (person->loc_x < person->path[0][0] and person->loc_y == person->path[0][1]) {
            person->facing = 1;
            if (isWalkable(person->path[0][0], person->path[0][1], current_map, people)) {
                person->loc_x = person->path[0][0];
                person->step++;
                person->path.erase(person->path.begin());

                if (person->step == 2) {
                    person->step = 0;
                }
                return;
            }
        }

        if (person->loc_x == person->path[0][0] and person->loc_y > person->path[0][1]) {
            person->facing = 0;
            if (isWalkable(person->path[0][0], person->path[0][1], current_map, people)) {
                person->loc_y = person->path[0][1];
                person->step++;
                person->path.erase(person->path.begin());

                if (person->step == 2) {
                    person->step = 0;
                }
                return;
            }
        }
        if (person->loc_x == person->path[0][0] and person->loc_y < person->path[0][1]) {
            person->facing = 2;
            if (isWalkable(person->path[0][0], person->path[0][1], current_map, people)) {
                person->loc_y = person->path[0][1];
                person->step++;
                person->path.erase(person->path.begin());

                if (person->step == 2) {
                    person->step = 0;
                }
                return;
            }
        }
    }
}



#endif //CHARACTERS_H
