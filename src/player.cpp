#include "cURLread.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

player::player() {



}
player::~player() {



}

void player::setID(size_t ID) {

	this->id = ID;

}
size_t player::getID() const {

	return this->id;

}

void player::setName(std::wstring Name) {

	this->name = Name;

}
std::wstring player::getName() const {

	return this->name;

}

void player::setTribeID(tribe* tribe) {

	this->tribeID = tribe;

}
tribe* player::getTribeID() const {

	return this->tribeID;

}

void player::setPoints(size_t count) {

	this->points = count;

}
size_t player::getPoints() const {

	return this->points;

}

void player::setRank(size_t rank) {

	this->rank = rank;

}
size_t player::getRank() const {

	return this->rank;

}

size_t player::getVillageCount() {

	return this->villages.size();

}

void player::addVillage(village* village) {

	this->villages.emplace_back(village);

}
village* player::getVillage(size_t index) const {

	if (index < this->villages.size())
		return this->villages[index];
	else
		throw std::exception();

}

void player::setOD(size_t OD) {

	this->od = OD;

}
size_t player::getOD() const {

	return this->od;

}

void player::setODA(size_t ODA) {

	this->oda = ODA;

}
size_t player::getODA() const {

	return this->oda;

}

void player::setODD(size_t ODD) {

	this->odd = ODD;

}
size_t player::getODD() const {

	return this->odd;

}

void player::setConq(village* village) {

	this->conqPoints += village->getPoints();
	this->conqs.emplace_back(village);

}
village* player::getConq(size_t index) const {

	if (index < this->conqs.size())
		return this->conqs[index];
	else
		throw std::exception();

}
size_t player::getConqPoints() const {

	return this->conqPoints;

}
size_t player::getConqCount() const {

	return this->conqs.size();

}

void player::setLoss(village* village) {

	this->lossPoints += village->getPoints();
	this->losses.emplace_back(village);

}
village* player::getLoss(size_t index) const {

	if (index < this->losses.size())
		return this->losses[index];
	else
		throw std::exception();

}
size_t player::getLossPoints() const {

	return this->lossPoints;

}
size_t player::getLossCount() const {

	return this->losses.size();

}