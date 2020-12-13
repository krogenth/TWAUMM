#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <cinttypes>
#ifdef _WIN64
#include <experimental/filesystem>
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
#include <filesystem>
#endif

#include <codecvt>
#include <locale>
#include <memory>
#include <cmath>
#include <stdio.h>

#include "gd.h"
#include "formatData.h"
#include "drawMap.h"

#include "tribe.h"
#include "player.h"
#include "village.h"

void drawTopTribes(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes) {

	gdImagePtr image;
	FILE* topTribeOut;
	std::string filePath = "";

#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topTribe.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topTribe.png";
#endif
	topTribeOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);
				

				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < (uint32_t)((float)kLength * worldLength); i += kLength) {

		uint32_t check = (uint32_t)((float)kLength * partialK) + i;

		gdImageLine(image, check, 30, check, 1030, kNumColor);
		gdImageLine(image, 0, check + 30, 1000, check + 30, kNumColor);

	}

	int rect[8];
	/*	meant for gdImageStringFTEx for unicode, but cannot get to work
	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = &fontPath1[0];
	*/

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topTribes[i]->getTag());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena-Condensed-Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topTribes.at(i)->getPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena-Condensed-Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena-Condensed-Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		data = "zoom: " + std::to_string(zoom);
		gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOut);

	fclose(topTribeOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayers(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers) {

	gdImagePtr image;
	FILE* topPlayerOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topPlayer.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topPlayer.png";
#endif

	topPlayerOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

				x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
				y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

				x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
				y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];
	/*	meant for gdImageStringFTEx for unicode, but cannot get to work
	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 0;
	gdStringExtras.vdpi = 0;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = &fontPath1[0];
	*/

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topPlayers[i]->getName());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena-Condensed-Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topPlayers.at(i)->getPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena-Condensed-Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena-Condensed-Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOut);

	fclose(topPlayerOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeODA(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODATribes) {

	gdImagePtr image;
	FILE* topTribeOdaOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topTribeODA.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topTribeODA.png";
#endif
	topTribeOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODATribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODATribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODATribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODATribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODATribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODATribes[i]->getTag());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topODATribes.at(i)->getODA());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topODATribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODA Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOdaOut);

	fclose(topTribeOdaOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeODD(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topODDTribes) {

	gdImagePtr image;
	FILE* topTribeOddOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topTribeODD.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topTribeODD.png";
#endif
	topTribeOddOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODDTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODDTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODDTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topODDTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topODDTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODDTribes[i]->getTag());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topODDTribes.at(i)->getODD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topODDTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODD Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOddOut);

	fclose(topTribeOddOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerODA(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODAPlayers) {

	gdImagePtr image;
	FILE* topPlayerOdaOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topPlayerODA.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topPlayerODA.png";
#endif
	topPlayerOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODAPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODAPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODAPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODAPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODAPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODAPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODAPlayers[i]->getName());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topODAPlayers.at(i)->getODA());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topODAPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODA Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOdaOut);

	fclose(topPlayerOdaOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerODD(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topODDPlayers) {

	gdImagePtr image;
	FILE* topPlayerOddOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topPlayerODD.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topPlayerODD.png";
#endif
	topPlayerOddOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODDPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODDPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODDPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topODDPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topODDPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topODDPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODDPlayers[i]->getName());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topODDPlayers.at(i)->getODD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topODDPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODD Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOddOut);

	fclose(topPlayerOddOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeConqs(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topConqTribes) {

	gdImagePtr image;
	FILE* topTribeConqOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topTribeConq.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topTribeConq.png";
#endif
	topTribeConqOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51),
			charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
			 gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topConqTribes.at(i)->getConqCount(); j++) {

			x = std::get<0>(topConqTribes.at(i)->getConq(j)->getCoord());
			y = std::get<1>(topConqTribes.at(i)->getConq(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topConqTribes.at(i)->getConqCount(); j++) {

				x = std::get<0>(topConqTribes.at(i)->getConq(j)->getCoord());
				y = std::get<1>(topConqTribes.at(i)->getConq(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topConqTribes[i]->getTag());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topConqTribes.at(i)->getConqPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topConqTribes.at(i)->getConqCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobling Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeConqOut);

	fclose(topTribeConqOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopTribeLosses(std::string world, size_t* colors, size_t zoom, std::deque<tribe*>& topTribes, std::deque<tribe*>& topLossTribes) {

	gdImagePtr image;
	FILE* topTribeLossOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topTribeLoss.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topTribeLoss.png";
#endif
	topTribeLossOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
			bgcolor = gdImageColorAllocate(image, 0, 120, 0),
			white = gdImageColorAllocate(image, 255, 255, 255),
			black = gdImageColorAllocate(image, 0, 0, 0),
			charcoal = gdImageColorAllocate(image, 51, 51, 51),
			charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
		gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topTribes.at(i)->getMemberCount(); j++) {

			for (uint32_t k = 0; k < topTribes.at(i)->getMemberID(j)->getVillageCount(); k++) {

				x = std::get<0>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());
				y = std::get<1>(topTribes.at(i)->getMemberID(j)->getVillage(k)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

			}

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topLossTribes.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossTribes.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossTribes.at(i)->getLoss(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);


				gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topLossTribes.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossTribes.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossTribes.at(i)->getLoss(j)->getCoord());

				if (x < 500)
					x = 500 - ((500 - x) * zoom);
				else
					x = 500 + ((x - 500) * zoom);

				if (y < 500)
					y = 500 - ((500 - y) * zoom);
				else
					y = 500 + ((y - 500) * zoom);

				gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topLossTribes[i]->getTag());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topLossTribes.at(i)->getLossPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topLossTribes.at(i)->getLossCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobled Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeLossOut);

	fclose(topTribeLossOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerConqs(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topConqPlayers) {

	gdImagePtr image;
	FILE* topPlayerConqOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topPlayerConq.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topPlayerConq.png";
#endif
	topPlayerConqOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
		gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topConqPlayers.at(i)->getConqCount(); j++) {

			x = std::get<0>(topConqPlayers.at(i)->getConq(j)->getCoord());
			y = std::get<1>(topConqPlayers.at(i)->getConq(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topConqPlayers.at(i)->getConqCount(); j++) {

			x = std::get<0>(topConqPlayers.at(i)->getConq(j)->getCoord());
			y = std::get<1>(topConqPlayers.at(i)->getConq(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topConqPlayers[i]->getName());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topConqPlayers.at(i)->getConqPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topConqPlayers.at(i)->getConqCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobling Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerConqOut);

	fclose(topPlayerConqOut);
	gdImageDestroy(image);

}

/**********************************************************************************************
***********************************************************************************************
**********************************************************************************************/

void drawTopPlayerLosses(std::string world, size_t* colors, size_t zoom, std::deque<player*>& topPlayers, std::deque<player*>& topLossPlayers) {

	gdImagePtr image;
	FILE* topPlayerLossOut;
	std::string filePath = "";
#ifdef _WIN64
	std::string fontPath1 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath1 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath1 += "\\Arena-Condensed-Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

#ifdef _WIN64
	filePath += "maps\\" + world + "\\topPlayerLoss.png";
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	filePath += "/var/www/TWAUMM/maps" + world + "/topPlayerLoss.png";
#endif
	topPlayerLossOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	uint32_t gdColors[15] = { 0 },
		gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	float worldLength = 10.0f / (float)zoom;
	uint32_t worldLengthFloor = std::floor(worldLength);
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	uint32_t x = 0, y = 0;

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 1, y - 1 + 30, x + zoom + 1, y + zoom + 1 + 30, charcoalAlpha);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topPlayers.at(i)->getVillageCount(); j++) {

			x = std::get<0>(topPlayers.at(i)->getVillage(j)->getCoord());
			y = std::get<1>(topPlayers.at(i)->getVillage(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x, y + 30, x + zoom, y + zoom + 30, gdAlphaColors[i]);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topLossPlayers.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossPlayers.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossPlayers.at(i)->getLoss(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);


			gdImageFilledRectangle(image, x - 2, y - 1 + 30, x + zoom + 2, y + zoom + 2 + 30, charcoal);

		}

	}

	for (uint32_t i = 0; i < 15; i++) {

		for (uint32_t j = 0; j < topLossPlayers.at(i)->getLossCount(); j++) {

			x = std::get<0>(topLossPlayers.at(i)->getLoss(j)->getCoord());
			y = std::get<1>(topLossPlayers.at(i)->getLoss(j)->getCoord());

			if (x < 500)
				x = 500 - ((500 - x) * zoom);
			else
				x = 500 + ((x - 500) * zoom);

			if (y < 500)
				y = 500 - ((500 - y) * zoom);
			else
				y = 500 + ((y - 500) * zoom);

			gdImageFilledRectangle(image, x - 1, y + 30, x + zoom + 1, y + zoom + 1 + 30, gdColors[i]);

		}

	}

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, 1000, partialK + i + 30, kNumColor);

	}

	int rect[8];

	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {

		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {

			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath1[0], 10, 0, x, y, &kNum[0]);

		}


	}

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = 1090;
		yWord = 62 + (i * 60);

		xColor = 1015;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topLossPlayers[i]->getName());
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);

		data = FormatWithCommas(topLossPlayers.at(i)->getLossPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

		data = FormatWithCommas(topLossPlayers.at(i)->getLossCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);

	}

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);
	//localtime_s(&timeInfo, &rawTime);
	//asctime_s(&buffer[0], 26, &timeInfo);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobled Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerLossOut);

	fclose(topPlayerLossOut);
	gdImageDestroy(image);

}