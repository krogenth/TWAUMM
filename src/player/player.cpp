#include "./player.h"

#include "../village/village.h"
#include "../tribe/tribe.h"

void Player::setTribe(Tribe* tribe) {
	this->tribe = tribe;
	this->tribe->addMember(this);
}

void Player::setConq(const Village* village) {
	this->conqPoints += village->getPoints();
	this->conqs.emplace_back(village);
	if (this->tribe) {
		this->tribe->setConq(village);
	}
}

void Player::setLoss(const Village* village) {
	this->lossPoints += village->getPoints();
	this->losses.emplace_back(village);
	if (this->tribe) {
		this->tribe->setLoss(village);
	}
}