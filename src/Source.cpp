#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <fstream>
#include <deque>
#include <ctime>
#ifdef _WIN64
#include <experimental/filesystem>
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
#include <filesystem>
#endif

#include <thread>

#include <iostream>

#include "cURLread.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

#include "readData.h"

#include "drawMap.h"

#include "curl/curl.h"

#if defined(_DEBUG) && defined(_WIN64)
#pragma comment(lib, "libcurl_debug.lib")
#elif defined(_WIN64)
#pragma comment(lib, "libcurl.lib")
#endif

int main(int argc, char* argv[]) {

	uint64_t currTime = 0;
	std::chrono::milliseconds timespan(30000);		//	30 seconds

	CURL* cURL;

	size_t world = 0;

	time_t time = 0;

	std::string data = "";
	std::string url = "";

	std::string temp = "";
	size_t colors[45] = { 0 };

	curl_global_init(CURL_GLOBAL_DEFAULT);
	cURL = curl_easy_init();

	std::ifstream colorsFile;
	colorsFile.open("colors.txt");
	if (!colorsFile.is_open())
		return EXIT_FAILURE;

	for (size_t i = 0; std::getline(colorsFile, temp); i++) {

		colors[i * 3] = atoi(temp.substr(0, temp.find(' ')).c_str());
		temp.erase(0, temp.find(' ') + 1);

		colors[i * 3 + 1] = atoi(temp.substr(0, temp.find(' ')).c_str());
		temp.erase(0, temp.find(' ') + 1);

		colors[i * 3 + 2] = atoi(temp.substr(0, temp.find(' ')).c_str());

	}

	colorsFile.close();

	//while (1) {

		currTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch()).count();

		//while ((currTime % 86400000) > 60) {

			//std::this_thread::sleep_for(timespan);
			//currTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock().now().time_since_epoch()).count();

		//}

		std::ifstream worldsFile("worlds.txt");
		if (!worldsFile.is_open())
			return EXIT_FAILURE;
		
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

			std::deque<player*> topPlayerConqs(15, nullptr);
			std::deque<player*> topPlayerLosses(15, nullptr);

			size_t zoom = 500;

			url = "https://" + data;
			std::cout << "url: " << url << '\n';

			std::deque<tribe> tribes;
			try {

				readTribes(url, tribes, topTribes);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read tribes!\n";

			}

			std::deque<player> players;
			try {

			readPlayers(url, players, tribes, topPlayers);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read players!\n";

			}

			std::deque<village> villages;
			try {

			readVillages(url, villages, players, zoom);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read villages!\n";

			}

			try {

			readODt(url, tribes, topODt);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read tribe OD!\n";

			}

			try {

			readODAt(url, tribes, topODAt);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read tribe ODA!\n";

			}

			try {

			readODDt(url, tribes, topODDt);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read tribe ODD!\n";

			}

			try {

			readOD(url, players, topOD);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read player OD!\n";

			}

			try {

			readODA(url, players, topODA);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read player ODA!\n";

			}

			try {

			readODD(url, players, topODD);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read player ODD!\n";

			}

			try {

			readConq(url, villages, players, tribes, time, topTribeConqs, topTribeLosses, topPlayerConqs, topPlayerLosses);

			}
			catch (std::exception& e) {

				std::cout << "Failed to read conquers!\n";

			}

			data = data.substr(0, data.find('.'));

#ifdef _WIN64
			std::experimental::filesystem::create_directory("maps");
			std::experimental::filesystem::create_directory("maps/" + data);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
			std::filesystem::create_directory("maps");
			std::filesystem::create_directory("maps/" + data);
#endif

			drawTopTribes(data, colors, zoom, topTribes);
			drawTopPlayers(data, colors, zoom, topPlayers);

			drawTopTribeODA(data, colors, zoom, topODAt);
			drawTopTribeODD(data, colors, zoom, topODDt);

			drawTopPlayerODA(data, colors, zoom, topODA);
			drawTopPlayerODD(data, colors, zoom, topODD);

			drawTopTribeConqs(data, colors, zoom, topTribes, topTribeConqs);
			drawTopTribeLosses(data, colors, zoom, topTribes, topTribeLosses);

			drawTopPlayerConqs(data, colors, zoom, topPlayers, topPlayerConqs);
			drawTopPlayerLosses(data, colors, zoom, topPlayers, topPlayerLosses);

		}

		worldsFile.close();

	//}

}