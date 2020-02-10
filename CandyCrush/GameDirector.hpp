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
    GameDirector();
    GameDirector(const GameDirector&);
    GameDirector(GameDirector&&);
    
    void _runAnimation();
    void _matchFinder();
    void _updateBoard();
    void _clickHandler();
    void _swapBack();
    void _deleteMatchedGems();
    void _swapTiles(Gem p1, Gem p2) noexcept;
    bool _contains(const sf::Vector2i &vec, const std::pair<int, int> &lowerLeft, const std::pair<int, int> &upperRight) noexcept;
    void _displayMainWindow(sf::RenderWindow&);
    
public:
    static GameDirector& instance();
    GameDirector& setAnimationSpeed(int speed);
    void run();
    
private:
    Board _board;
    sf::Vector2i _pos;
    sf::Vector2i _offset;
    bool _isSwap, _isMoving;
    int _x0, _y0, _x, _y, _click, _speed, _tileSize;
};

#endif /* Game_hpp */
