#include "func.h"
#include <iostream>

void Map::Camera::CheckPosition(int& inputPosition, Animatronic* anim1, Animatronic* anim2, Animatronic* anim3, Animatronic* anim4) {
    if (inputPosition == anim1->getPositionAnim()) {
        std::cout << anim1->GetName() << " is here\n";
    }
    if (inputPosition == anim2->getPositionAnim()) {
        std::cout << anim2->GetName() << " is here\n";
    }  
    if (inputPosition == anim3->getPositionAnim()) {
        std::cout << anim3->GetName() << " is here\n";
    }
    if (inputPosition == anim4->getPositionAnim()) {
        std::cout << anim4->GetName() << " is here\n";
    }
}
