#pragma once
#include <deque>
#include <string>

class tribe;
class player;

void drawTopTribes(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes);
void drawTopPlayers(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers);

void drawTopTribeOD(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODTribes);
void drawTopTribeODA(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODATribes);
void drawTopTribeODD(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODDTribes);

void drawTopPlayerOD(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODPlayers);
void drawTopPlayerODA(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODAPlayers);
void drawTopPlayerODD(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODDPlayers);

void drawTopTribeConqs(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topConqTribes);
void drawTopTribeLosses(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topLossTribes);

void drawTopPlayerConqs(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topConqPlayers);
void drawTopPlayerLosses(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topLossPlayers);