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
    GameDirector(const GameDirector&);
    GameDirector(GameDirector&&);
    
    void _runAnimation();
    void _matchFinder();
    void _updateBoard();
    void _clickHandler();
    void _swapBack();
    void _deleteMatchedGems();
    
public:
    static GameDirector& instance();
    void run();
    GameDirector& setAnimationSpeed(int speed);
    
private:
    Board _board;
    Vector2i _pos;
    int _animationSpeed;
    bool _isSwap = false, _isMoving = false;
    int _x0{}, _y0{}, _x{}, _y{}, _click{}, _speed{7};
};

#endif /* Game_hpp */
