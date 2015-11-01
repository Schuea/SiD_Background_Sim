/*
 * CaloIDs.h
 *
 *  Created on: Sep 9, 2015
 *      Author: Anne Schuetz
 */

#ifndef CALOIDS_H_
#define CALOIDS_H_

#include <bitset>
#include <array>

class CaloIDs {
public:
	CaloIDs();
	CaloIDs(std::string SubDetector);
	virtual ~CaloIDs();

	std::string GetName();

	void GetCellID(int ID0, int ID1);

	int GetLayer(int ID0);
	int GetLayer(long CellID);

	double* GetCellPos(int ID0);

	std::bitset<32> IntToBitsetConversion(int ID);
	std::bitset<64> LongToBitsetConversion(long ID);

	std::bitset<64> CellID;
	std::bitset<6> LayerID6;
	std::bitset<8> LayerID8;
	int Number_layers = 0;

	std::array<float, 3> getBinningEnergy1D() const {
		return binningEnergy1D;
	}
	std::array<int, 3> getBinning1D() const {
		return binning1D;
	}
	std::array<int, 6> getBinning2D() const {
		return binning2D;
	}
	std::array<int, 9> getBinning3D() const {
		return binning3D;
	}

protected:
	std::array<float, 3> binningEnergy1D;
	std::array<int, 3> binning1D;
	std::array<int, 6> binning2D;
	std::array<int, 9> binning3D;

private:
	std::string subdetector;

	template<size_t sRes, size_t s1, size_t s2>
	std::bitset<sRes> concatString(const std::bitset<s1>& bs1,
			const std::bitset<s2>& bs2);

	int StartBin_layers = 0;
	int LengthBin_layers = 0;
};

#endif /* CALOIDS_H_ */
