#include "./village.h"

#include "../player/player.h"

void Village::setPlayer(Player* player) {
	this->player = player;
	this->player->addVillage(this);
}