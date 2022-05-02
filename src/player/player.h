#pragma once
#include <vector>
#include <string>

class Tribe;
class Village;

class Player {
public:
	Player() {}
	~Player() {}

	void virtual setID(size_t id) { this->id = id; }
	size_t virtual getID() const { return this->id; }

	void setName(std::wstring name) { this->name = name; }
	std::wstring getName() const { return this->name; }

	void setTribe(Tribe* tribe);
	Tribe* getTribe() const { return this->tribe; }

	void setPoints(size_t points) { this->points = points; }
	size_t getPoints() const { return this->points; }

	void setRank(size_t rank) { this->rank = rank; }
	size_t getRank() const { return this->rank; }

	size_t getVillageCount() { return this->villages.size(); }

	void addVillage(Village* village) { this->villages.emplace_back(village); }
	Village* getVillage(size_t index) const { return this->villages.at(index); }
	std::vector<Village*>::const_iterator villageCBegin() const { return this->villages.cbegin(); }
	std::vector<Village*>::const_iterator villageCEnd() const { return this->villages.cend(); }

	void setOD(size_t od) { this->od = od; }
	size_t getOD() const { return this->od; }

	void setODA(size_t oda) { this->oda = oda; }
	size_t getODA() const { return this->oda; }

	void setODD(size_t odd) { this->odd = odd; }
	size_t getODD() const { return this->odd; }

	void setConq(const Village* village);
	const Village* getConq(size_t index) const { return this->conqs.at(index); }
	size_t getConqPoints() const { return this->conqPoints; }
	size_t getConqCount() const { return this->conqs.size(); }
	std::vector<const Village*>::const_iterator conqCBegin() const { return this->conqs.cbegin(); }
	std::vector<const Village*>::const_iterator conqCEnd() const { return this->conqs.cend(); }

	void setLoss(const Village* village);
	const Village* getLoss(size_t index) const { return this->losses.at(index); }
	size_t getLossPoints() const { return this->lossPoints; }
	size_t getLossCount() const { return this->losses.size(); }
	std::vector<const Village*>::const_iterator lossCBegin() const { return this->losses.cbegin(); }
	std::vector<const Village*>::const_iterator lossCEnd() const { return this->losses.cend(); }

private:
	size_t id = 0;
	std::wstring name = L"";
	Tribe* tribe = nullptr;
	size_t points = 0;
	size_t rank = 0;
	size_t od = 0;
	size_t oda = 0;
	size_t odd = 0;

	size_t conqPoints = 0;
	size_t lossPoints = 0;

	std::vector<Village*> villages;
	std::vector<const Village*> conqs;
	std::vector<const Village*> losses;
};