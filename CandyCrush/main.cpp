#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Gem.h"


void swapTiles(Board& grid, Gem p1, Gem p2) noexcept {
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);
    
    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}

int main() {
    Board board;
    RenderWindow app(VideoMode(744, 1080), "Match-3 Game!", Style::None);
    app.setFramerateLimit(60);
    
    Texture t1,t2;
    t1.loadFromFile("/Users/ben/Desktop/CandyCrush/Resources/Background.png");
    t2.loadFromFile("/Users/ben/Desktop/CandyCrush/Resources/gems.png");
    Sprite background(t1), gems(t2);
    
    BOARD_LOOP {
        board[i][j].kind = std::rand() % 3;
        board[i][j].col = j;
        board[i][j].row = i;
        board[i][j].x = j * ts;
        board[i][j].y = i * ts;
    }
    
    int x0{}, y0{}, x{}, y{};
    int click = 0;
    Vector2i pos;
    bool isSwap = false, isMoving = false;
    
    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();
            
            if (e.type == Event::MouseButtonPressed)
                if (static_cast<int>(e.key.code) == static_cast<int>(Mouse::Left)) {
                    const Vector2i mousePos = Mouse::getPosition(app);
                    if (!isSwap && !isMoving && isInInterval(mousePos)) ++click;
                    pos = mousePos - offset;
                }
        }
        
        // mouse click
        if (click == 1) {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        
        if (click == 2) {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1) {
                swapTiles(board, board[y0][x0], board[y][x]); isSwap = true; click = 0;
            }
            else click = 1;
        }
        
        //Match finding
        BOARD_LOOP {
            if (board[i][j].kind == board[i + 1][j].kind)
                if (board[i][j].kind == board[i - 1][j].kind)
                    for(int n = -1; n <= 1; ++n) board[i + n][j].match++;
            
            if (board[i][j].kind == board[i][j + 1].kind)
                if (board[i][j].kind == board[i][j - 1].kind)
                    for(int n = -1; n <= 1; ++n) board[i][j + n].match++;
        }
        
        //Moving animation
        isMoving = false;
        BOARD_LOOP {
            Gem &p = board[i][j];
            int dx{}, dy{};
            for(int n = 0; n < 10; ++n) {  // 10 - speed
                dx = p.x - p.col * ts;
                dy = p.y - p.row * ts;
                if (dx) p.x -= dx / std::abs(dx);
                else if (dy) p.y -= dy / std::abs(dy);
                else break;
            }
            if (dx || dy) isMoving = true;
        }
        
        //Deleting amimation
        if (!isMoving)
            BOARD_LOOP
                if (board[i][j].match && board[i][j].alpha >= 10) {
                    board[i][j].alpha -= 10; isMoving = true;
                }
        
        //Get score
        int score = 0;
        BOARD_LOOP
            score += board[i][j].match;
        
        //Second swap if no match
        if (isSwap && !isMoving) {
            if (!score) swapTiles(board, board[y0][x0], board[y][x]); isSwap = 0;
        }
        
        //Update grid
        if (!isMoving) {
            for(int i = 7; i > 0; --i)
                for(int j = 1; j <= 6; ++j)
                    if (board[i][j].match)
                        for(int n = i; n > 0; --n)
                            if (!board[n][j].match) {
                                swapTiles(board, board[n][j], board[i][j]);
                                break;
                            }
            
            for(int j = 1; j <= 6; ++j)
                for(int i = 7, n = 0; i > 0; --i)
                    if (board[i][j].match) {
                        board[i][j].kind = std::rand() % 5;
                        board[i][j].y = -ts * n++;
                        board[i][j].match = 0;
                        board[i][j].alpha = 255;
                    }
        }
        
        //////draw///////
        app.draw(background);
        
        BOARD_LOOP {
            const Gem &p = board[i][j];
            gems.setTextureRect(IntRect(p.kind * 80, 0, 80, 80));
            gems.setColor(Color(255, 255, 255, p.alpha));
            gems.setPosition(p.x, p.y);
            gems.move(offset.x - ts, offset.y - ts);
            app.draw(gems);
        }
        
        app.display();
    }
    return 0;
}
