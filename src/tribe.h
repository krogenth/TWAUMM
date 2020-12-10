#pragma once
#include <deque>
#include <string>

class player;
class village;

class tribe
{

public:
	tribe();
	~tribe();

	void setID(size_t ID);
	size_t getID() const;

	void setName(std::wstring Name);
	std::wstring getName() const;

	void setTag(std::wstring Tag);
	std::wstring getTag() const;

	void setMemberCount(size_t count);
	size_t getMemberCount() const;

	void setVillageCount(size_t count);
	size_t getVillageCount() const;

	void setTopPoints(size_t count);
	size_t getTopPoints() const;

	void setPoints(size_t count);
	size_t getPoints() const;

	void setRank(size_t rank);
	size_t getRank() const;

	void addMemberID(player* memberID);
	player* getMemberID(size_t index) const;

	void setOD(size_t OD);
	size_t getOD() const;

	void setODA(size_t ODA);
	size_t getODA() const;

	void setODD(size_t ODD);
	size_t getODD() const;

	void setConq(village* village);
	village* getConq(size_t index) const;
	size_t getConqPoints() const;
	size_t getConqCount() const;

	void setLoss(village* village);
	village* getLoss(size_t index) const;
	size_t getLossPoints() const;
	size_t getLossCount() const;

private:
	size_t id = 0;
	std::wstring name = L"";
	std::wstring tag = L"";
	size_t members = 0;
	size_t villages = 0;
	size_t topPoints = 0;
	size_t points = 0;
	size_t rank = 0;
	size_t od = 0;
	size_t oda = 0;
	size_t odd = 0;

	size_t conqPoints = 0;
	size_t lossPoints = 0;

	std::deque<player*> memberIDs;

	std::deque<village*> conqs;
	std::deque<village*> losses;

};