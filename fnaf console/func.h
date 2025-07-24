#pragma once
#include <iostream>
#include <cstdlib>      
#include <ctime>        
#include <thread>       
#include <chrono>
#include <iomanip>
//#define DEBUG

#define MOVE_CURSOR(row, col) "\033[" << row << ";" << col << "H"
#define CLEAR_LINE "\033[K"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

extern std::atomic<bool> gameRunning;

class Animatronic;
class Player;

class Map 
{
	int timeNow = 0;

protected:
	int night = 1;

public:
	int GetNightStatus() const {
		return night;
	}

	class Door {
	private:
		bool isClosed;
	public:
		Door() {
			isClosed = false;
		}

		void CloseDoor() {
			isClosed = true;
		}

		void OpenDoor() {
			isClosed = false;
				
		}

		bool GetDoorStatus() const {
			return isClosed;
		}
	};
	
	Door doorLeft;
	Door doorRight;
	

	void SetNight(int value) {
		switch (value)
		{
		case 1:
			night = 1;
			break;
		case 2:
			night = 2;
			break;
		case 3:
			night = 3;
			break;
		case 4:
			night = 4;
			break;
		case 5:
			night = 5;
			break;
		default:
			break;
		}
	}

	void SetTime(int time) {
		this->timeNow = time;
	}

	int GetTime() {
		return timeNow;
	}

	void RenderMap() const {
		using std::cout;
		using std::endl;
		cout << endl;
		cout << "\t  10-\\    0\n";
		cout << "\t     |    |\n";
		cout << "\t   8-/----1----\\---9\n";
		cout << "\t     |         |\n";
		cout << "\t     5         2\n";
		cout << "\t     |         |\n";
		cout << "\t   6-|         |-3\n";
		cout << "\t     |-7-[M]-4-|\n";

	}

	class Camera {
	
	public:
		int cameraPositions;
		void RenderCamera() {

		}

		static void CheckPosition(int& inputPosition, Animatronic* anim1, Animatronic* anim2, Animatronic* anim3, Animatronic* anim4);
	};

};


class Player {
private:
	 bool PlayerAlive;

	class Energy {

		double energyAvaible = 100;
	public:
		void operator-= (double value) {
			energyAvaible -= value;
		}

		bool operator>(int value) const {
			if (energyAvaible > value) {
				return true;
			}
			else {
				return false;
			}
		}

		bool operator<=(int value) const {
			if (energyAvaible <= value) {
				return true; 
			}
			else {
				return false;
			}
		}

		double GetEnergy() const {
			return energyAvaible;
		}
	};

	enum doorsSight
	{
		RIGHT = 3,
		LEFT = 2
	};

	
public:
	Energy PlayerEnergy;
	
	void SetPlayerLive(bool value) {
		PlayerAlive = value;
	}

	bool isAlive() const {

		if (PlayerAlive == true) {
			return true;

		}
		else {
			return false;
		}
	}


	Player() {
		 PlayerAlive = true;
	}

	void attemptCloseDoor(int door, Map& map)  {
		if (door == doorsSight::LEFT) {
			if (map.doorLeft.GetDoorStatus() == true) {
				map.doorLeft.OpenDoor();
			
				if (map.doorLeft.GetDoorStatus() == false) {
					std::cout << "LEFT DOOR IS OPEN NOW\n";
				}
				return;
			}

			if (PlayerEnergy.GetEnergy() > 0 && (door == doorsSight::LEFT)) {
				map.doorLeft.CloseDoor();
				if (map.doorLeft.GetDoorStatus() == true) {
					std::cout << "LEFT DOOR IS CLOSED\n";
				}
				return;

			}
			else {
				std::cout << "Bip boop(run out of energy)" << std::endl;
				return;
			}

		}

		if (door == doorsSight::RIGHT) {
			if (map.doorRight.GetDoorStatus() == true) {
				map.doorRight.OpenDoor();

				if (map.doorRight.GetDoorStatus() == false)
				{
					std::cout << "RIGHT DOOR IS OPENED\n";
				}
				return;
			}

			else if (PlayerEnergy.GetEnergy() > 0 && (door == doorsSight::RIGHT)) {
				map.doorRight.CloseDoor();
				if (map.doorRight.GetDoorStatus() == true) {
					std::cout << "RIGHT DOOR IS CLOSED\n";
				}
				return;
			}
			else {
				std::cout << "Bip boop(run out of energy)" << std::endl;
				return;
			}
		}
	}

	void attemptToLookCamera(int &value, Animatronic* anim1, Animatronic* anim2, Animatronic* anim3, Animatronic* anim4) {
		Map::Camera::CheckPosition(value, anim1, anim2, anim3, anim4);
	}

};

class Animatronic {
protected:
	std::string jumpscare;
	std::string name;
	int rollDice;
	Player* playerA = nullptr;
	Map* mapA = nullptr;
public:
	
	std::string GetName() {
		return name;
	}
 int positionAnim;
	int ai_lvl;

	Animatronic(std::string name, std::string jumpscare, Player* playerP, Map* mapP) {
		this->name = name;
		this->jumpscare = jumpscare;
		playerA = playerP;
		this->mapA = mapP;
	}

