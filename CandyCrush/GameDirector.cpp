//
//  Game.cpp
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "GameDirector.hpp"
#include "Gem.h"

using namespace sf;

void GameDirector::run() {
    RenderWindow app(VideoMode(744, 1080), "Match-3 Game!", Style::None);
    app.setFramerateLimit(60);
    
    Texture t1,t2;
    t1.loadFromFile("/Users/ben/Desktop/Match-3/Resources/Background.png");
    t2.loadFromFile("/Users/ben/Desktop/Match-3/Resources/gems.png");
    Sprite background(t1), gems(t2);
    
    BOARD_LOOP {
        _board[i][j].kind = std::rand() % 3;
        _board[i][j].col = j;
        _board[i][j].row = i;
        _board[i][j].x = j * ts;
        _board[i][j].y = i * ts;
    }
    
    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();
            
            if (e.type == Event::MouseButtonPressed)
                if (static_cast<int>(e.key.code) == static_cast<int>(Mouse::Left)) {
                    const Vector2i mousePos = Mouse::getPosition(app);
                    if (!_isSwap && !_isMoving && isInInterval(mousePos)) ++_click;
                    _pos = mousePos - offset;
                }
        }
        
        // mouse click
        if (_click == 1) {
            _x0 = _pos.x / ts + 1;
            _y0 = _pos.y / ts + 1;
        }
        
        if (_click == 2) {
            _x = _pos.x / ts + 1;
            _y = _pos.y / ts + 1;
            if (abs(_x - _x0) + abs(_y - _y0) == 1) {
                swapTiles(_board, _board[_y0][_x0], _board[_y][_x]); _isSwap = true; _click = 0;
            }
            else _click = 1;
        }
        
        //Match finding
        BOARD_LOOP {
            if (_board[i][j].kind == _board[i + 1][j].kind)
                if (_board[i][j].kind == _board[i - 1][j].kind)
                    for(int n = -1; n <= 1; ++n) _board[i + n][j].match++;
            
            if (_board[i][j].kind == _board[i][j + 1].kind)
                if (_board[i][j].kind == _board[i][j - 1].kind)
                    for(int n = -1; n <= 1; ++n) _board[i][j + n].match++;
        }
        
        //Moving animation
        _runAnimation();
        
        //Deleting amimation
        if (!_isMoving)
            BOARD_LOOP
            if (_board[i][j].match && _board[i][j].alpha >= 10) {
                _board[i][j].alpha -= 10; _isMoving = true;
            }
        
        //Get score
        int score = 0;
        BOARD_LOOP
        score += _board[i][j].match;
        
        //Second swap if no match
        if (_isSwap && !_isMoving) {
            if (!score) swapTiles(_board, _board[_y0][_x0], _board[_y][_x]); _isSwap = 0;
        }
        
        //Update grid
        if (!_isMoving) {
            for(int i = 7; i > 0; --i)
                for(int j = 1; j <= 6; ++j)
                    if (_board[i][j].match)
                        for(int n = i; n > 0; --n)
                            if (!_board[n][j].match) {
                                swapTiles(_board, _board[n][j], _board[i][j]);
                                break;
                            }
            
            for(int j = 1; j <= 6; ++j)
                for(int i = 7, n = 0; i > 0; --i)
                    if (_board[i][j].match) {
                        _board[i][j].kind = std::rand() % 5;
                        _board[i][j].y = -ts * n++;
                        _board[i][j].match = 0;
                        _board[i][j].alpha = 255;
                    }
        }
        
        //////draw///////
        app.draw(background);
        
        BOARD_LOOP {
            const Gem &p = _board[i][j];
            gems.setTextureRect(IntRect(p.kind * 80, 0, 80, 80));
            gems.setColor(Color(255, 255, 255, p.alpha));
            gems.setPosition(p.x, p.y);
            gems.move(offset.x - ts, offset.y - ts);
            app.draw(gems);
        }
        
        app.display();
    }
}

void GameDirector::_runAnimation() {
    _isMoving = false;
    BOARD_LOOP {
        Gem &p = _board[i][j];
        int dx{}, dy{};
        for(int n = 0; n < _speed; ++n) {  // 10 - speed
            dx = p.x - p.col * ts;
            dy = p.y - p.row * ts;
            if (dx) p.x -= dx / std::abs(dx);
            else if (dy) p.y -= dy / std::abs(dy);
            else break;
        }
        if (dx || dy) _isMoving = true;
    }
}

GameDirector& GameDirector::setAnimationSpeed(int speed) {
    _speed = speed;
    return *this;
}

GameDirector GameDirector::instance() {
    static GameDirector _game;
    return _game;
}
