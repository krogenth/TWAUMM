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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topTribe.png";
	topTribeOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);

	unsigned int gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topTribes[i]->getTag());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topTribes.at(i)->getPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm *timeInfo;
	char *buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topPlayer.png";
	topPlayerOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);

	unsigned int gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topPlayers[i]->getName());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topPlayers.at(i)->getPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topTribeODA.png";
	topTribeOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);

	unsigned int gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODATribes[i]->getTag());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topODATribes.at(i)->getODA());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topODATribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODA Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOdaOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topTribeODD.png";
	topTribeOddOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);

	unsigned int gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODDTribes[i]->getTag());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topODDTribes.at(i)->getODD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topODDTribes.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODD Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOddOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topPlayerODA.png";
	topPlayerOdaOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);

	unsigned int gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODAPlayers[i]->getName());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topODAPlayers.at(i)->getODA());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topODAPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODA Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOdaOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topPlayerODD.png";
	topPlayerOddOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);

	unsigned int gdColors[15] = { 0 };

	for (int count = 0; count < 15; count++)
		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topODDPlayers[i]->getName());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topODDPlayers.at(i)->getODD());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topODDPlayers.at(i)->getVillageCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top ODD Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerOddOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topTribeConq.png";
	topTribeConqOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);
	int charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	unsigned int gdColors[15] = { 0 };
	unsigned int gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topConqTribes[i]->getTag());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topConqTribes.at(i)->getConqPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topConqTribes.at(i)->getConqCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobling Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeConqOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topTribeLoss.png";
	topTribeLossOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);
	int charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	unsigned int gdColors[15] = { 0 };
	unsigned int gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topLossTribes[i]->getTag());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topLossTribes.at(i)->getLossPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topLossTribes.at(i)->getLossCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobled Tribes Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeLossOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topPlayerConq.png";
	topPlayerConqOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);
	int charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	unsigned int gdColors[15] = { 0 };
	unsigned int gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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

		gdImageLine(image, partialK + i, 30, partialK + i, 1030, kNumColor);
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topConqPlayers[i]->getName());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topConqPlayers.at(i)->getConqPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topConqPlayers.at(i)->getConqCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobling Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerConqOut);

	delete timeInfo;
	delete[] buffer;

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
	fontPath1 += "\\Arena Condensed Bold.ttf";

#ifdef _WIN64
	std::string fontPath2 = std::experimental::filesystem::current_path().u8string().c_str();
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	std::string fontPath2 = std::filesystem::current_path().u8string().c_str();
#endif
	fontPath2 += "\\Arena.ttf";

	filePath += "maps\\" + world + "\\topPlayerLoss.png";
	topPlayerLossOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95);
	int bgcolor = gdImageColorAllocate(image, 0, 120, 0);
	int white = gdImageColorAllocate(image, 255, 255, 255);
	int black = gdImageColorAllocate(image, 0, 0, 0);
	int charcoal = gdImageColorAllocate(image, 51, 51, 51);
	int charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	unsigned int gdColors[15] = { 0 };
	unsigned int gdAlphaColors[15] = { 0 };

	for (int count = 0; count < 15; count++) {

		gdColors[count] = gdImageColorAllocate(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2]);
		gdAlphaColors[count] = gdImageColorAllocateAlpha(image, colors[count * 3], colors[count * 3 + 1], colors[count * 3 + 2], 80);

	}

	unsigned int worldLength = 10 / zoom;
	unsigned int kLength = 100 * zoom;

	unsigned int remain = worldLength % 2;

	unsigned int wholeK;
	if (remain == 1)
		wholeK = (worldLength)-1;
	else
		wholeK = worldLength;

	float partialK = ((worldLength - wholeK) * 100 * zoom) / 2;

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
		gdImageLine(image, 0, partialK + i + 30, kLength * worldLength, partialK + i + 30, kNumColor);

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

	for (uint32_t i = 0; i < 15; i++) {

		uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wdata;
		std::string data;

		xWord = (kLength * worldLength) + 90;
		yWord = 62 + (i * 60);

		xColor = (kLength * worldLength) + 15;
		yColor = 55 + (i * 60);

		colorHeight = yColor + 30;
		colorWidth = xColor + 60;

		gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
		gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, gdColors[i]);

		data = converter.to_bytes(topLossPlayers[i]->getName());
		//wdata = topTribes.at(i)->getTag();
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord, &data[0], black);
		//gdImageStringFTEx(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord, (char*)wdata.c_str(), &gdStringExtras);

		data = FormatWithCommas(topLossPlayers.at(i)->getLossPoints());
		data += " points";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 15, &data[0], black);

		data = FormatWithCommas(topLossPlayers.at(i)->getLossCount());
		data += " villages";
		gdImageStringFT(image, &rect[0], black, &fontPath1[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
		//gdImageString16(image, "C:\\Windows\\Fonts\\Arena Condensed Bold.ttf", 10, 0, xWord, yWord + 30, &data[0], black);

		//included for testing purposes
		//data = "zoom: " + std::to_string(zoom);
		//gdImageStringFT(image, &rect[0], white, &fontPath1[0], 14, 0, 500, 20, &data[0]);

	}

	time_t rawTime;
	struct tm* timeInfo;
	char* buffer = new char[26];

	std::time(&rawTime);
	timeInfo = localtime(&rawTime);
	buffer = asctime(timeInfo);

	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 200, 20, "Top Nobled Players Map");
	gdImageStringFT(image, &rect[0], white, &fontPath2[0], 14, 0, 1000, 20, buffer);

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topPlayerLossOut);

	delete timeInfo;
	delete[] buffer;

	fclose(topPlayerLossOut);
	gdImageDestroy(image);

}