	int getPositionAnim() const {
		return positionAnim;
	}

	
	virtual void Move() {}
	virtual void isActive(Map& playerMap) {}
	virtual void DoJumpscare() {}
	virtual bool RollToMove() {
		return false;
	}

	~Animatronic() {
	
	}
};

class Freddy : public Animatronic {

public:
	Freddy(std::string name, std::string jumpscare, Player* playerP, Map* mapP) : Animatronic(name, jumpscare, playerP, mapP) {
		positionAnim = 0;

	}

    void DoJumpscare() override {  
        std::cout << jumpscare << "   " << jumpscare << "   " << jumpscare << "\n";  
        std::cout << "(type smth to end loop)\n";  
        gameRunning = false;  
        if (playerA) {  
            playerA->SetPlayerLive(false);  
        }  
    }

	bool RollToMove() override {
		srand(static_cast<int>(time(0)));
		rollDice = rand() % 20 + 1;

		if (rollDice <= ai_lvl) {
			return true;
		}
		else {
			return false;
		}
	}

	void Move() override {

		if (RollToMove()) {
			if (positionAnim < 4) {
				positionAnim++;
#ifdef DEBUG
	std::cout << name << " is moving(postion anim -> " << positionAnim << ")\n";
#endif // DEBUG
			}
			else if (positionAnim == 4) {
				if ((mapA->doorRight.GetDoorStatus()) == true) {
					positionAnim -= 2;
				}
				else if ((mapA->doorRight.GetDoorStatus()) == false) {
					DoJumpscare();
					playerA->SetPlayerLive(false);
				}
			}
		}
		else {
#ifdef DEBUG
			std::cout << name << " in same spot (postion anim -> " << positionAnim << ")\n";
#endif
			return;
		}
	}

	void isActive(Map& playerMap) override {
		switch (playerMap.GetNightStatus()) {
		case 1: ai_lvl = 0; break;
		case 2: ai_lvl = 1; break;
		case 3: ai_lvl = 5; break;
		case 4: ai_lvl = 10; break;
		case 5: ai_lvl = 14; break;
		case 6: ai_lvl = 20; break;
		default: ai_lvl = 0; break;
		}
	}
	
};

class Chika : public Animatronic{
public:
	Chika(std::string name, std::string jumpscare, Player* playerP, Map* mapP) : Animatronic(name, jumpscare, playerP, mapP) {
		positionAnim = 0;
	}


	void DoJumpscare() override {
		std::cout << jumpscare << " " << jumpscare << " " << jumpscare << std::endl;
		std::cout << "(type smth to end loop)\n";
		gameRunning = false;
		if (playerA) {
			playerA->SetPlayerLive(false);
		}
	}

	bool RollToMove() override {
		srand(static_cast<int>(time(0)));
		rollDice = rand() % 20 + 1;

		if (rollDice <= ai_lvl) {
			return true;
		}
		else {
			return false;
		}
	}

	int RollToMoveN_Ch() {
		rollDice = rand() % 20 + 1;

		return rollDice;
	}

	void Move() override {

		if (RollToMove()) {
			if (positionAnim < 4 || positionAnim == 9) {
				//move logic
				MoveLogicChika(positionAnim);
#ifdef DEBUG
				std::cout << name << " is moving(postion anim -> " << positionAnim << ")\n";
#endif // DEBUG			
			}
			else if (positionAnim == 4) {
				if ((mapA->doorRight.GetDoorStatus()) == true) {
					positionAnim -= 2;
				}
				else if ((mapA->doorRight.GetDoorStatus()) == false) {
					DoJumpscare();
					playerA->SetPlayerLive(false);
				}
			}
		}
		else {
#ifdef DEBUG
			std::cout << name <<" in same spot (postion anim -> " << positionAnim << ")\n";
#endif
			return;
		}
	}

	void MoveLogicChika(int &positionAnim) {
		int tempLogic = 0;

		if (positionAnim == 9) {
			tempLogic = RollToMoveN_Ch();
			if (tempLogic > 14) {
				positionAnim = 1;
			}
			else {
#ifdef DEBUG
				std::cout << name << " in same spot (postion anim -> " << positionAnim << ")\n";
#endif
				return;
			}
		}

		if (positionAnim == 1 || positionAnim == 3) {
			tempLogic = RollToMoveN_Ch();
			if (tempLogic > 7) {
				positionAnim++;
			}
			else {
				positionAnim = 9;
				
			}
		}

		if (positionAnim == 0 || positionAnim == 2) {
			positionAnim++;
		}
	}

	void isActive(Map& playerMap) override {
		switch (playerMap.GetNightStatus()) {
		case 1: ai_lvl = 1; break;
		case 2: ai_lvl = 7; break;
		case 3: ai_lvl = 12; break;
		case 4: ai_lvl = 19; break;
		case 5: ai_lvl = 20; break;
		case 6: ai_lvl = 21; break;
		default: ai_lvl = 3; break;
		}
	}

};

class Bonny : public Animatronic {
public:

	Bonny(std::string name, std::string jumpscare, Player* playerP, Map* mapP) : Animatronic(name, jumpscare, playerP, mapP) {
		positionAnim = 0;
	}

	void DoJumpscare() override {
		std::cout << jumpscare << " " << jumpscare << " " << jumpscare << std::endl;
		std::cout << "(type smth to end loop)\n";
		gameRunning = false;
		if (playerA) {
			playerA->SetPlayerLive(false);
		}
	}

	void isActive(Map& playerMap) override {
		switch (playerMap.GetNightStatus()) {
		case 1: ai_lvl = 6; break;
		case 2: ai_lvl = 3; break;
		case 3: ai_lvl = 11; break;
		case 4: ai_lvl = 16; break;
		case 5: ai_lvl = 20; break;
		case 6: ai_lvl = 21; break;
		default: ai_lvl = 3; break;
		}
	}

	bool RollToMove() override {
		srand(static_cast<int>(time(0)));
		rollDice = rand() % 20 + 1;

		if (rollDice <= ai_lvl) {
			return true;
		}
		else {
			return false;
		}
	}

	int RollToMoveBunny() {
		rollDice = rand() % 20 + 1;

		return rollDice;
	}

	void Move() override {

		if (RollToMove()) {
			if (positionAnim < 7 || positionAnim == 10) {
				MoveLogicBunny(positionAnim);
#ifdef DEBUG
				std::cout << name << " is moving(postion anim -> " << positionAnim << ")\n";
#endif // DEBUG
			}
			else if (positionAnim == 7) {
				if ((mapA->doorLeft.GetDoorStatus()) == true) {
					positionAnim -= 2;
				}
				else if ((mapA->doorLeft.GetDoorStatus()) == false) {
					DoJumpscare();
					playerA->SetPlayerLive(false);
				}
			}
		}
		else {
#ifdef DEBUG
			std::cout << name << " in same spot (postion anim -> " << positionAnim << ")\n";
#endif
			return;
		}
	}


	void MoveLogicBunny(int& positionAnim) {
		if (positionAnim == 10) {
			if (RollToMoveBunny() > 12) {
				positionAnim = 1;
				return;
			}
			else {
				return;
			}
		}

//================== FOR 0 AND 1 POSITION ===================
		if (positionAnim == 0) {
			if (RollToMoveBunny() > 7) {
				++positionAnim;
				return;
			}
			else {
				positionAnim = 10;
				return;
			}			
		}

		if (positionAnim == 1) {
			if (RollToMoveBunny() > 7) {
				positionAnim = 5;
				return;
			}
			else {
				positionAnim = 10;
				return;
			}
		}
//================== END OF THIS LOGIC ===================
		if (positionAnim >= 5 && positionAnim < 7) {
			if (RollToMove()) {
				++positionAnim;
			}
		}		
	}
};

class Foxy : public Animatronic {
	int FoxyStageStatus;

public:
	Foxy(std::string name, std::string jumpscare, Player* playerP, Map* mapP) : Animatronic(name, jumpscare, playerP, mapP) {
		positionAnim = 8;
		FoxyStageStatus = 0;
	}

	void DoJumpscare() override {
		std::cout << jumpscare << " " << jumpscare << " " << jumpscare << std::endl;
		std::cout << "(type smth to end loop)\n";
		gameRunning = false;
		if (playerA) {
			playerA->SetPlayerLive(false);
		}
	}

	void isActive(Map& playerMap) override {
		switch (playerMap.GetNightStatus()) {
		case 1: ai_lvl = 0; break;
		case 2: ai_lvl = 0; break;
		case 3: ai_lvl = 5; break;
		case 4: ai_lvl = 10; break;
		case 5: ai_lvl = 13; break;
		case 6: ai_lvl = 21; break;
		default: ai_lvl = 3; break;
		}
	}

	bool RollToMove() override {
		srand(static_cast<int>(time(0)));
		rollDice = rand() % 20 + 1;

		if (rollDice <= ai_lvl) {
			return true;
		}
		else {
			return false;
		}
	}

	void Move() override {

		if (FoxyStageStatus == 3) {
#ifdef DEBUG
			std::cout << name << " is runnig towards to you\n";
#endif // DEBUG
			std::this_thread::sleep_for(std::chrono::seconds(2));
			positionAnim = 5;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			positionAnim = 7;
			if ((mapA->doorLeft.GetDoorStatus()) == true) {
				positionAnim = 8;
				FoxyStageStatus = 0;
#ifdef DEBUG
				std::cout << name << "bonk! Now he's sleeping\n";
#endif // DEBUG
				std::this_thread::sleep_for(std::chrono::seconds(5));
			}
			else if ((mapA->doorLeft.GetDoorStatus()) == false) {
				DoJumpscare();
				playerA->SetPlayerLive(false);
			}
		}
		else if (RollToMove()) {
			++FoxyStageStatus;
#ifdef DEBUG
			std::cout << name << " on status stage = " << FoxyStageStatus << "\n";
#endif // DEBUG
		} else {
#ifdef DEBUG
			std::cout << name << " is sleeping (status = " << FoxyStageStatus << ")\n";
#endif
			return;
		}
	}
};