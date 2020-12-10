#pragma once
#include <tuple>
#include <string>

class player;

class village
{

public:
	village();
	~village();

	void setID(size_t ID);
	size_t getID() const;

	void setCoord(std::tuple<size_t, size_t> Coords);
	std::tuple<size_t, size_t> getCoord() const;

	void setPoints(size_t count);
	size_t getPoints() const;

	void setPlayerID(player* ID);
	player* getPlayerID() const;

private:
	size_t id = 0;
	size_t x = 0, y = 0;
	size_t points = 0;
	player* playerID = nullptr;

};

void readVillages(std::string url, std::deque<village>& villages, std::deque<player>& players, size_t& zoom);