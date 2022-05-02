#include "./draw.h"

#include <gd.h>

#include <cinttypes>
#include <cmath>
#include <functional>

#include "../tribe/tribe.h"
#include "../player/player.h"
#include "../village/village.h"
#include "../handleData/formatData.h"
#include "../conversion/stringConversion.h"

void drawVillage(gdImagePtr image, const uint32_t color, const std::tuple<uint32_t, uint32_t>& coords, const uint32_t zoom, const uint32_t offset) {
	uint32_t x = std::get<0>(coords);
	uint32_t y = std::get<1>(coords);

	if (x < 500)
		x = 500 - ((500 - x) * zoom);
	else
		x = 500 + ((x - 500) * zoom);

	if (y < 500)
		y = 500 - ((500 - y) * zoom);
	else
		y = 500 + ((y - 500) * zoom);

	gdImageFilledRectangle(image, x - offset, y - offset + 30, x + zoom + offset, y + zoom + offset + 30, color);
}

void drawPlayerVillages(gdImagePtr image, const uint32_t color, const std::vector<Village*>::const_iterator begin, const std::vector<Village*>::const_iterator end, const uint32_t zoom, uint32_t offset) {
	for (auto iter = begin; iter != end; ++iter) {
		drawVillage(image, color, (*iter)->getCoord(), zoom, offset);
	}
}

void drawPlayerVillages(gdImagePtr image, const uint32_t color, const std::vector<const Village*>::const_iterator begin, const std::vector<const Village*>::const_iterator end, const uint32_t zoom, uint32_t offset) {
	for (auto iter = begin; iter != end; ++iter) {
		drawVillage(image, color, (*iter)->getCoord(), zoom, offset);
	}
}

void drawKontinents(gdImagePtr image, const uint32_t& kNumColor, const float& worldLength, const uint32_t& kLength, const float& partialK) {
	std::string fontPath = "./arialuni.ttf";
	for (uint32_t i = 0; i < kLength * worldLength; i += kLength) {
		gdImageLine(image, (uint32_t)((float)kLength * partialK) + i, 30, (uint32_t)((float)kLength * partialK) + i, 1030, kNumColor);
		gdImageLine(image, 0, (uint32_t)((float)kLength * partialK) + i + 30, 1000, (uint32_t)((float)kLength * partialK) + i + 30, kNumColor);
	}

	int rect[8];
	uint32_t x = 0, y = 0;
	for (uint8_t xLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); xLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); xLine++) {
		for (uint8_t yLine = (uint8_t)std::ceil((10.0f - worldLength - 1.0f) / 2.0f); yLine < 10 - (uint8_t)std::ceil((10.0f - worldLength) / 2.0f); yLine++) {
			std::string kNum = std::to_string(xLine) + std::to_string(yLine);
			x = 82 + ((yLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			y = 128 + ((xLine - (uint8_t)std::floor((10.0f - worldLength) / 2.0f)) * kLength);
			gdImageStringFT(image, &rect[0], kNumColor, &fontPath[0], 10, 0, x, y, &kNum[0]);
		}
	}
}

void drawImageHeader(gdImagePtr image, std::string& world, std::string map) {
	std::string fontPath = "./arialuni.ttf";
	int rect[8];

	int32_t white = gdImageColorAllocate(image, 255, 255, 255);

	time_t rawTime;
	struct tm timeInfo;
	char buffer[26];

	std::time(&rawTime);

#if defined(_WIN64)
	localtime_s(&timeInfo, &rawTime);
	asctime_s(&buffer[0], sizeof(buffer), &timeInfo);
#endif
#if defined (unix) || defined (__unix) || defined (__unix__)
	localtime_r(&rawTime, &timeInfo);
	asctime_r(&timeInfo, &buffer[0]);
#endif

	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 15, 20, &world[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 200, 20, &map[0]);
	gdImageStringFT(image, &rect[0], white, &fontPath[0], 14, 0, 1000, 20, buffer);
}

void drawTopInformation(gdImagePtr image, const uint32_t color, const std::wstring& name, size_t points, size_t villages, uint32_t index) {
	std::string fontPath = "./arialuni.ttf";

	gdFTStringExtra gdStringExtras;
	gdStringExtras.flags = gdFTEX_LINESPACE | gdFTEX_CHARMAP;
	gdStringExtras.linespacing = 1.05;
	gdStringExtras.charmap = gdFTEX_Unicode;
	gdStringExtras.hdpi = 96;
	gdStringExtras.vdpi = 96;
	gdStringExtras.xshow = nullptr;
	gdStringExtras.fontpath = nullptr;
	int rect[8];
	int32_t black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	uint32_t xWord, yWord, xColor, yColor, colorHeight, colorWidth;
	std::string data;

	xWord = 1090;
	yWord = 62 + (index * 60);
	xColor = 1015;
	yColor = 55 + (index * 60);
	colorHeight = yColor + 30;
	colorWidth = xColor + 60;

	gdImageFilledRectangle(image, xColor - 1, yColor - 1, colorWidth + 1, colorHeight + 1, charcoal);
	gdImageFilledRectangle(image, xColor, yColor, colorWidth, colorHeight, color);

	data = wide_string_to_string(name);
	gdImageStringFTEx(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord, &data[0], &gdStringExtras);

	data = FormatWithCommas(points) + " points";
	gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 15, (char*)&data[0]);

	data = FormatWithCommas<size_t>(villages) + " villages";
	gdImageStringFT(image, &rect[0], black, &fontPath[0], 10, 0, xWord, yWord + 30, (char*)&data[0]);
}

