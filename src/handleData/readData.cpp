#include "./readData.h"

#include <locale>
#include <sstream>
#include <algorithm>
#include <iostream>

#include <httplib.h>

#include "./search/search.h"

#include "../tribe/tribe.h"
#include "../player/player.h"
#include "../village/village.h"

#include "../conversion/stringConversion.h"

std::deque<Tribe> readTribes(std::string url, std::deque<Tribe*>& topTribes) {
	//	$tribe_id, $name, $tag, $members, $villages, $points, $all_points, $rank
	std::deque<Tribe> tribes;

	httplib::Client cli(url);
	auto res = cli.Get("/map/ally.txt");

	std::stringstream sstream(res->body);
	std::string tribeData = "";
	while (std::getline(sstream, tribeData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream tribeStream(tribeData);
			while (std::getline(tribeStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}
		// replace '+' characters in the tribe name and tag
		std::replace(dataVec.at(1).begin(), dataVec.at(1).end(), '+', ' ');
		std::replace(dataVec.at(2).begin(), dataVec.at(2).end(), '+', ' ');

		tribes.emplace_back(Tribe());
		tribes.back().setID(std::stoull(dataVec.at(0)));
		tribes.back().setName(string_to_wide_string(dataVec.at(1)));
		tribes.back().setTag(string_to_wide_string(dataVec.at(2)));
		tribes.back().setMemberCount(std::stoull(dataVec.at(3)));
		tribes.back().setVillageCount(std::stoull(dataVec.at(4)));
		tribes.back().setTopPoints(std::stoull(dataVec.at(5)));
		tribes.back().setPoints(std::stoull(dataVec.at(6)));
		tribes.back().setRank(std::stoull(dataVec.at(7)));

		if (tribes.back().getRank() <= 15) {
			topTribes.at(tribes.back().getRank() - 1) = &tribes.back();
		}

		dataVec.clear();
	}
	
	return tribes;
}

std::deque<Player> readPlayers(std::string url, std::deque<Tribe>& tribes, std::deque<Player*>& topPlayers) {
	//	$player_id, $name, $tribe_id, $villages, $points, $rank
	std::deque<Player> players;

	httplib::Client cli(url);
	auto res = cli.Get("/map/tribe.txt");

	std::stringstream sstream(res->body);
	std::string playerData = "";
	while (std::getline(sstream, playerData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream playerStream(playerData);
			while (std::getline(playerStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}
		// replace '+' characters in the tribe name and tag
		std::replace(dataVec.at(1).begin(), dataVec.at(1).end(), '+', ' ');

		auto tribe = searchDequeByID<Tribe>(tribes, std::stoull(dataVec.at(2)));

		players.emplace_back(Player());
		players.back().setID(std::stoull(dataVec.at(0)));
		players.back().setName(string_to_wide_string(dataVec.at(1)));

		if(tribe) players.back().setTribe(tribe);
		players.back().setPoints(std::stoull(dataVec.at(4)));
		players.back().setRank(std::stoull(dataVec.at(5)));

		if (players.back().getRank() <= 15) {
			topPlayers.at(players.back().getRank() - 1) = &players.back();
		}

		if (players.back().getRank() <= 15) {
			topPlayers.at(players.back().getRank() - 1) = &players.back();
		}

		dataVec.clear();
	}

	return players;
}

std::deque<Village> readVillages(std::string url, std::deque<Player>& players, uint32_t& zoom) {
	//	$village_id, $name, $x, $y, $player_id, $points, $rank
	std::deque<Village> villages;

	httplib::Client cli(url);
	auto res = cli.Get("/map/village.txt");

	std::stringstream sstream(res->body);
	std::string villageData = "";
	while (std::getline(sstream, villageData)) {
		std::deque<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(villageData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}
		auto player = searchDequeByID<Player>(players, std::stoull(dataVec.at(4)));

		villages.emplace_back(Village());
		villages.back().setID(std::stoull(dataVec.at(0)));
		villages.back().setCoord(std::make_tuple(std::stoul(dataVec.at(2)), std::stoul(dataVec.at(3))));
		if(player) villages.back().setPlayer(player);
		villages.back().setPoints(std::stoull(dataVec.at(5)));

		// update zoom based on x coordinate
		if (std::get<0>(villages.back().getCoord()) < zoom)
			zoom = std::get<0>(villages.back().getCoord());

		dataVec.clear();
	}

	if (zoom > 399)
		zoom = 4;
	else if (zoom > 260)
		zoom = 2;
	else
		zoom = 1;

	return villages;
}

void readOD(std::string url, std::deque<Player>& players, std::deque<Player*>& topOD) {
	//	$rank, $id, $score
	httplib::Client cli(url);
	auto res = cli.Get("/map/kill_all.txt");

	std::stringstream sstream(res->body);
	std::string odData = "";
	while (std::getline(sstream, odData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(odData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		auto player = searchDequeByID<Player>(players, std::stoull(dataVec.at(1)));
		if (!player) {
			continue;
		}

		player->setOD(std::stoull(dataVec.at(2)));
		auto rank = std::stoull(dataVec.at(0));

		// check if the rank is in top 15
		if (rank <= 15) {
			topOD.at(rank - 1) = player;
		}

		dataVec.clear();
	}
}

void readODA(std::string url, std::deque<Player>& players, std::deque<Player*>& topODA) {
	//	$rank, $id, $score
	httplib::Client cli(url);
	auto res = cli.Get("/map/kill_att.txt");

	std::stringstream sstream(res->body);
	std::string odaData = "";
	while (std::getline(sstream, odaData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(odaData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		auto player = searchDequeByID<Player>(players, std::stoull(dataVec.at(1)));
		if (!player) {
			continue;
		}

		player->setODA(std::stoull(dataVec.at(2)));
		auto rank = std::stoull(dataVec.at(0));

		// check if the rank is in top 15
		if (rank <= 15) {
			topODA.at(rank - 1) = player;
		}

		dataVec.clear();
	}
}

void readODD(std::string url, std::deque<Player>& players, std::deque<Player*>& topODD) {
	//	$rank, $id, $score
	httplib::Client cli(url);
	auto res = cli.Get("/map/kill_def.txt");

	std::stringstream sstream(res->body);
	std::string oddData = "";
	while (std::getline(sstream, oddData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(oddData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		auto player = searchDequeByID<Player>(players, std::stoull(dataVec.at(1)));
		if (!player) {
			continue;
		}

		player->setODD(std::stoull(dataVec.at(2)));
		auto rank = std::stoull(dataVec.at(0));

		// check if the rank is in top 15
		if (rank <= 15) {
			topODD.at(rank - 1) = player;
		}

		dataVec.clear();
	}
}

void readODt(std::string url, std::deque<Tribe>& tribes, std::deque<Tribe*>& topODt) {
	//	$rank, $id, $score
	httplib::Client cli(url);
	auto res = cli.Get("/map/kill_all_tribe.txt");

	std::stringstream sstream(res->body);
	std::string odData = "";
	while (std::getline(sstream, odData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(odData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		auto tribe = searchDequeByID<Tribe>(tribes, std::stoull(dataVec.at(1)));
		if (!tribe) {
			continue;
		}

		tribe->setOD(std::stoull(dataVec.at(2)));
		auto rank = std::stoull(dataVec.at(0));

		// check if the rank is in top 15
		if (rank <= 15) {
			topODt.at(rank - 1) = tribe;
		}

		dataVec.clear();
	}
}

void readODAt(std::string url, std::deque<Tribe>& tribes, std::deque<Tribe*>& topODAt) {
	//	$rank, $id, $score
	httplib::Client cli(url);
	auto res = cli.Get("/map/kill_att_tribe.txt");

	std::stringstream sstream(res->body);
	std::string odaData = "";
	while (std::getline(sstream, odaData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(odaData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		auto tribe = searchDequeByID<Tribe>(tribes, std::stoull(dataVec.at(1)));
		if (!tribe) {
			continue;
		}

		tribe->setODA(std::stoull(dataVec.at(2)));
		auto rank = std::stoull(dataVec.at(0));

		// check if the rank is in top 15
		if (rank <= 15) {
			topODAt.at(rank - 1) = tribe;
		}

		dataVec.clear();
	}
}

void readODDt(std::string url, std::deque<Tribe>& tribes, std::deque<Tribe*>& topODDt) {
	//	$rank, $id, $score
	httplib::Client cli(url);
	auto res = cli.Get("/map/kill_def_tribe.txt");

	std::stringstream sstream(res->body);
	std::string oddData = "";
	while (std::getline(sstream, oddData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(oddData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		auto tribe = searchDequeByID<Tribe>(tribes, std::stoull(dataVec.at(1)));
		if (!tribe) {
			continue;
		}

		tribe->setODD(std::stoull(dataVec.at(2)));
		auto rank = std::stoull(dataVec.at(0));

		// check if the rank is in top 15
		if (rank <= 15) {
			topODDt.at(rank - 1) = tribe;
		}

		dataVec.clear();
	}
}

void readConq(std::string url, const std::deque<Village>& villages, std::deque<Player>& players, std::deque<Tribe>& tribes, uint64_t time) {
	//	$village_id, $unix_timestamp, $new_owner, $old_owner
	httplib::Client cli(url);
	auto res = cli.Get("/map/conquer.txt");

	std::stringstream sstream(res->body);
	std::string conqData = "";
	while (std::getline(sstream, conqData)) {
		std::vector<std::string> dataVec;
		{
			std::string data = "";
			char delimiter = ',';
			std::stringstream villageStream(conqData);
			while (std::getline(villageStream, data, delimiter)) {
				dataVec.emplace_back(data);
			}
		}

		// check if the conquer is not within the last 24 hours
		if (std::stoull(dataVec.at(1)) < (time - (uint64_t)86400)) {
			continue;
		}

		auto village = searchDequeByID<Village>(villages, std::stoull(dataVec.at(0)));
		auto conquerer = searchDequeByID<Player>(players, std::stoull(dataVec.at(2)));
		auto conquered = searchDequeByID<Player>(players, std::stoull(dataVec.at(3)));
		
		// verify the village exists
		if (!village) {
			continue;
		}

		if (conquerer) {
			conquerer->setConq(village);
		}
		if (conquered) {
			conquered->setLoss(village);
		}
	}
}

void getTopConqLoss(std::deque<Player>& players, std::deque<Tribe>& tribes, std::deque<Tribe*>& topTribeConqs, std::deque<Tribe*>& topTribeLosses, std::deque<Player*>& topPlayerConqs, std::deque<Player*>& topPlayerLosses) {
	std::deque<Tribe> sortedTribes(tribes);
	std::deque<Player> sortedPlayers(players);
	
	std::sort(sortedTribes.begin(), sortedTribes.end(), [](Tribe& a, Tribe& b) {
		return a.getConqPoints() > b.getConqPoints();
		});
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](Player& a, Player& b) {
		return a.getConqPoints() > b.getConqPoints();
		});
	for (size_t i = 0; i < 15; i++) {
		if (i < sortedTribes.size()) {
			topTribeConqs.at(i) = searchDequeByID(tribes, sortedTribes.at(i).getID());
		}
		if (i < sortedPlayers.size()) {
			topPlayerConqs.at(i) = searchDequeByID(players, sortedPlayers.at(i).getID());
		}
	}

	std::sort(sortedTribes.begin(), sortedTribes.end(), [](Tribe& a, Tribe& b) {
		return a.getLossPoints() > b.getLossPoints();
		});
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](Player& a, Player& b) {
		return a.getLossPoints() > b.getLossPoints();
		});
	for (size_t i = 0; i < 15; i++) {
		if (i < sortedTribes.size()) {
			topTribeLosses.at(i) = searchDequeByID(tribes, sortedTribes.at(i).getID());
		}
		if (i < sortedPlayers.size()) {
			topPlayerLosses.at(i) = searchDequeByID(players, sortedPlayers.at(i).getID());
		}
	}
}