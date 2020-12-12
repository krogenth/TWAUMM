#include "cURLread.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

tribe::tribe() {



}

tribe::~tribe() {



}

void tribe::setID(size_t ID) {

	this->id = ID;

}
size_t tribe::getID() const {

	return this->id;

}

void tribe::setName(std::wstring Name) {

	this->name = Name;

}
std::wstring tribe::getName() const {

	return this->name;

}

void tribe::setTag(std::wstring Tag) {

	this->tag = Tag;

}
std::wstring tribe::getTag() const {

	return this->tag;

}

void tribe::setMemberCount(size_t count) {

	this->members = count;

}
size_t tribe::getMemberCount() const {

	return this->members;

}

void tribe::setVillageCount(size_t count) {

	this->villages = count;

}
size_t tribe::getVillageCount() const {

	return this->villages;

}

void tribe::setTopPoints(size_t count) {

	this->topPoints = count;

}
size_t tribe::getTopPoints() const {

	return this->topPoints;

}

void tribe::setPoints(size_t count) {

	this->points = count;

}
size_t tribe::getPoints() const {

	return this->points;

}

void tribe::setRank(size_t rank) {

	this->rank = rank;

}
size_t tribe::getRank() const {

	return this->rank;

}

void tribe::addMemberID(player* memberID) {

	this->memberIDs.emplace_back(memberID);

}
player* tribe::getMemberID(size_t index) const {

	if (index < memberIDs.size())
		return this->memberIDs[index];
	else
		throw std::exception();

}

void tribe::setOD(size_t OD) {

	this->od = OD;

}
size_t tribe::getOD() const {

	return this->od;

}

void tribe::setODA(size_t ODA) {

	this->oda = ODA;

}
size_t tribe::getODA() const {

	return this->oda;

}

void tribe::setODD(size_t ODD) {

	this->odd = ODD;

}
size_t tribe::getODD() const {

	return this->odd;

}

void tribe::setConq(village* village) {

	this->conqPoints += village->getPoints();
	this->conqs.emplace_back(village);

}
village* tribe::getConq(size_t index) const {

	if (index < conqs.size())
		return this->conqs[index];
	else
		throw std::exception();

}
size_t tribe::getConqPoints() const {

	return this->conqPoints;

}
size_t tribe::getConqCount() const {

	return this->conqs.size();

}

void tribe::setLoss(village* village) {

	this->lossPoints += village->getPoints();
	this->losses.emplace_back(village);

}
village* tribe::getLoss(size_t index) const {

	if (index < losses.size())
		return this->losses[index];
	else
		throw std::exception();

}
size_t tribe::getLossPoints() const {

	return this->lossPoints;

}
size_t tribe::getLossCount() const {

	return this->losses.size();

}