/****************************************************************************
			DRAW TRIBE FUNCTIONS
****************************************************************************/

void drawTopTribes(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topTribes) {
	gdImagePtr image;
	FILE* topTribeOut;
	std::string filePath = "./maps/" + world + "/topTribe.png";

	topTribeOut = fopen(filePath.c_str(), "wb");

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [tribe, end, i] = std::tuple{ topTribes.cbegin(), topTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoal, (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 1);
			}
		}
	}

	for (auto [tribe, end, i] = std::tuple{ topTribes.cbegin(), topTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdColors.at(i), (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 0);
			}
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [tribe, end, i] = std::tuple{ topTribes.cbegin(), topTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			drawTopInformation(image, gdColors.at(i), (*tribe)->getTag(), (*tribe)->getPoints(), (*tribe)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top Tribes Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	fwrite(data, sizeof(char), size, topTribeOut);

	fclose(topTribeOut);
	gdImageDestroy(image);
}

void drawTopTribeOD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topODTribes) {
	gdImagePtr image;
	
	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [tribe, end, i] = std::tuple{ topODTribes.cbegin(), topODTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoal, (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 1);
			}
		}
	}

	for (auto [tribe, end, i] = std::tuple{ topODTribes.cbegin(), topODTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdColors.at(i), (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 0);
			}
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [tribe, end, i] = std::tuple{ topODTribes.cbegin(), topODTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			drawTopInformation(image, gdColors.at(i), (*tribe)->getTag(), (*tribe)->getPoints(), (*tribe)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top OD Tribes Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topTribeOD.png";
	FILE* topODTribeOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topODTribeOut);

	fclose(topODTribeOut);
	gdImageDestroy(image);
}

void drawTopTribeODA(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topODATribes) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [tribe, end, i] = std::tuple{ topODATribes.cbegin(), topODATribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoal, (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 1);
			}
		}
	}

	for (auto [tribe, end, i] = std::tuple{ topODATribes.cbegin(), topODATribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdColors.at(i), (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 0);
			}
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [tribe, end, i] = std::tuple{ topODATribes.cbegin(), topODATribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			drawTopInformation(image, gdColors.at(i), (*tribe)->getTag(), (*tribe)->getPoints(), (*tribe)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top ODA Tribes Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topTribeODA.png";
	FILE* topODATribeOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topODATribeOut);

	fclose(topODATribeOut);
	gdImageDestroy(image);
}

void drawTopTribeODD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topODDTribes) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [tribe, end, i] = std::tuple{ topODDTribes.cbegin(), topODDTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoal, (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 1);
			}
		}
	}

	for (auto [tribe, end, i] = std::tuple{ topODDTribes.cbegin(), topODDTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdColors.at(i), (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 0);
			}
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [tribe, end, i] = std::tuple{ topODDTribes.cbegin(), topODDTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			drawTopInformation(image, gdColors.at(i), (*tribe)->getTag(), (*tribe)->getPoints(), (*tribe)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top ODD Tribes Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topTribeODD.png";
	FILE* topODDTribeOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topODDTribeOut);

	fclose(topODDTribeOut);
	gdImageDestroy(image);
}

void drawTopTribeConqs(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topConqTribes) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	std::vector<uint32_t> gdColors;
	std::vector<uint32_t> gdAlphaColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
		gdAlphaColors.emplace_back(gdImageColorAllocateAlpha(image, std::get<0>(color), std::get<1>(color), std::get<2>(color), 80));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [tribe, end, i] = std::tuple{ topConqTribes.cbegin(), topConqTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoalAlpha, (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 1);
			}
		}
	}
	for (auto [tribe, end, i] = std::tuple{ topConqTribes.cbegin(), topConqTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdAlphaColors.at(i), (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 0);
			}
		}
	}

	for (auto [tribe, end, i] = std::tuple{ topConqTribes.cbegin(), topConqTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoal, (*iter)->conqCBegin(), (*iter)->conqCEnd(), zoom, 2);
			}
		}
	}
	for (auto [tribe, end, i] = std::tuple{ topConqTribes.cbegin(), topConqTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdColors.at(i), (*iter)->conqCBegin(), (*iter)->conqCEnd(), zoom, 1);
			}
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [tribe, end, i] = std::tuple{ topConqTribes.cbegin(), topConqTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			drawTopInformation(image, gdColors.at(i), (*tribe)->getTag(), (*tribe)->getConqPoints(), (*tribe)->getConqCount(), i);
		}
	}
	drawImageHeader(image, world, "Top Nobling Tribes Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topTribeConq.png";
	FILE* topTribeConqOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topTribeConqOut);

	fclose(topTribeConqOut);
	gdImageDestroy(image);
}

