#include <vector>
#include <filesystem>
#include <thread>
#include <iostream>
#include <array>
#include <tuple>
#include <fstream>

#include <nlohmann/json.hpp>

#include "./initialization/initialization.h"
#include "./handleData/readData.h"
#include "./village/village.h"
#include "./player/player.h"
#include "./tribe/tribe.h"
#include "./draw/draw.h"

void mainLoop();
nlohmann::json loadConfig(std::string filename);
void sleepTilNextDay();
std::string getWorldTag(std::string world);

int main(int argc, char* argv[]) {
	while (1) {
		mainLoop();
		sleepTilNextDay();
	}
}

void mainLoop() {
	auto config = loadConfig("config.json");
	if (config.empty()) {
		std::cout << "Failed to load config.json" << std::endl;
		return;
	}

	//	verify the output directory exists already
	if (!std::filesystem::exists(config["outdir"].get<std::string>())) {
		std::filesystem::create_directory(config["outdir"].get<std::string>());
	}

	auto colors = readColorsFile(config["colors_file"].get<std::string>());
	auto worlds = readWorldsFile(config["worlds_file"].get<std::string>());
	for (const auto& world : worlds) {
		std::deque<Tribe*> topTribes(15, nullptr);
		std::deque<Tribe*> topODt(15, nullptr);
		std::deque<Tribe*> topODAt(15, nullptr);
		std::deque<Tribe*> topODDt(15, nullptr);
		std::deque<Tribe*> topTribeConqs(15, nullptr);
		std::deque<Tribe*> topTribeLosses(15, nullptr);

		std::deque<Player*> topPlayers(15, nullptr);
		std::deque<Player*> topOD(15, nullptr);
		std::deque<Player*> topODA(15, nullptr);
		std::deque<Player*> topODD(15, nullptr);
		std::deque<Player*> topPlayerConqs(15, nullptr);
		std::deque<Player*> topPlayerLosses(15, nullptr);

		uint32_t zoom = 500;

		auto tribes = readTribes("https://" + world, topTribes);
		auto players = readPlayers("https://" + world, tribes, topPlayers);
		auto villages = readVillages("https://" + world, players, zoom);
		readODt("https://" + world, tribes, topODt);
		readODAt("https://" + world, tribes, topODAt);
		readODDt("https://" + world, tribes, topODDt);
		readOD("https://" + world, players, topOD);
		readODA("https://" + world, players, topODA);
		readODD("https://" + world, players, topODD);
		readConq("https://" + world, villages, players, tribes, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch()).count());
		getTopConqLoss(players, tribes, topTribeConqs, topTribeLosses, topPlayerConqs, topPlayerLosses);

		// verify specified world directory exists
		if (!std::filesystem::exists(config["outdir"].get<std::string>() + "/" + getWorldTag(world))) {
			std::filesystem::create_directory(config["outdir"].get<std::string>() + "/" + getWorldTag(world));
		}

		drawTopTribes(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topTribes);
		drawTopTribeOD(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topODt);
		drawTopTribeODA(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topODAt);
		drawTopTribeODD(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topODDt);
		drawTopTribeConqs(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topTribeConqs);
		drawTopTribeLosses(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topTribeLosses);

		drawTopPlayers(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topPlayers);
		drawTopPlayerOD(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topOD);
		drawTopPlayerODA(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topODA);
		drawTopPlayerODD(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topODD);
		drawTopPlayerConqs(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topPlayerConqs);
		drawTopPlayerLosses(config["outdir"].get<std::string>(), getWorldTag(world), colors, zoom, topPlayerLosses);
	}
}

nlohmann::json loadConfig(std::string filename) {
	nlohmann::json json;
	std::ifstream file(filename);
	if (file.is_open()) {
		file >> json;
		file.close();
	}
	return json;
}

void sleepTilNextDay() {
	uint64_t currTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch()).count();
	uint64_t dayInSeconds = 60 * 60 * 24;
	uint64_t timeToNextDay = dayInSeconds - (currTime % dayInSeconds);
	std::this_thread::sleep_for(std::chrono::seconds(timeToNextDay));
}

std::string getWorldTag(std::string world) {
	return world.substr(0, world.find('.'));
}