//
//  Gem.h
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Gem_h
#define Gem_h

#include <SFML/Graphics.hpp>
#include <array>

enum class GemType : int {
    None = -1,
    PurpuleGem = 1000,
    RedGem,
    BlueGem,
    GreenGem,
    OrangeGem
};

enum class BombType : int {
    None = -1,
    Normal = 1000,
    Vertical,
    Horizontal
};

struct Gem {
    int x, y, col, row, match, alpha;
    
    GemType kind;
    BombType bomb;
    
    Gem()
    : x{}
    , y{}
    , col{}
    , row{}
    , match{}
    , kind{GemType::None}
    , bomb{BombType::None}
    , alpha{255} {}
    
};

using Board = std::array<std::array<Gem, 8>, 8>;

#define BOARD_LOOP \
for (int i = 1; i <= 7; ++i) \
for (int j = 1; j <= 6; ++j) \

#endif /* Gem_h */
