#pragma once

#include <deque>
#include <string>

class Tribe;
class Player;
class Village;

std::deque<Tribe> readTribes(std::string url, std::deque<Tribe*>& topTribes);
std::deque<Player> readPlayers(std::string url, std::deque<Tribe>& tribes, std::deque<Player*>& topPlayers);
std::deque<Village> readVillages(std::string url, std::deque<Player>& players, uint32_t& zoom);

void readOD(std::string url, std::deque<Player>& players, std::deque<Player*>& topOD);
void readODA(std::string url, std::deque<Player>& players, std::deque<Player*>& topODA);
void readODD(std::string url, std::deque<Player>& players, std::deque<Player*>& topODD);

void readODt(std::string url, std::deque<Tribe>& tribes, std::deque<Tribe*>& topODt);
void readODAt(std::string url, std::deque<Tribe>& tribes, std::deque<Tribe*>& topODAt);
void readODDt(std::string url, std::deque<Tribe>& tribes, std::deque<Tribe*>& topODDt);

void readConq(std::string url, const std::deque<Village>& villages, std::deque<Player>& players, std::deque<Tribe>& tribes, uint64_t time);
void getTopConqLoss(std::deque<Player>& players, std::deque<Tribe>& tribes, std::deque<Tribe*>& topTribeConqs, std::deque<Tribe*>& topTribeLosses, std::deque<Player*>& topPlayerConqs, std::deque<Player*>& topPlayerLosses);