void drawTopTribeLosses(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Tribe*>& topLossTribes) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	std::vector<uint32_t> gdColors;
	std::vector<uint32_t> gdAlphaColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
		gdAlphaColors.emplace_back(gdImageColorAllocateAlpha(image, std::get<0>(color), std::get<1>(color), std::get<2>(color), 80));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [tribe, end, i] = std::tuple{ topLossTribes.cbegin(), topLossTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoalAlpha, (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 1);
			}
		}
	}
	for (auto [tribe, end, i] = std::tuple{ topLossTribes.cbegin(), topLossTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdAlphaColors.at(i), (*iter)->villageCBegin(), (*iter)->villageCEnd(), zoom, 0);
			}
		}
	}

	for (auto [tribe, end, i] = std::tuple{ topLossTribes.cbegin(), topLossTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, charcoal, (*iter)->conqCBegin(), (*iter)->conqCEnd(), zoom, 2);
			}
		}
	}
	for (auto [tribe, end, i] = std::tuple{ topLossTribes.cbegin(), topLossTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			for (auto iter = (*tribe)->memberCBegin(); iter != (*tribe)->memberCEnd(); ++iter) {
				drawPlayerVillages(image, gdColors.at(i), (*iter)->conqCBegin(), (*iter)->conqCEnd(), zoom, 1);
			}
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [tribe, end, i] = std::tuple{ topLossTribes.cbegin(), topLossTribes.cend(), 0 }; tribe != end; ++tribe, ++i) {
		if (*tribe) {
			drawTopInformation(image, gdColors.at(i), (*tribe)->getTag(), (*tribe)->getConqPoints(), (*tribe)->getConqCount(), i);
		}
	}
	drawImageHeader(image, world, "Top Nobled Tribes Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topTribeLoss.png";
	FILE* topTribeLossOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topTribeLossOut);

	fclose(topTribeLossOut);
	gdImageDestroy(image);
}

