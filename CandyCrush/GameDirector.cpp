//
//  Game.cpp
//  CandyCrush
//
//  Created by Ben on 2/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "GameDirector.hpp"
#include <tuple>

using namespace sf;

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

GameDirector& GameDirector::setAnimationSpeed(int speed) noexcept {
    _speed = speed;
    return *this;
}

GameDirector& GameDirector::instance() noexcept {
    static GameDirector game;
    return game;
}

// MARK: Private methods implementations

void GameDirector::_runAnimations() noexcept {
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

void GameDirector::_matchFinder() noexcept {
    
    if (!_bombFinder(_x, _y)) _bombFinder(_x0, _y0);
    
    BOARD_LOOP {
          if (_board[i][j].kind == _board[i + 1][j].kind &&
              _board[i][j].kind == _board[i - 1][j].kind &&
              _board[i][j].bomb == BombType::None)
                  for(int n = -1; n <= 1; ++n) _board[i + n][j].match++;
          
          if (_board[i][j].kind == _board[i][j + 1].kind &&
              _board[i][j].kind == _board[i][j - 1].kind &&
              _board[i][j].bomb == BombType::None)
                  for(int n = -1; n <= 1; ++n) _board[i][j + n].match++;
        
          if (_board[i][j].kind == _board[i][j + 1].kind &&
              _board[i][j].kind == _board[i + 1][j].kind &&
              _board[i][j].kind == _board[i + 1][j + 1].kind) {
              
              _board[i][j + 1].match++;
              _board[i + 1][j].match++;
              _board[i + 1][j + 1].match++;
          }

      }
}

void GameDirector::_updateBoard() noexcept {
    if (!_isMoving) {
           for (int i = 7; i > 0; --i)
               for (int j = 1; j <= 6; ++j)
                   if (_board[i][j].match)
                       for (int n = i - 1; n > 0; --n)
                           if (!_board[n][j].match) {
                               _swapTiles(_board[n][j], _board[i][j]);
                               break;
                           }
               
       for (int j = 1; j <= 6; ++j)
           for (int i = 7, n = 0; i > 0; --i)
               if (_board[i][j].match) {
                   _board[i][j].bomb = BombType::None;
                   _board[i][j].kind = static_cast<GemType>(std::rand() % 5 + 1000);
                   _board[i][j].y = -_tileSize * n++;
                   _board[i][j].match = 0;
                   _board[i][j].alpha = 255;
               }
       }
}

void GameDirector::_clickHandler() noexcept {
    if (_click == 1) {
        _x0 = _pos.x / _tileSize + 1;
        _y0 = _pos.y / _tileSize + 1;
        
        switch (_board[_y0][_x0].bomb) {
            case BombType::Normal:
                _board[_y0][_x0].match++;
                _board[_y0 - 1][_x0 - 1].match++;
                _board[_y0 - 1][_x0].match++;
                _board[_y0 - 1][_x0 + 1].match++;
                _board[_y0][_x0 - 1].match++;
                _board[_y0][_x0 + 1].match++;
                _board[_y0 + 1][_x0 - 1].match++;
                _board[_y0 + 1][_x0].match++;
                _board[_y0 + 1][_x0 + 1].match++;
                _board[_y0][_x0].bomb = BombType::None;
                _click = 0;

                break;
                
            case BombType::Vertical:
                for (int i = 1; i <= _board[_y0].size(); _board[i++][_x0].match++) {}
                _board[_y0][_x0].bomb = BombType::None;
                _click = 0;

                break;
                
            case BombType::Horizontal:
                for (int i = 1; i <= _board.size(); _board[_y0][i++].match++) {}
                _board[_y0][_x0].bomb = BombType::None;
                _click = 0;

                break;
                
            default:
                break;
        }
                
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

void GameDirector::_swapBack() noexcept {
    int score = 0;
    BOARD_LOOP
    score += _board[i][j].match;
    
    if (_isSwap && !_isMoving) {
        if (!score) _swapTiles(_board[_y0][_x0], _board[_y][_x]); _isSwap = false;
    }
}

void GameDirector::_deleteMatchedGems() noexcept {
    if (!_isMoving)
    BOARD_LOOP
    if (_board[i][j].match && _board[i][j].alpha >= 10) {
        _board[i][j].alpha -= 10; _isMoving = true;
    }
}

void GameDirector::_swapTiles(Gem p1, Gem p2) noexcept {
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);
    
    _board[p1.row][p1.col] = std::move(p1);
    _board[p2.row][p2.col] = std::move(p2);
}

bool GameDirector::_contains(const Vector2i &vec,
                             const std::pair<int, int> &lowerLeft,
                             const std::pair<int, int> &upperRight) noexcept {
    
    return (vec.x >= lowerLeft.first && vec.x <= upperRight.first) &&
           (vec.y >= upperRight.second && vec.y <= lowerLeft.second);
}

void GameDirector::_displayMainWindow(RenderWindow& app) {
    Texture t;
    t.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/mainWndow.png");
    Sprite mainWindow(t);
    bool breakLoop = false;
    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();
            
            if (e.type == Event::MouseButtonPressed)
                if (static_cast<int>(e.key.code) == static_cast<int>(Mouse::Left) && _contains(Mouse::getPosition(app), {190, 730}, {558, 640})) {
                    breakLoop = true;
                }
        }
        app.draw(mainWindow);
        app.display();
        if (breakLoop) break;
        
    }
    app.clear();
}

