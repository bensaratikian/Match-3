//
//  Run.cpp
//  CandyCrush
//
//  Created by Ben Saratikyan on 2/14/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "GameDirector.hpp"

using namespace sf;

void GameDirector::run() {
    RenderWindow app(VideoMode(744, 1080), "My Game", Style::Close);
    app.setFramerateLimit(60);
    
    _displayMainWindow(app);
    
    Texture t1, t2, t3, t4, t5;
    t1.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/Background.png");
    t2.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/gems.png");
    t3.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/bomb.png");
    t4.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/v_bomb.png");
    t5.loadFromFile("/Users/bensaratikyan/Desktop/Match-3/Resources/h_bomb.png");
    Sprite background(t1), gems(t2), bomb(t3), verticalBomb(t4), horizontalBomb(t5);
    
    BOARD_LOOP {
        _board[i][j].kind = static_cast<GemType>(std::rand() % 5 + 1000);
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
                    if (!_isSwap && !_isMoving && _contains(mousePos, {112, 894}, {633, 286})) {
                        ++_click;
                        _pos = mousePos - _offset;
                    }
                }
        }
        
        // mouse click handler
        _clickHandler();
        
        //Finding matches
        _matchFinder();
        
        //Run animations
        _runAnimations();
        
        //Deleting animation if matched
        _deleteMatchedGems();
                
        //Swap back if gems do not match
        _swapBack();
        
        //Update board
        _updateBoard();
        
        app.draw(background);
        
        _loadLevels(app);

        BOARD_LOOP {
            Gem &p = _board[i][j];
            
            switch (p.bomb) {
                case BombType::None:
                      gems.setTextureRect(IntRect(static_cast<int>(p.kind) % 1000 * 80, 0, 80, 80));
                      gems.setColor(Color(255, 255, 255, p.alpha));
                      gems.setPosition(p.x, p.y);
                      gems.move(_offset.x - _tileSize, _offset.y - _tileSize);
                      app.draw(gems);
                    break;
                    
                case BombType::Normal:
                    bomb.setColor(Color(255, 255, 255, p.alpha));
                    bomb.setPosition(p.x, p.y);
                    bomb.move(_offset.x - _tileSize, _offset.y - _tileSize);
                    app.draw(bomb);
                    break;
                    
                case BombType::Vertical:
                    verticalBomb.setColor(Color(255, 255, 255, p.alpha));
                    verticalBomb.setPosition(p.x, p.y);
                    verticalBomb.move(_offset.x - _tileSize, _offset.y - _tileSize);
                    app.draw(verticalBomb);
                    break;
                    
                case BombType::Horizontal:
                    horizontalBomb.setColor(Color(255, 255, 255, p.alpha));
                    horizontalBomb.setPosition(p.x, p.y);
                    horizontalBomb.move(_offset.x - _tileSize, _offset.y - _tileSize);
                    app.draw(horizontalBomb);
                    break;
                    
                default:
                    break;
            }
            
            // MARK: Displaying level requirements on the screen
            if (!p.isCounted && p.match)
            switch (p.kind) {
                case GemType::PurpuleGem:
                    if (_levels[0]) {
                    _levels[0]--;
                    p.isCounted = true;
                    }
                    break;
                    
                case GemType::BlueGem:
                if (_levels[1]) {
                    _levels[1]--;
                    p.isCounted = true;
                    }
                    break;
                    
                case GemType::RedGem:
                    if (_levels[2]) {
                    _levels[2]--;
                    p.isCounted = true;
                    }
                    break;
                    
                default:
                    break;
            }
            
        }
        
        app.display();
    }
}