/****************************************************************************
			DRAW PLAYER FUNCTIONS
****************************************************************************/

void drawTopPlayers(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topPlayers) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [player, end, i] = std::tuple{ topPlayers.cbegin(), topPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoal, (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 1);
			drawPlayerVillages(image, gdColors.at(i), (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 0);
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [player, end, i] = std::tuple{ topPlayers.cbegin(), topPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawTopInformation(image, gdColors.at(i), (*player)->getName(), (*player)->getPoints(), (*player)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top Players Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topPlayer.png";
	FILE* topPlayerOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topPlayerOut);

	fclose(topPlayerOut);
	gdImageDestroy(image);
}

void drawTopPlayerOD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topODPlayers) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [player, end, i] = std::tuple{ topODPlayers.cbegin(), topODPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoal, (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 1);
			drawPlayerVillages(image, gdColors.at(i), (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 0);
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [player, end, i] = std::tuple{ topODPlayers.cbegin(), topODPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawTopInformation(image, gdColors.at(i), (*player)->getName(), (*player)->getPoints(), (*player)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top OD Players Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topPlayerOD.png";
	FILE* topODPlayerOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topODPlayerOut);

	fclose(topODPlayerOut);
	gdImageDestroy(image);
}

void drawTopPlayerODA(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topODAPlayers) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [player, end, i] = std::tuple{ topODAPlayers.cbegin(), topODAPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoal, (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 1);
			drawPlayerVillages(image, gdColors.at(i), (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 0);
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [player, end, i] = std::tuple{ topODAPlayers.cbegin(), topODAPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawTopInformation(image, gdColors.at(i), (*player)->getName(), (*player)->getPoints(), (*player)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top ODA Players Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topPlayerODA.png";
	FILE* topODAPlayerOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topODAPlayerOut);

	fclose(topODAPlayerOut);
	gdImageDestroy(image);
}

void drawTopPlayerODD(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topODDPlayers) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51);

	std::vector<uint32_t> gdColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [player, end, i] = std::tuple{ topODDPlayers.cbegin(), topODDPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoal, (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 1);
			drawPlayerVillages(image, gdColors.at(i), (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 0);
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [player, end, i] = std::tuple{ topODDPlayers.cbegin(), topODDPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawTopInformation(image, gdColors.at(i), (*player)->getName(), (*player)->getPoints(), (*player)->getVillageCount(), i);
		}
	}
	drawImageHeader(image, world, "Top ODD Players Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topPlayerODD.png";
	FILE* topODDPlayerOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topODDPlayerOut);

	fclose(topODDPlayerOut);
	gdImageDestroy(image);
}

void drawTopPlayerConqs(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topConqPlayers) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	std::vector<uint32_t> gdColors;
	std::vector<uint32_t> gdAlphaColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
		gdAlphaColors.emplace_back(gdImageColorAllocateAlpha(image, std::get<0>(color), std::get<1>(color), std::get<2>(color), 80));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [player, end, i] = std::tuple{ topConqPlayers.cbegin(), topConqPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoalAlpha, (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 1);
			drawPlayerVillages(image, gdAlphaColors.at(i), (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 0);
		}
	}

	for (auto [player, end, i] = std::tuple{ topConqPlayers.cbegin(), topConqPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoal, (*player)->conqCBegin(), (*player)->conqCEnd(), zoom, 2);
			drawPlayerVillages(image, gdColors.at(i), (*player)->conqCBegin(), (*player)->conqCEnd(), zoom, 1);
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [player, end, i] = std::tuple{ topConqPlayers.cbegin(), topConqPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawTopInformation(image, gdColors.at(i), (*player)->getName(), (*player)->getConqPoints(), (*player)->getConqCount(), i);
		}
	}
	drawImageHeader(image, world, "Top Nobling Players Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topPlayerConq.png";
	FILE* topPlayerConqOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topPlayerConqOut);

	fclose(topPlayerConqOut);
	gdImageDestroy(image);
}

void drawTopPlayerLosses(const std::string& outdir, std::string world, const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& colors, const uint32_t zoom, std::deque<Player*>& topLossPlayers) {
	gdImagePtr image;

	image = gdImageCreateTrueColor(1250, 1030);
	int32_t kNumColor = gdImageColorAllocateAlpha(image, 255, 255, 255, 95),
		bgcolor = gdImageColorAllocate(image, 0, 120, 0),
		white = gdImageColorAllocate(image, 255, 255, 255),
		black = gdImageColorAllocate(image, 0, 0, 0),
		charcoal = gdImageColorAllocate(image, 51, 51, 51),
		charcoalAlpha = gdImageColorAllocateAlpha(image, 51, 51, 51, 50);

	std::vector<uint32_t> gdColors;
	std::vector<uint32_t> gdAlphaColors;
	for (const auto& color : colors) {
		gdColors.emplace_back(gdImageColorAllocate(image, std::get<0>(color), std::get<1>(color), std::get<2>(color)));
		gdAlphaColors.emplace_back(gdImageColorAllocateAlpha(image, std::get<0>(color), std::get<1>(color), std::get<2>(color), 80));
	}

	float worldLength = 10.0f / static_cast<float>(zoom);
	uint32_t worldLengthFloor = static_cast<uint32_t>(std::floor(worldLength));
	uint32_t kLength = 100 * zoom;
	uint32_t wholeK = worldLengthFloor - (worldLengthFloor % 2);
	float partialK = (worldLength - (float)wholeK) / 2.0f;

	gdImageFilledRectangle(image, 0, 30, 1000, 1030, bgcolor);
	gdImageFilledRectangle(image, 1000, 30, 1250, 1030, white);

	for (auto [player, end, i] = std::tuple{ topLossPlayers.cbegin(), topLossPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoalAlpha, (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 1);
			drawPlayerVillages(image, gdAlphaColors.at(i), (*player)->villageCBegin(), (*player)->villageCEnd(), zoom, 0);
		}
	}

	for (auto [player, end, i] = std::tuple{ topLossPlayers.cbegin(), topLossPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawPlayerVillages(image, charcoal, (*player)->lossCBegin(), (*player)->lossCEnd(), zoom, 2);
			drawPlayerVillages(image, gdColors.at(i), (*player)->lossCBegin(), (*player)->lossCEnd(), zoom, 1);
		}
	}

	drawKontinents(image, kNumColor, worldLength, kLength, partialK);
	for (auto [player, end, i] = std::tuple{ topLossPlayers.cbegin(), topLossPlayers.cend(), 0 }; player != end; ++player, ++i) {
		if (*player) {
			drawTopInformation(image, gdColors.at(i), (*player)->getName(), (*player)->getLossPoints(), (*player)->getLossCount(), i);
		}
	}
	drawImageHeader(image, world, "Top Nobled Players Map");

	int size = 0;
	char* data = (char*)gdImagePngPtr(image, &size);
	std::string filePath = "./" + outdir + "/" + world + "/topPlayerLoss.png";
	FILE* topPlayerLossOut = fopen(filePath.c_str(), "wb");
	fwrite(data, sizeof(char), size, topPlayerLossOut);

	fclose(topPlayerLossOut);
	gdImageDestroy(image);
}