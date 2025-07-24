#include <iostream>
#include "func.h"
#include <cstdlib>      
#include <ctime>        
#include <thread>       
#include <chrono>
#include <mutex>

void animatronicLoop(Animatronic* anim1, Animatronic* anim2, Animatronic* anim3, Animatronic* anim4, int intervalSeconds, Player& player);
void checkTimeInMap(std::chrono::steady_clock::time_point startTime, int intervalSeconds, Player& player, Map& map);
void EnergyLoop(Player& player, Map& map, int intervalSeconds, Animatronic* anim);
void RenderLoop(Player& player, Map& map);
std::atomic<bool> gameRunning(true);
std::mutex coutMutex;

int main(int argc, char* argv) {

    Map map;
    Player player;
 
    auto start = std::chrono::high_resolution_clock::now();

    Animatronic* freddy = new Freddy("Freddy", "Freddy's jumpscare", &player, &map);
    Animatronic* chika = new Chika("Chika", "Chika's jumpscare", &player, &map);
    Animatronic* bonny = new Bonny("Bonny", "Bonny's jumpscare", &player, &map);
    Animatronic* foxy = new Foxy("Foxy", "Foxy's jumpscare", &player, &map);

    map.SetNight(5);
    freddy->isActive(map);
    chika->isActive(map);
    bonny->isActive(map);
    foxy->isActive(map);

    std::thread animThread(animatronicLoop, freddy, chika, bonny, foxy, 4, std::ref(player));
    std::thread energyThread(EnergyLoop, std::ref(player), std::ref(map), 1, freddy);
    std::thread renderThread(RenderLoop, std::ref(player), std::ref(map));

    //написать таймер через chorono где то в мапе, мб в функцию там передаватт
    /*auto start = std::chrono::high_resolution_clock::now(); // Старт

    // ... подождали (например, игра работала 5 секунд) ...

    auto now = std::chrono::high_resolution_clock::now();   // Сейчас

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);

    std::cout << "Прошло секунд: " << elapsed.count();

    done
    */

    std::thread timeThread(checkTimeInMap, start, 1, std::ref(player), std::ref(map));

    while (player.isAlive()) {
       

       // пофиксить закрытие дверей и их логику - done

        int choice;
       
        std::cin >> choice;
        for (int i = 17; i <= 20; ++i) {
            std::cout << "\033[" << i << ";1H" << "\033[K";
        }

        if (!(player.isAlive())) {
            
            animThread.detach();
            timeThread.detach();
            break;
        }

        switch (choice) {
        case 1:
            int cam;
            std::cout << "What camera number: ";
            std::cin >> cam;
            std::cout << '\n';
            player.attemptToLookCamera(cam, freddy, chika, bonny, foxy);
            break;
        case 2:
            player.attemptCloseDoor(2, map);
            break;
        case 3:
            player.attemptCloseDoor(3, map);
            break;
        default:
            std::cout << "Unknown action.\n";
            std::cin.clear();
            std::cin.ignore();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    }

    

    gameRunning = false;
    if (animThread.joinable()) animThread.join();
    if (timeThread.joinable()) timeThread.join();
    if (energyThread.joinable()) energyThread.join();
    if (renderThread.joinable()) renderThread.join();



    delete freddy;
    delete chika;
    delete bonny;
    delete foxy;
    std::cout << "Game Over.\n";
    std::cin.ignore();
    std::cin.get();

    return 0;
}



void animatronicLoop(Animatronic* anim1, Animatronic* anim2, Animatronic* anim3, Animatronic* anim4,  int intervalSeconds, Player& player) {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
        if (player.isAlive()) {
            anim1->Move();
        }

        if (player.isAlive()) {
            anim2->Move();
        }
        
        if (player.isAlive()) {
            anim3->Move();
        }

        if (player.isAlive()) {
            anim4->Move();
        }
    }
}

void checkTimeInMap(std::chrono::steady_clock::time_point startTime, int intervalSeconds, Player& player, Map& map) {
    while (gameRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
        std::chrono::steady_clock::time_point nowTime = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(nowTime - startTime);
        map.SetTime(diff.count());
        if (diff.count() >= 100) {
            
            for (int i = 0; i < 3; i++) {

                std::lock_guard<std::mutex> lock(coutMutex);
               
                std::cout << ">>> 6 AM <<<\n";
            }
            std::cout << "type smth\n";
            if (player.isAlive()) {
                player.SetPlayerLive(false);
            }
            gameRunning = false;
        }
    }
}

void EnergyLoop(Player& player, Map& map, int intervalSeconds, Animatronic* anim) {
    while (gameRunning)
    {
        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
        if (player.PlayerEnergy > 0) {

            if (map.doorLeft.GetDoorStatus() == true && map.doorRight.GetDoorStatus() == true) {
                player.PlayerEnergy -= 1.5; //1.2
            }
            else if (map.doorLeft.GetDoorStatus() == true || map.doorRight.GetDoorStatus() == true) {
                player.PlayerEnergy -= 1.2; // 0.8
            }
            else {
                player.PlayerEnergy -= 0.4; // 0.4
            }
        }

        if (player.PlayerEnergy <= 0) {
            {
                std::lock_guard<std::mutex> lock(coutMutex);
            }
            std::cout << "RUN OUT OF ENERGY\n\n";
            for (int i = 0; i < 3; i++) {
                std::cout << "MUSIC PLAYING\n";
            }
            std::cout << std::endl;
            anim->DoJumpscare();
            player.SetPlayerLive(false);
            gameRunning = false;
        }
    }
}

void RenderLoop(Player& player, Map& map) {
    while (gameRunning) {
        {
            std::lock_guard<std::mutex> lock(coutMutex);

       

            // time
            std::cout << "\033[1;1H" << CLEAR_LINE;
            std::cout << "Time: " << map.GetTime() << " sec\n";

            // energy
            std::cout << CLEAR_LINE;
            std::cout << "Energy: " << static_cast<int>(player.PlayerEnergy.GetEnergy()) << "\n";

            // map
            std::cout << CLEAR_LINE;
            map.RenderMap();

            // Меню
            std::cout << CLEAR_LINE << "[1]Cameras   [2]Left door   [3]Right Door\n";

            // erase 13 - 15,
            for (int i = 13; i <= 15; ++i) {
                std::cout << "\033[" << i << ";1H" << "\033[K";
            }

           

            // end 
            std::cout << CLEAR_LINE;
            if (player.PlayerEnergy <= 0) {
                std::cout << ">>> OUT OF POWER <<<\n";
            }
            else if (map.GetTime() >= 100) {
                std::cout << ">>> 6 AM — You survived the night! <<<\n";
            }
            else if (!player.isAlive()) {
                std::cout << ">>> GAME OVER <<<\n";
                std::cout << "Press Enter to exit...\n";
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << SHOW_CURSOR;
}