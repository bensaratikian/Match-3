#include <iostream>
#include "GameDirector.hpp"

int main() {
    GameDirector::instance().setAnimationSpeed(10).run();
    return 0;
}
