#include <codecvt>
#include <locale>

#include "cURLread.h"

#include "readData.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

tribe* tbinarySearch(size_t tribeID, size_t lower, size_t upper, std::deque<tribe>& tribes) {

	while (lower <= upper) {

		size_t mid = (upper + lower) >> 1;

		if (tribes[mid].getID() > tribeID)
			upper = mid - 1;
		else if (tribes[mid].getID() < tribeID)
			lower = mid + 1;
		else
			return &tribes[mid];

	}

	return nullptr;

}

player* pbinarySearch(size_t playerID, size_t lower, size_t upper, std::deque<player>& players) {

	while (lower <= upper) {

		size_t mid = (upper + lower) >> 1;

		if (players[mid].getID() > playerID)
			upper = mid - 1;
		else if (players[mid].getID() < playerID)
			lower = mid + 1;
		else
			return &players[mid];

	}

	return nullptr;

}

village* vbinarySearch(size_t villageID, size_t lower, size_t upper, std::deque<village>& villages) {

	while (lower <= upper) {

		size_t mid = (upper + lower) >> 1;

		if (villages[mid].getID() > villageID)
			upper = mid - 1;
		else if (villages[mid].getID() < villageID)
			lower = mid + 1;
		else
			return &villages[mid];

	}

	return nullptr;

}

