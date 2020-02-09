//
//  Game.hpp
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

class GameDirector final {
    GameDirector() = default;
    GameDirector(const GameDirector&) = default;
    GameDirector(GameDirector&&) = default;

public:
    static GameDirector instance();
    void run();
};

#endif /* Game_hpp */
