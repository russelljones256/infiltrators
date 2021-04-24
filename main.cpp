#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "maps.h"
#include "characters.h"
#include "pathfinding.h"
#include <vector>
#include <cmath>
#include <cstdio>
#include <tuple>
#include <unistd.h>


class Infiltrators : public olc::PixelGameEngine {
public:
    Infiltrators() {
        sAppName = "Infiltrators";
    }

private:

    void draw_map(const map& map){
        for (int i = 0; i <= map.map_width - 1; i++){
            for (int j = 0; j <= map.map_height - 1; j++){
                std::unique_ptr<olc::Sprite> sprTile = std::make_unique<olc::Sprite>(MAPS_H::get_map_site(i, j, map).sprite_path);
                DrawPartialSprite(olc::vi2d(i,j) * 32, sprTile.get(), olc::vi2d(0,0), olc::vi2d(16,16), 2);
            }
        }
    }

    void draw_hud(const map& map){
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < ScreenHeight(); j++) {
		        std::unique_ptr <olc::Sprite> sprTile = std::make_unique<olc::Sprite>("assets/sprites/environ/meadow_32.png");
		        DrawPartialSprite(olc::vi2d ((ScreenWidth() - i * 32), j * 32), sprTile.get(),olc::vi2d(0,0), olc::vi2d(32,32));
            }
        }
        FillRect(0, 0, ScreenWidth(), 10, olc::DARK_GREEN);
        FillRect(0, ScreenHeight() - 10, ScreenWidth(), 10, olc::DARK_GREEN);
        FillRect(0, 0, 10, ScreenHeight(), olc::DARK_GREEN);
        FillRect(ScreenWidth() - 7 * 32, 0, 10, ScreenHeight(), olc::DARK_GREEN);
        FillRect(ScreenWidth() - 7 * 32, ScreenHeight() / 2, ScreenWidth() - 7 * 32, 10, olc::DARK_GREEN);
        FillRect(ScreenWidth() - 7 * 32, ScreenHeight() * 0.7, ScreenWidth() - 7 * 32, 10, olc::DARK_GREEN);
        FillRect(ScreenWidth() - 10, 0, 10, ScreenHeight(), olc::DARK_GREEN);
        DrawString(olc::vi2d(ScreenWidth() - 6 * 32, 0.53 * ScreenHeight()), "Squadmates", olc::DARK_CYAN,2);
        DrawString(olc::vi2d(ScreenWidth() - 6.5 * 32, 0.73 * ScreenHeight()), "Seen Enemies", olc::DARK_CYAN,2);

    }

    void update_hud(entities people, int active_character){
        for (int i = 0; i< people.all_allies_list.size(); i++){
            if (i == active_character){
                DrawPartialDecal(olc::vi2d(ScreenWidth() - 5 * 32, ScreenHeight() * 0.15),
                                 people.all_allies_list[i]->decal.get(), olc::vi2d(2 + 60 * people.all_allies_list[i]->step, 2 + 36 * 2), olc::vi2d(28, 36), {2.5f, 2.5f});

                DrawStringDecal(olc::vi2d(ScreenWidth() - 5.5 * 32, ScreenHeight() * 0.05), people.all_allies_list[i]->name,olc::DARK_CYAN, {3.0f, 3.0f});
            }else{
                DrawPartialDecal(olc::vi2d(ScreenWidth() - (6 - i) * 32 , ScreenHeight() * 0.57),
                                 people.all_allies_list[i]->decal.get(), olc::vi2d(2 + 60 * people.all_allies_list[i]->step, 2 + 36 * 2), olc::vi2d(28, 36));
            }
        }
        for (int i = 0; i< people.all_enemies_list.size(); i++)
        {

            DrawPartialDecal(olc::vi2d(ScreenWidth() - 32 * (2 + (4 - i) % 5), ScreenHeight() * (0.77 + (0.1 * std::floor(i / 4)))),
                             people.all_enemies_list[i]->decal.get(), olc::vi2d(2 + 60 * people.all_enemies_list[i]->step, 2 + 36 * 2), olc::vi2d(28, 36));

            }
        }

    int active_character = 0;
    std::string gamemode = "realtime";
    character ranger_f;
    character mage_m;
    character warrior_m;
    entities people;
    map current_map = MAPS_H::map_process("assets/maps/basic.map");