bool GameDirector::_bombFinder(int x, int y) noexcept {
    
    // MARK: Normal Bomb Logic
    if (_board[y][x].kind == _board[y][x - 1].kind &&
        _board[y][x].kind == _board[y - 1][x].kind &&
        _board[y][x].kind == _board[y - 1][x - 1].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Normal;
        _board[y][x - 1].match++;
        _board[y - 1][x].match++;
        _board[y - 1][x - 1].match++;
        return true;
        
    } else if (_board[y][x].kind == _board[y + 1][x].kind &&
               _board[y][x].kind == _board[y][x - 1].kind &&
               _board[y][x].kind == _board[y + 1][x - 1].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Normal;
        _board[y + 1][x].match++;
        _board[y][x - 1].match++;
        _board[y + 1][x - 1].match++;
        return true;
        
    } else if (_board[y][x].kind == _board[y][x + 1].kind &&
               _board[y][x].kind == _board[y + 1][x].kind &&
               _board[y][x].kind == _board[y + 1][x + 1].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Normal;
        _board[y][x + 1].match++;
        _board[y + 1][x].match++;
        _board[y + 1][x + 1].match++;
        return true;
        
    } else if (_board[y][x].kind == _board[y - 1][x].kind &&
               _board[y][x].kind == _board[y - 1][x + 1].kind &&
               _board[y][x].kind == _board[y][x + 1].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Normal;
        _board[y - 1][x].match++;
        _board[y - 1][x + 1].match++;
        _board[y][x + 1].match++;
        return true;
        
    // MARK: Vertical Bomb Logic
    } else if (_board[y][x].kind == _board[y - 1][x].kind &&
               _board[y][x].kind == _board[y + 1][x].kind &&
               _board[y][x].kind == _board[y + 2][x].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Vertical;
        _board[y - 1][x].match++;
        _board[y + 1][x].match++;
        _board[y + 2][x].match++;
        return true;
        
    } else if (_board[y][x].kind == _board[y - 2][x].kind &&
               _board[y][x].kind == _board[y - 1][x].kind &&
               _board[y][x].kind == _board[y + 1][x].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Vertical;
        _board[y - 2][x].match++;
        _board[y - 1][x].match++;
        _board[y + 1][x].match++;
        return true;
        
    // MARK: Horziontal Bomb Logic
    } else if (_board[y][x].kind == _board[y][x - 2].kind &&
               _board[y][x].kind == _board[y][x - 1].kind &&
               _board[y][x].kind == _board[y][x + 1].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Horizontal;
        _board[y][x - 2].match++;
        _board[y][x - 1].match++;
        _board[y][x + 1].match++;
        return true;
        
    } else if (_board[y][x].kind == _board[y][x - 1].kind &&
               _board[y][x].kind == _board[y][x + 1].kind &&
               _board[y][x].kind == _board[y][x + 2].kind) {
        
        _board[y][x].kind = GemType::None;
        _board[y][x].bomb = BombType::Horizontal;
        _board[y][x - 1].match++;
        _board[y][x + 1].match++;
        _board[y][x + 2].match++;
        return true;
        
    }
    
    return false;
    
}



