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

using namespace sf;

//const int tileSize = 87;
//const Vector2i offset(115, 290);

struct Gem {
    int x, y, col, row, kind, match, alpha;
    
    Gem()
    : x{}
    , y{}
    , col{}
    , row{}
    , kind{}
    , match{}
    , alpha{255} {}
    
};

using Board = std::array<std::array<Gem, 8>, 8>;

#define BOARD_LOOP \
for (int i = 1; i <= 7; ++i) \
for (int j = 1; j <= 6; ++j) \

#endif /* Gem_h */