public:
    bool OnUserCreate() override {
        std::vector<node*> node_map = PATHFINDING_H::Generate_nodes_map(current_map);
        std::vector<std::vector<int>> path;
        path = PATHFINDING_H::A_star(4, 5, 0, 0, node_map, current_map);


        set_walking(&warrior_m, path);
        warrior_m.name = "warrior";
        warrior_m.loc_y = 5;
        warrior_m.loc_x = 4;
        ranger_f.name = "Ranger";
        mage_m.name = "Mage";
        ranger_f.decal =  std::make_unique<olc::Decal>(std::make_unique<olc::Sprite>("assets/sprites/characters/ranger_f.png").get());
        mage_m.decal =  std::make_unique<olc::Decal>(std::make_unique<olc::Sprite>("assets/sprites/characters/mage_m.png").get());
        warrior_m.decal =  std::make_unique<olc::Decal>(std::make_unique<olc::Sprite>("assets/sprites/characters/warrior_m.png").get());
        people.all_char_list.push_back(&ranger_f);
        people.all_allies_list.push_back(&ranger_f);
        people.all_char_list.push_back(&mage_m);
        people.all_allies_list.push_back(&mage_m);
        people.all_char_list.push_back(&warrior_m);
        people.all_enemies_list.push_back(&warrior_m);

        draw_map(current_map);
        draw_hud(current_map);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        update_hud(people, active_character);

        for (auto  & i : people.all_enemies_list){i->seen = false;}
        for (auto & i : people.all_char_list){
            i->owntime+=fElapsedTime;
            do_actions(i, current_map, people);
        }

        if (GetKey(olc::Key::LEFT).bPressed) {
            people.all_allies_list[active_character]->facing = 3;
            if (isWalkable(people.all_allies_list[active_character]->loc_x -1, people.all_allies_list[active_character]->loc_y, current_map, people)) {
                people.all_allies_list[active_character]->loc_x -= 1;
                people.all_allies_list[active_character]->step++;
            }
        }

        if (GetKey(olc::Key::RIGHT).bPressed) {
            people.all_allies_list[active_character]->facing = 1;
            if (isWalkable(people.all_allies_list[active_character]->loc_x + 1, people.all_allies_list[active_character]->loc_y, current_map, people)) {
                people.all_allies_list[active_character]->loc_x += 1;
                people.all_allies_list[active_character]->step++;
            }
        }

        if (GetKey(olc::Key::DOWN).bPressed) {
            people.all_allies_list[active_character]->facing = 2;
            if (isWalkable(people.all_allies_list[active_character]->loc_x, people.all_allies_list[active_character]->loc_y + 1, current_map, people)) {
                people.all_allies_list[active_character]->loc_y += 1;
                people.all_allies_list[active_character]->step++;
            }
        }

        if (GetKey(olc::Key::UP).bPressed) {
            people.all_allies_list[active_character]->facing = 0;
            if (isWalkable(people.all_allies_list[active_character]->loc_x, people.all_allies_list[active_character]->loc_y - 1, current_map, people)) {
                people.all_allies_list[active_character]->loc_y -= 1;
                people.all_allies_list[active_character]->step++;
            }
        }

        if (GetKey(olc::Key::TAB).bPressed){
            active_character++;
            if (active_character == people.all_allies_list.size()){
                active_character = 0;
            }
        }
        for (auto & i : people.all_enemies_list){
            for (auto & j : people.all_allies_list){
                if (can_A_see_B(j, i, current_map)){
                    i->seen = true;
                    break;
                }
            }
        }
        for (auto & i : people.all_char_list) {
            if (i->seen) {
                DrawPartialDecal(olc::vi2d(i->loc_x, i->loc_y) * 32,
                                 i->decal.get(), olc::vi2d(2 + 60 * i->step, 2 + 36 * i->facing), olc::vi2d(28, 36));
                if (people.all_char_list[active_character]->step == 2) {
                    people.all_char_list[active_character]->step = 0;
                }
            }
        }
        return true;
    }
};

int main() {
    Infiltrators graphics;
    if (graphics.Construct(32*28, 32*15, 4, 4))
        graphics.Start();

    return 0;
}
