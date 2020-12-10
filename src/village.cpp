#include "cURLread.h"

#include "player.h"
#include "village.h"

village::village() {



}
village::~village() {



}

void village::setID(size_t ID) {

	this->id = ID;

}
size_t village::getID() const {

	return this->id;

}

void village::setCoord(std::tuple<size_t, size_t> Coords) {

	this->x = std::get<0>(Coords);
	this->y = std::get<1>(Coords);

}
std::tuple<size_t, size_t> village::getCoord() const {

	return { this->x, this->y };

}

void village::setPoints(size_t count) {

	this->points = count;

}
size_t village::getPoints() const {

	return this->points;

}

void village::setPlayerID(player* ID) {

	this->playerID = ID;

}
player* village::getPlayerID() const {

	return this->playerID;

}