void readTribes(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topTribes) {

	//	$tribe_id, $name, $tag, $members, $villages, $points, $all_points, $rank

	url += "ally.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on ally.txt!");

	char buffer[256];
	std::string bufferString = "";

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = L"";
	std::string tempBuffer = "";
	std::wstring wtempBuffer = L"";

	while (!url_feof(handle)) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;
		
		//	add new tribe to the listing
		tribes.emplace_back(tribe());

		//	get tribe ID and store in newest listing
		tribes[tribes.size() - 1].setID(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		//	get tribe name and store in newest listing
		tempBuffer = bufferString.substr(0, bufferString.find(','));
		int tempBufferSize = 0;
		tempBuffer = curl_easy_unescape(handle, tempBuffer.c_str(), tempBuffer.length(), &tempBufferSize);
		size_t pos = 0;
		std::string replaceChar = " ";
		while ((pos = tempBuffer.find('+')) != std::string::npos) {
			tempBuffer.replace(pos, 1, replaceChar);
		}
		wide = converter.from_bytes(tempBuffer);
		tribes[tribes.size() - 1].setName(wide);
		bufferString.erase(0, bufferString.find(',') + 1);

		//	get tribe tag and store in newest listing
		tempBuffer = bufferString.substr(0, bufferString.find(','));
		tempBuffer = curl_easy_unescape(handle, tempBuffer.c_str(), tempBuffer.length(), &tempBufferSize);
		while ((pos = tempBuffer.find('+')) != std::string::npos) {
			tempBuffer.replace(pos, 1, replaceChar);
		}
		wide = converter.from_bytes(tempBuffer);
		tribes[tribes.size() - 1].setTag(wide);
		bufferString.erase(0, bufferString.find(',') + 1);

		//	get tribe tag and store in newest listing
		tribes[tribes.size() - 1].setMemberCount(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		tribes[tribes.size() - 1].setVillageCount(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		tribes[tribes.size() - 1].setTopPoints(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		tribes[tribes.size() - 1].setPoints(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		tribes[tribes.size() - 1].setRank(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

	}

	url_fclose(handle);

	for (auto& val : tribes) {

		if (val.getRank() <= 15)
			topTribes[val.getRank() - 1] = &val;

	}

}

void readPlayers(std::string url, std::deque<player>& players, std::deque<tribe>& tribes, std::deque<player*>& topPlayers) {

	//	$player_id, $name, $tribe_id, $villages, $points, $rank

	url += "tribe.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on tribe.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t tribeIDCheck = 0;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = L"";
	std::string tempBuffer = "";
	std::wstring wtempBuffer = L"";

	while (!url_feof(handle)) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = L"";

		bufferString = buffer;

		players.emplace_back(player());

		players[players.size() - 1].setID(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		tempBuffer = bufferString.substr(0, bufferString.find(','));
		int tempBufferSize = 0;
		tempBuffer = curl_easy_unescape(handle, tempBuffer.c_str(), tempBuffer.length(), &tempBufferSize);
		size_t pos = 0;
		std::string replaceChar = " ";
		while ((pos = tempBuffer.find('+')) != std::string::npos) {
			tempBuffer.replace(pos, 1, replaceChar);
		}
		wide = converter.from_bytes(tempBuffer);
		players[players.size() - 1].setName(wide);
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);
		if (tribeIDCheck) {

			players[players.size() - 1].setTribeID(tbinarySearch(tribeIDCheck, 0, tribes.size(), tribes));
			players[players.size() - 1].getTribeID()->addMemberID(&players[players.size() - 1]);

		}

		bufferString.erase(0, bufferString.find(',') + 1);	//we are not going by given village count, we will count them

		players[players.size() - 1].setPoints(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		players[players.size() - 1].setRank(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

	}

	url_fclose(handle);

	for (auto& val : players) {

		if (val.getRank() <= 15)
			topPlayers[val.getRank() - 1] = &val;

	}

}

void readVillages(std::string url, std::deque<village>& villages, std::deque<player>& players, size_t& zoom) {

	//	$village_id, $name, $x, $y, $player_id, $points, $rank

	url += "village.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on village.txt!");

	char buffer[256];
	std::string bufferString = "";
	std::tuple<size_t, size_t> coords;
	size_t playerIDCheck = 0;

	while (!url_feof(handle)) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		villages.push_back(village());

		villages[villages.size() - 1].setID(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));
		bufferString.erase(0, bufferString.find(',') + 1);

		bufferString.erase(0, bufferString.find(',') + 1);	//skip village name, it is useless

		std::get<0>(coords) = (size_t)atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		if (std::get<0>(coords) < zoom)
			zoom = std::get<0>(coords);

		std::get<1>(coords) = (size_t)atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		villages[villages.size() - 1].setCoord(coords);

		playerIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);
		if (playerIDCheck) {

			villages[villages.size() - 1].setPlayerID(pbinarySearch(playerIDCheck, 0, players.size(), players));
			villages[villages.size() - 1].getPlayerID()->addVillage(&villages[villages.size() - 1]);

		}

		villages[villages.size() - 1].setPoints(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

	}

	if (zoom > 399)
		zoom = 5;
	else if (zoom > 350)
		zoom = 3;
	else if (zoom > 260)
		zoom = 2;
	else
		zoom = 1;

	url_fclose(handle);

}

void readOD(std::string url, std::deque<player>& players, std::deque<player*>& topOD) {

	//	$rank, $id, $score

	url += "kill_all.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_all.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t rankCheck = 0;
	size_t playerIDCheck = 0;
	player* playerTemp = nullptr;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		rankCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		playerIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		playerTemp = pbinarySearch(playerIDCheck, 0, players.size(), players);
		playerTemp->setOD(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

		if (rankCheck <= 15)
			topOD[rankCheck - 1] = playerTemp;

	}

	url_fclose(handle);

}

void readODA(std::string url, std::deque<player>& players, std::deque<player*>& topODA) {

	//	$rank, $id, $score

	url += "kill_att.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_att.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t rankCheck = 0;
	size_t playerIDCheck = 0;
	player* playerTemp = nullptr;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		rankCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		playerIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		playerTemp = pbinarySearch(playerIDCheck, 0, players.size(), players);
		playerTemp->setODA(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

		if (rankCheck <= 15)
			topODA[rankCheck - 1] = playerTemp;

	}

	url_fclose(handle);

}

void readODD(std::string url, std::deque<player>& players, std::deque<player*>& topODD) {

	//	$rank, $id, $score

	url += "kill_def.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_def.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t rankCheck = 0;
	size_t playerIDCheck = 0;
	player* playerTemp = nullptr;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		rankCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		playerIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		playerTemp = pbinarySearch(playerIDCheck, 0, players.size(), players);
		playerTemp->setODD(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

		if (rankCheck <= 15)
			topODD[rankCheck - 1] = playerTemp;

	}

	url_fclose(handle);

}

void readODt(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topODt) {

	//	$rank, $id, $score

	url += "kill_all_tribe.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_all_tribe.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t rankCheck = 0;
	size_t tribeIDCheck = 0;
	tribe* tribeTemp = nullptr;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		rankCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeTemp = tbinarySearch(tribeIDCheck, 0, tribes.size(), tribes);
		tribeTemp->setOD(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

		if (rankCheck <= 15)
			topODt[rankCheck - 1] = tribeTemp;

	}

	url_fclose(handle);

}

void readODAt(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topODAt) {

	//	$rank, $id, $score

	url += "kill_att_tribe.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_att_tribe.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t rankCheck = 0;
	size_t tribeIDCheck = 0;
	tribe* tribeTemp = nullptr;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		rankCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeTemp = tbinarySearch(tribeIDCheck, 0, tribes.size(), tribes);
		tribeTemp->setODA(atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str()));

		if (rankCheck <= 15)
			topODAt[rankCheck - 1] = tribeTemp;

	}

	url_fclose(handle);

}

