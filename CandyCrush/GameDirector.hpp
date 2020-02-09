//
//  Game.hpp
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "Gem.h"

class GameDirector final {
    GameDirector() = default;
    GameDirector(const GameDirector&) = default;
    GameDirector(GameDirector&&) = default;
    
    void _runAnimation();

public:
    static GameDirector instance();
    void run();
    GameDirector& setAnimationSpeed(int speed);
private:
    Board _board;
    int _animationSpeed;
    bool _isSwap = false, _isMoving = false;
    Vector2i _pos;
    int _x0{}, _y0{}, _x{}, _y{}, _click{}, _speed{10};
};

#endif /* Game_hpp */
