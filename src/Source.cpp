#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <fstream>
#include <deque>
#include <ctime>
#ifdef _WIN64
#include <experimental/filesystem>
#endif
#ifdef unix | __unix | __unix__
#include <filesystem>
#endif

#include <thread>

//#include <iostream>

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

	uint64_t currTime = 0;
	std::chrono::milliseconds timespan(30000);		//	30 seconds

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

	while (1) {

		currTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch()).count();

		while ((currTime % 86400000) > 60) {

			std::this_thread::sleep_for(timespan);
			currTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch()).count();

		}

		worldsFile.seekg(std::ios::beg);
		
		while (std::getline(worldsFile, data)) {

			if (data[0] == ';')
				continue;

			time = std::time(NULL);

			std::deque<tribe*> topTribes(15, nullptr);
			std::deque<player*> topPlayers(15, nullptr);

			std::deque<tribe*> topPlayerODt(15, nullptr);
			std::deque<tribe*> topPlayerODAt(15, nullptr);
			std::deque<tribe*> topPlayerODDt(15, nullptr);

			std::deque<player*> topPlayerOD(15, nullptr);
			std::deque<player*> topPlayerODA(15, nullptr);
			std::deque<player*> topPlayerODD(15, nullptr);

			std::deque<tribe*> topTribeConqs(15, nullptr);
			std::deque<tribe*> topTribeLosses(15, nullptr);

			std::deque<player*> topPlayerConqs(15, nullptr);
			std::deque<player*> topPlayerLosses(15, nullptr);

			size_t zoom = 500;

			url = "https://" + data + "/map/";

			std::deque<tribe> tribes;
			readTribes(url, tribes, topTribes);

			std::deque<player> players;
			readPlayers(url, players, tribes, topPlayers);

			std::deque<village> villages;
			readVillages(url, villages, players, zoom);

			readODt(url, tribes, topPlayerODt);
			readODAt(url, tribes, topPlayerODAt);
			readODDt(url, tribes, topPlayerODDt);

			readOD(url, players, topPlayerOD);
			readODA(url, players, topPlayerODA);
			readODD(url, players, topPlayerODD);

			readConq(url, villages, players, tribes, time, topTribeConqs, topTribeLosses, topPlayerConqs, topPlayerLosses);

			data = data.substr(0, data.find('.'));

#ifdef _WIN64
			std::experimental::filesystem::create_directory("maps");
			std::experimental::filesystem::create_directory("maps/" + data);
#endif
#ifdef unix | __unix | __unix__
			std::filesystem::create_directory("maps");
			std::filesystem::create_directory("maps/" + data);
#endif

			drawTopTribes(data, colors, zoom, topTribes);
			drawTopPlayers(data, colors, zoom, topPlayers);

			drawTopTribeODA(data, colors, zoom, topPlayerODAt);
			drawTopTribeODD(data, colors, zoom, topPlayerODDt);

			drawTopPlayerODA(data, colors, zoom, topPlayerODA);
			drawTopPlayerODD(data, colors, zoom, topPlayerODD);

			drawTopTribeConqs(data, colors, zoom, topTribes, topTribeConqs);
			drawTopTribeLosses(data, colors, zoom, topTribes, topTribeLosses);

			drawTopPlayerConqs(data, colors, zoom, topPlayers, topPlayerConqs);
			drawTopPlayerLosses(data, colors, zoom, topPlayers, topPlayerLosses);

		}

	}

}