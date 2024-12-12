#ifndef GAME_H
#define GAME_H

#include <vector>

namespace game {
    namespace gamePlay {        
        class Tile {
            private:
            public:
            // Tile type
            // What should I write here?
            enum TileType {placeholder, home, buildable, prison, tax, random};

            TileType type;
            Tile(TileType _type = placeholder) : type(_type) {}
            ~Tile() {}
        };

        class Buildable : Tile {
            private:
            public:
            Buildable() : Tile(buildable) {}
            ~Buildable() {}
        };

        class Home : Tile {
            private:
            public:
            Home() : Tile(home) {}
            ~Home() {}
        };

        class Prison : Tile {
            private:
            public:
            Prison() : Tile(prison) {}
            ~Prison() {}
        };

        class Tax : Tile {
            private:
            public:
            Tax() : Tile(tax) {}
            ~Tax() {}
        };

        class Random : Tile {
            private:
            public:
            Random() : Tile(random) {};
            ~Random() {}
        };

        class Map {
            private:
            public:
            std::vector<Tile> Tiles;

            Map() {}
            ~Map() {}
        };
    }
    
    namespace player {
        class Player {
            private:
            public:
            long long cash;

            Player(long long initialCash = 0) : cash(initialCash) {}
            ~Player() {}
        };

        class ComputerPlayer : Player {
            private:
            public:

            ComputerPlayer(long long initialCash = 0) : Player(initialCash) {}
            ~ComputerPlayer() {}
        };
    }
}

#endif