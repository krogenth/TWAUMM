#pragma once
#include <deque>
#include <string>

class tribe;
class player;
class village;

tribe* tbinarySearch(size_t tribeID, size_t lower, size_t upper, const std::deque<tribe>& tribes);
player* pbinarySearch(size_t playerID, size_t lower, size_t upper, const std::deque<player>& players);
village* vbinarySearch(size_t villageID, size_t lower, size_t upper, const std::deque<village>& villages);

void readTribes(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topTribes);

void readPlayers(std::string url, std::deque<player>& players, std::deque<tribe>& tribes, std::deque<player*>& topPlayers);

void readVillages(std::string url, std::deque<village>& villages, std::deque<player>& players, size_t& zoom);

void readOD(std::string url, std::deque<player>& players, std::deque<player*>& topOD);
void readODA(std::string url, std::deque<player>& players, std::deque<player*>& topODA);
void readODD(std::string url, std::deque<player>& players, std::deque<player*>& topODD);

void readODt(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topODt);
void readODAt(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topODAt);
void readODDt(std::string url, std::deque<tribe> & tribes, std::deque<tribe*>& topODDt);

void readConq(std::string url, std::deque<village>& villages, std::deque<player>& players, std::deque<tribe>& tribes, time_t time, std::deque<tribe*>& topTribeConqs, std::deque<tribe*>& topTribeLosses, std::deque<player*>& topConqs, std::deque<player*>& topLosses);
