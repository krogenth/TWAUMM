#pragma once
#include <deque>
#include <vector>
#include <string>

class Tribe;
class Player;

void drawTopTribes(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topTribes);
void drawTopTribeOD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topODTribes);
void drawTopTribeODA(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topODATribes);
void drawTopTribeODD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topODDTribes);
void drawTopTribeConqs(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topConqTribes);
void drawTopTribeLosses(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topLossTribes);

void drawTopPlayers(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topPlayers);
void drawTopPlayerOD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topODPlayers);
void drawTopPlayerODA(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topODAPlayers);
void drawTopPlayerODD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topODDPlayers);
void drawTopPlayerConqs(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topConqPlayers);
void drawTopPlayerLosses(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topLossPlayers);