#pragma once
#include <vector>
#include <string>

#include "../village/village.h"

class Player;

class Tribe {
public:
	Tribe() {}
	~Tribe() {}

	void virtual setID(size_t id) { this->id = id; }
	size_t virtual getID() const { return this->id; }

	void setName(std::wstring name) { this->name = name; }
	const std::wstring& getName() const { return this->name; }

	void setTag(std::wstring tag) { this->tag = tag; }
	const std::wstring& getTag() const { return this->tag; }

	void setMemberCount(size_t count) { this->memberCount = count; }
	const size_t getMemberCount() const { return this->memberCount; }

	void setVillageCount(size_t count) { this->villages = count; }
	const size_t getVillageCount() const { return this->villages; }

	void setTopPoints(size_t points) { this->topPoints = points; }
	const size_t getTopPoints() const { return this->topPoints; }

	void setPoints(size_t points) { this->points = points; }
	const size_t getPoints() const { return this->points; }

	void setRank(size_t rank) { this->rank = rank; }
	const size_t getRank() const { return this->rank; }

	void addMember(Player* player) { this->members.emplace_back(player); }
	Player* getMember(size_t index) const { return this->members.at(index); }
	std::vector<Player*>::const_iterator memberCBegin() const { return this->members.cbegin(); }
	std::vector<Player*>::const_iterator memberCEnd() const { return this->members.cend(); }

	void setOD(size_t od) { this->od = od; }
	size_t getOD() const { return this->od; }

	void setODA(size_t oda) { this->oda = oda; }
	size_t getODA() const { return this->oda; }

	void setODD(size_t odd) { this->odd = odd; }
	size_t getODD() const { return this->odd; }

	void setConq(const Village* village) { this->conqPoints += village->getPoints(); this->conqs.emplace_back(village); }
	const Village* getConq(size_t index) const { return this->conqs.at(index); }
	const size_t getConqPoints() const { return this->conqPoints; }
	const size_t getConqCount() const { return this->conqs.size(); }
	std::vector<const Village*>::const_iterator conqCBegin() const { return this->conqs.cbegin(); }
	std::vector<const Village*>::const_iterator conqCEnd() const { return this->conqs.cend(); }

	void setLoss(const Village* village) { this->lossPoints += village->getPoints(); this->losses.emplace_back(village); }
	const Village* getLoss(size_t index) const { return this->losses.at(index); }
	const size_t getLossPoints() const { return this->lossPoints; }
	const size_t getLossCount() const { return this->losses.size(); }
	std::vector<const Village*>::const_iterator lossCBegin() const { return this->losses.cbegin(); }
	std::vector<const Village*>::const_iterator lossCEnd() const { return this->losses.cend(); }

private:
	size_t id = 0;
	std::wstring name = L"";
	std::wstring tag = L"";
	size_t memberCount = 0;
	size_t villages = 0;
	size_t topPoints = 0;
	size_t points = 0;
	size_t rank = 0;
	size_t od = 0;
	size_t oda = 0;
	size_t odd = 0;

	size_t conqPoints = 0;
	size_t lossPoints = 0;

	std::vector<Player*> members;
	std::vector<const Village*> conqs;
	std::vector<const Village*> losses;

};