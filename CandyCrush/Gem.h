//
//  Gem.h
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Gem_h
#define Gem_h

using namespace sf;

const int ts = 87; //tile size
const Vector2i offset(115, 290);

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

constexpr bool isInInterval(const Vector2i &vec) noexcept {
    return (vec.x >= 112 && vec.x <= 633) && (vec.y >= 286 && vec.y <= 894);
}

#define BOARD_LOOP \
for (int i = 1; i <= 7; ++i) \
for (int j = 1; j <= 6; ++j) \

#endif /* Gem_h */
