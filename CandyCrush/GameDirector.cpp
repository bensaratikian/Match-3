//
//  Game.cpp
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "GameDirector.hpp"

GameDirector::GameDirector()
: _x0{}
, _y0{}
, _x{}
, _y{}
, _click{}
, _speed{7}
, _isSwap{}
, _isMoving{}
, _tileSize{87}
, _offset{115, 290} {}


// MARK: Public methods implementations

void GameDirector::run() {
    RenderWindow app(VideoMode(744, 1080), "My Game", Style::Close);
    app.setFramerateLimit(60);
    
    Texture t1, t2;
    t1.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/Background.png");
    t2.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/gems.png");
    Sprite background(t1), gems(t2);
    
    BOARD_LOOP {
        _board[i][j].kind = std::rand() % 3;
        _board[i][j].col = j;
        _board[i][j].row = i;
        _board[i][j].x = j * _tileSize;
        _board[i][j].y = i * _tileSize;
    }
    
    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();
            
            if (e.type == Event::MouseButtonPressed)
                if (static_cast<int>(e.key.code) == static_cast<int>(Mouse::Left)) {
                    const Vector2i mousePos = Mouse::getPosition(app);
                    if (!_isSwap && !_isMoving && _isInInterval(mousePos)) ++_click;
                    _pos = mousePos - _offset;
                }
        }
        
        // mouse click handler
        _clickHandler();
        
        //Finding matches
        _matchFinder();
        
        //Run animations
        _runAnimation();
        
        //Deleting amimation if matched
        _deleteMatchedGems();
                
        //Swap back if gems do not match
        _swapBack();
        
        //Update board
        _updateBoard();
        
        app.draw(background);
        
        BOARD_LOOP {
            const Gem &p = _board[i][j];
            gems.setTextureRect(IntRect(p.kind * 80, 0, 80, 80));
            gems.setColor(Color(255, 255, 255, p.alpha));
            gems.setPosition(p.x, p.y);
            gems.move(_offset.x - _tileSize, _offset.y - _tileSize);
            app.draw(gems);
        }
        
        app.display();
    }
}

GameDirector& GameDirector::setAnimationSpeed(int speed) {
    _speed = speed;
    return *this;
}

GameDirector& GameDirector::instance() {
    static GameDirector game;
    return game;
}

// MARK: Private methods implementations

void GameDirector::_runAnimation() {
    _isMoving = false;
    BOARD_LOOP {
        Gem &p = _board[i][j];
        int dx{}, dy{};
        for(int n = 0; n < _speed; ++n) {
            dx = p.x - p.col * _tileSize;
            dy = p.y - p.row * _tileSize;
            if (dx) p.x -= dx / std::abs(dx);
            else if (dy) p.y -= dy / std::abs(dy);
            else break;
        }
        if (dx || dy) _isMoving = true;
    }
}

void GameDirector::_matchFinder() {
    BOARD_LOOP {
          if (_board[i][j].kind == _board[i + 1][j].kind)
              if (_board[i][j].kind == _board[i - 1][j].kind)
                  for(int n = -1; n <= 1; ++n) _board[i + n][j].match++;
          
          if (_board[i][j].kind == _board[i][j + 1].kind)
              if (_board[i][j].kind == _board[i][j - 1].kind)
                  for(int n = -1; n <= 1; ++n) _board[i][j + n].match++;
      }
}

void GameDirector::_updateBoard() {
    if (!_isMoving) {
           for(int i = 7; i > 0; --i)
               for(int j = 1; j <= 6; ++j)
                   if (_board[i][j].match)
                       for(int n = i; n > 0; --n)
                           if (!_board[n][j].match) {
                               _swapTiles(_board[n][j], _board[i][j]);
                               break;
                           }
               
       for(int j = 1; j <= 6; ++j)
           for(int i = 7, n = 0; i > 0; --i)
               if (_board[i][j].match) {
                   _board[i][j].kind = std::rand() % 5;
                   _board[i][j].y = -_tileSize * n++;
                   _board[i][j].match = 0;
                   _board[i][j].alpha = 255;
               }
       }
}

void GameDirector::_clickHandler() {
    if (_click == 1) {
        _x0 = _pos.x / _tileSize + 1;
        _y0 = _pos.y / _tileSize + 1;
    }
    
    if (_click == 2) {
        _x = _pos.x / _tileSize + 1;
        _y = _pos.y / _tileSize + 1;
        if (abs(_x - _x0) + abs(_y - _y0) == 1) {
            _swapTiles(_board[_y0][_x0], _board[_y][_x]);
            _isSwap = true;
            _click = 0;
        }
        else _click = 1;
    }
}

void GameDirector::_swapBack() {
    int score = 0;
    BOARD_LOOP
    score += _board[i][j].match;
    
    if (_isSwap && !_isMoving) {
        if (!score) _swapTiles(_board[_y0][_x0], _board[_y][_x]); _isSwap = 0;
    }
}

void GameDirector::_deleteMatchedGems() {
    if (!_isMoving)
    BOARD_LOOP
    if (_board[i][j].match && _board[i][j].alpha >= 10) {
        _board[i][j].alpha -= 10; _isMoving = true;
    }
}

void GameDirector::_swapTiles(Gem p1, Gem p2) noexcept {
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);
    
    _board[p1.row][p1.col] = p1;
    _board[p2.row][p2.col] = p2;
}

bool GameDirector::_isInInterval(const Vector2i &vec) noexcept {
    return (vec.x >= 112 && vec.x <= 633) && (vec.y >= 286 && vec.y <= 894);
}