void readODDt(std::string url, std::deque<tribe>& tribes, std::deque<tribe*>& topODDt) {

	//	$rank, $id, $score

	url += "kill_def_tribe.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_att_tribe.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t rankCheck = 0;
	size_t tribeIDCheck = 0;
	tribe* tribeTemp = nullptr;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		rankCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeIDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		tribeTemp = tbinarySearch(tribeIDCheck, 0, tribes.size(), tribes);
		tribeTemp->setODD(atoi((char*)bufferString.c_str()));

		if (rankCheck <= 15)
			topODDt[rankCheck - 1] = tribeTemp;

	}

	url_fclose(handle);

}

void readConq(std::string url, std::deque<village>& villages, std::deque<player>& players, std::deque<tribe>& tribes, time_t time, std::deque<tribe*>& topTribeConqs, std::deque<tribe*>& topTribeLosses, std::deque<player*>& topConqs, std::deque<player*>& topLosses) {

	//	$village_id, $unix_timestamp, $new_owner, $old_owner

	url += "conquer.txt";

	URL_FILE* handle = url_fopen(url.c_str(), "r");
	if (!handle)
		throw std::exception("cURL handle failed on kill_all.txt!");

	char buffer[256];
	std::string bufferString = "";
	size_t IDCheck = 0;
	tribe* tribeTemp = nullptr;
	player* playerTemp = nullptr;
	village* villageTemp = nullptr;
	time_t timeStamp = 0;

	for (size_t i = 0; !url_feof(handle); i++) {

		if (url_fgets((char*)buffer, sizeof(buffer), handle) == NULL) {

			url_fclose(handle);
			return;

		}

		bufferString = buffer;

		//	grab village ID and store it for verification, remove information from buffer
		IDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);
		//	verify that the village ID is non-zero
		if (IDCheck)
			villageTemp = vbinarySearch(IDCheck, 0, villages.size(), villages);

		//	we did not find the village, just continue
		if (!villageTemp)
			continue;

		//	grab the conquer timestamp and store, remove information from buffer
		timeStamp = atoll((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);

		//	compare timstamp to current unix timestamp, if it is NOT within the set timeframe, we can skip this one
		if (timeStamp < time - 86400)		//	86400 = 1 day, 604800 seconds = 7 days
			continue;

		//	grab the new player owner ID for verification, remove information from buffer
		IDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		bufferString.erase(0, bufferString.find(',') + 1);
		if (IDCheck)
			playerTemp = pbinarySearch(IDCheck, 0, players.size(), players);

		if (playerTemp) {

			playerTemp->setConq(villageTemp);
			if (playerTemp->getTribeID())
				playerTemp->getTribeID()->setConq(villageTemp);

		}

		//	grab the old player owner ID for verification, last piece of data, no need to remove from buffer
		IDCheck = atoi((char*)bufferString.substr(0, bufferString.find(',')).c_str());
		if (IDCheck)
			playerTemp = pbinarySearch(IDCheck, 0, players.size(), players);

		if (playerTemp) {

			playerTemp->setLoss(villageTemp);
			if (playerTemp->getTribeID())
				playerTemp->getTribeID()->setLoss(villageTemp);

		}

	}

	url_fclose(handle);

	for (auto& val : tribes) {

		for (size_t i = 0; i < 15; i++) {

			if (!topTribeConqs[i]) {

				topTribeConqs[i] = &val;
				break;

			}
			else if (topTribeConqs[i]->getConqPoints() < val.getConqPoints()) {

				for (size_t j = 14; j > i; j--)
					topTribeConqs[j] = topTribeConqs[j - 1];

				topTribeConqs[i] = &val;
				break;

			}

		}

		for (size_t i = 0; i < 15; i++) {

			if (!topTribeLosses[i]) {

				topTribeLosses[i] = &val;
				break;

			}
			else if (topTribeLosses[i]->getLossPoints() < val.getLossPoints()) {

				for (size_t j = 14; j > i; j--)
					topTribeLosses[j] = topTribeLosses[j - 1];

				topTribeLosses[i] = &val;
				break;

			}

		}

	}

	for (auto& val : players) {

		for (size_t i = 0; i < 15; i++) {

			if (!topConqs[i]) {

				topConqs[i] = &val;
				break;

			}
			else if (topConqs[i]->getConqPoints() < val.getConqPoints()) {

				for (size_t j = 14; j > i; j--)
					topConqs[j] = topConqs[j - 1];

				topConqs[i] = &val;
				break;

			}

		}

		for (size_t i = 0; i < 15; i++) {

			if (!topLosses[i]) {

				topLosses[i] = &val;
				break;

			}
			else if (topLosses[i]->getLossPoints() < val.getLossPoints()) {

				for (size_t j = 14; j > i; j--)
					topLosses[j] = topLosses[j - 1];

				topLosses[i] = &val;
				break;

			}

		}

	}

}