// Created by russell on 15/04/2021.
//
//void draw_map(const map& current_map){
//    for (int i = 0; i <= current_map.map_width - 1; i++){
//        for (int j = 0; j <= current_map.map_height - 1; j++){
//           std::unique_ptr<olc::Sprite> sprTile = std::make_unique<olc::Sprite>(MAPS_H::get_map_site(i, j, current_map).sprite_path);
//            DrawSprite(olc::vi2d(i,j) * 32, sprTile.get());
//        }
//    }
//}
// COPY THIS INTO PGE PRIVATE
#ifndef MAPS_H
#define MAPS_H
#include <vector>
#include "olcPixelGameEngine.h"

struct tile{
    std::string name{};
    bool hard{false};
    bool opaque{false};
    std::string sprite_path {};
};

struct map{
    std::vector<tile> map = {{}};
    int map_width{};
    int map_height{};
};

map map_process(const std::string& path){

    map current_map;
    std::string map_line_data;
    std::ifstream map_file(path);

    int count = 0;
    while (getline(map_file, map_line_data))
    {
        count ++;
        int line_len = map_line_data.size();

        current_map.map_width = line_len;
        for (int i = 0; i < line_len ; i++){
            if (map_line_data[i] == 'w'){
                tile wall{"wall", true, true, "assets/sprites/environ/wall.png"};
                current_map.map.push_back(wall);
            }else if (map_line_data[i] == 'r'){
                tile front{"front", true, false, "assets/sprites/environ/wall_2.png"};
                current_map.map.push_back(front);
            }else if (map_line_data[i] == 'u'){
                tile crate{"crate", true, true, "assets/sprites/environ/crate.png"};
                current_map.map.push_back(crate);
            }else if (map_line_data[i] == 'f'){
                tile floor{"floor", false, false, "assets/sprites/environ/floor.png"};
                current_map.map.push_back(floor);

            }else if (map_line_data[i] == 'g'){
                tile grass{"grass", false, false, "assets/sprites/environ/grass.png"};
                current_map.map.push_back(grass);

            }
        }
    }
    current_map.map.erase(current_map.map.begin());
    current_map.map_height = count;

    return current_map;
}

tile get_map_site(int x, int y, map current_map){
    return current_map.map[(current_map.map_width) * y + x];
}


#endif //MAPS_H