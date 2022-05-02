#pragma once
#include <tuple>
#include <string>
#include <vector>

class Player;

class Village {
public:
	Village() {}
	~Village() {}

	void virtual setID(size_t id) { this->id = id; }
	size_t virtual getID() const { return this->id; }

	void setCoord(std::tuple<uint32_t, uint32_t> coords) { this->coords = coords; }
	std::tuple<uint32_t, uint32_t> getCoord() const { return this->coords; }

	void setPoints(size_t points) { this->points = points; }
	size_t getPoints() const { return this->points; }

	void setPlayer(Player* player);
	Player* getPlayer() const { return this->player; }

private:
	size_t id = 0;
	std::tuple<uint32_t, uint32_t> coords{ 0, 0 };
	size_t points = 0;
	Player* player = nullptr;
};