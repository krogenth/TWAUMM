#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <fstream>
#include <deque>
#include <ctime>
#include <experimental/filesystem>

#include "cURLread.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

#include "readData.h"

#include "drawMap.h"

#include "curl\curl.h"

#ifdef _DEBUG
#pragma comment(lib, "libcurl_debug.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif

int main(int argc, char* argv[]) {

	CURL* cURL;

	size_t world = 0;
	std::string server = "";

	time_t time = 0;

	std::string data = "";
	std::string url = "";

	std::string temp = "";
	size_t colors[45] = { 0 };

	std::ifstream worldsFile("worlds.txt");
	if (!worldsFile.is_open())
		return EXIT_FAILURE;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	cURL = curl_easy_init();

	std::ifstream colorsFile;
	colorsFile.open("colors.txt");

	for (size_t i = 0; std::getline(colorsFile, temp); i++) {

		colors[i * 3] = atoi(temp.substr(0, temp.find(' ')).c_str());
		temp.erase(0, temp.find(' ') + 1);

		colors[i * 3 + 1] = atoi(temp.substr(0, temp.find(' ')).c_str());
		temp.erase(0, temp.find(' ') + 1);

		colors[i * 3 + 2] = atoi(temp.substr(0, temp.find(' ')).c_str());

	}

	while (std::getline(worldsFile, data)) {

		if (data[0] == ';')
			continue;

		time = std::time(NULL);

		std::deque<tribe*> topTribes(15, nullptr);
		std::deque<player*> topPlayers(15, nullptr);

		std::deque<tribe*> topODt(15, nullptr);
		std::deque<tribe*> topODAt(15, nullptr);
		std::deque<tribe*> topODDt(15, nullptr);

		std::deque<player*> topOD(15, nullptr);
		std::deque<player*> topODA(15, nullptr);
		std::deque<player*> topODD(15, nullptr);

		std::deque<tribe*> topTribeConqs(15, nullptr);
		std::deque<tribe*> topTribeLosses(15, nullptr);
		
		std::deque<player*> topConqs(15, nullptr);
		std::deque<player*> topLosses(15, nullptr);

		size_t zoom = 500;

		url = "https://" + data + "/map/";

		std::deque<tribe> tribes;
		readTribes(url, tribes, topTribes);

		std::deque<player> players;
		readPlayers(url, players, tribes, topPlayers);

		std::deque<village> villages;
		readVillages(url, villages, players, zoom);

		readODt(url, tribes, topODt);
		readODAt(url, tribes, topODAt);
		readODDt(url, tribes, topODDt);

		readOD(url, players, topOD);
		readODA(url, players, topODA);
		readODD(url, players, topODD);

		readConq(url, villages, players, tribes, time, topTribeConqs, topTribeLosses, topConqs, topLosses);

		data = data.substr(0, data.find('.'));

		std::experimental::filesystem::create_directory("maps");
		std::experimental::filesystem::create_directory("maps/" + data);
		/*
		void drawTopPlayers(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers);

		void drawTopTribeODA(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODATribes);
		void drawTopTribeODD(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODDTribes);

		void drawTopPlayerODA(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODAPlayers);
		void drawTopPlayerODD(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODDPlayers);

		void drawTopTribeConqs(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topConqTribes);
		void drawTopTribeLosses(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topLossTribes);

		void drawTopPlayerConqs(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topConqPlayers);
		void drawTopPlayerLosses(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topLossPlayers);
		*/


		drawTopTribes(data, colors, zoom, topTribes);
		drawTopPlayers(data, colors, zoom, topPlayers);

		drawTopTribeODA(data, colors, zoom, topODAt);
		drawTopTribeODD(data, colors, zoom, topODDt);

		drawTopPlayerODA(data, colors, zoom, topODA);
		drawTopPlayerODD(data, colors, zoom, topODD);

		drawTopTribeConqs(data, colors, zoom, topTribes, topTribeConqs);
		drawTopTribeLosses(data, colors, zoom, topTribes, topTribeLosses);

		drawTopPlayerConqs(data, colors, zoom, topPlayers, topConqs);
		drawTopPlayerLosses(data, colors, zoom, topPlayers, topLosses);

	}

}