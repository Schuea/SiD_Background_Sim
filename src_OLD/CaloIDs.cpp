/*
 * CaloIDs.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: Anne Schuetz
 *     Version: v01.00
 * Description: C++ project for decoding the detector specific geometrical
 *  			cell IDs of SimCalorimeterHit collections
 */

#include "CaloIDs.h"

#include <iostream>
#include <string>
#include <bitset>
#include <limits>

CaloIDs::CaloIDs() {
}

CaloIDs::CaloIDs(std::string SubDetector) :
		CaloIDs() {

	subdetector = SubDetector;

	if (subdetector == std::string("EcalBarrel")) {
		StartBin_layers = 13;
		LengthBin_layers = 6;
		Number_layers = 31;

		binningEnergy1D = {40., 0., 0.01};
		binning1D = {10, 0, 10};
		binning2D = {70, -1500, 1500, 70, -1500, 1500};
		binning3D = {100, -2000, 2000, 100, -1500, 1500, 100, -1500, 1500};
	}
	else if (subdetector == std::string("EcalEndcap")) {
		StartBin_layers = 13;
		LengthBin_layers = 6;
		Number_layers = 31;

		binningEnergy1D = {40, 0, 0.01};
		binning1D = {15, 0, 15};
		binning2D = {70, -1500, 1500, 70, -1500, 1500};
		binning3D = {70, -2000, 2000, 70, -1500, 1500, 70, -1500, 1500};
	}
	else if (subdetector == std::string("HcalBarrel")) {
		StartBin_layers = 13;
		LengthBin_layers = 6;
		Number_layers = 40;

		binningEnergy1D = {40, 0, 0.00002};
		binning1D = {6, 0, 6};
		binning2D = {70, -2500, 2500, 70, -2500, 2500};
		binning3D = {100, -3100, 3100, 70, -2500, 2500, 70, -2500, 2500};
	}
	else if (subdetector == std::string("HcalEndcap")) {
		StartBin_layers = 13;
		LengthBin_layers = 8;
		Number_layers = 45;

		binningEnergy1D = {250, 0, 0.00045};
		binning1D = {50, 0, 50};
		binning2D = {70, -1500, 1500, 70, -1500, 1500};
		binning3D = {100, -3100, 3100, 70, -1500, 1500, 70, -1500, 1500};
	}
	else if (subdetector == std::string("MuonBarrel")) {
		StartBin_layers = 15;
		LengthBin_layers = 6;
		Number_layers = 11;

		binningEnergy1D = {25, 0, 0.00003};
		binning1D = {10, 0, 10};
		binning2D = {150, -6300, 6300, 150, -6300, 6300};
		binning3D = {100, -3100, 3100, 150, -6300, 6300, 150, -6300, 6300};
	}
	else if (subdetector == std::string("MuonEndcap")) {
		StartBin_layers = 15;
		LengthBin_layers = 6;
		Number_layers = 11;

		binningEnergy1D = {35, 0, 0.0075};
		binning1D = {50, 0, 15000};
		binning2D = {150, -6300, 6300, 150, -6300, 6300};
		binning3D = {150, -5700, 5700, 150, -6300, 6300, 150, -6300, 6300};
	}
	else if (subdetector == std::string("LumiCal")) {
		StartBin_layers = 8;
		LengthBin_layers = 8;
		Number_layers = 30;

		binningEnergy1D = {40, 0, 0.045};
		binning1D = {100, 0, 210};
		binning2D = {100, -230, 230, 100, -230, 230};
		binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
	}
	else if (subdetector == std::string("BeamCal")) {
		StartBin_layers = 8;
		LengthBin_layers = 8;
		Number_layers = 50;

		binningEnergy1D = {45, 0, 2.5};
		binning1D = {150, 0, 2000000};
		binning2D = {100, -140, 140, 100, -140, 104};
		binning3D = {150, -3200, 3200, 100, -140, 140, 100, -140, 140};
	}

	else if (subdetector == std::string("SiVertexEncap")) {
		//Number_layers = ;
	}
	else if (subdetector == std::string("SiVertexBarrel")) {
		//Number_layers = ;
	}
	else if (subdetector == std::string("SiTrackerForward")) {
		//Number_layers = ;
	}
	else if (subdetector == std::string("SiTrackerEncap")) {
		//Number_layers = ;
	}
	else if (subdetector == std::string("SiTrackerBarrel")) {
		//Number_layers = ;
	}
	else {
		throw std::exception();
	}

}

CaloIDs::~CaloIDs() {
	// TODO Auto-generated destructor stub
}

std::string CaloIDs::GetName() {
	return subdetector;
}

void CaloIDs::GetCellID(int ID0, int ID1) {

	std::bitset<32> ID0_bit;
	std::bitset<32> ID1_bit;
	ID0_bit = IntToBitsetConversion(ID0);
	/*std::cout << "ID0_bit = ";
	 for(int n=31; n>=0; --n){
	 std::cout << ID0_bit[n];
	 }
	 std::cout<<std::endl;*/
	ID1_bit = IntToBitsetConversion(ID1);
	/*std::cout << "ID1_bit = ";
	 for(int n=31; n>=0; --n){
	 std::cout << ID1_bit[n];
	 }
	 std::cout<<std::endl;*/

	CellID = concatString<64, 32, 32>(ID1_bit, ID0_bit);
}

template<size_t sRes, size_t s1, size_t s2>
std::bitset<sRes> CaloIDs::concatString(const std::bitset<s1>& bs1,
		const std::bitset<s2>& bs2) {
	std::string string1;
	string1 = bs1.to_string();
	//std::cout << "string1 = " << string1 << std::endl;
	std::string string2;
	string2 = bs2.to_string();
	//std::cout << "string2 = " << string2 << std::endl;

	std::bitset<sRes> res(string1 + string2);
	return res;
}

int CaloIDs::GetLayer(int ID0) {

	std::bitset<32> ID0_bit;
	ID0_bit = IntToBitsetConversion(ID0);

	for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers;
			++i) {
		if (LengthBin_layers == 6)
			LayerID6[i - StartBin_layers] = ID0_bit[i];
		if (LengthBin_layers == 8)
			LayerID8[i - StartBin_layers] = ID0_bit[i];
	}

	//std::bitset<6> which_one (std::string("111111"));
	//std::bitset<6> Layer = Layer_bit & which_one;
	//int Layer = (ID0_bit >> 13)&which_one;
	//int Layer = (ID0_bit >> 13)&0x3f;
	int Layer = 0;
	if (LengthBin_layers == 6)
		Layer = LayerID6.to_ulong();
	if (LengthBin_layers == 8)
		Layer = LayerID6.to_ulong();

	//std::cout << "Layer_bit = " << Layer_bit << std::endl;
	//std::cout << "Layer_bit & which_one = " << Layer << std::endl;
	//std::cout << "Layer = " << Layer << std::endl;
	//std::cout << "Layer (ID0) = " << Layer << std::endl;
	return Layer;
}

int CaloIDs::GetLayer(long CellID){
	std::bitset<64> ID_bit;
		ID_bit = LongToBitsetConversion(CellID);

		for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers;
				++i) {
			if (LengthBin_layers == 6)
				LayerID6[i - StartBin_layers] = ID_bit[i];
			if (LengthBin_layers == 8)
				LayerID8[i - StartBin_layers] = ID_bit[i];
		}

		//std::bitset<6> which_one (std::string("111111"));
		//std::bitset<6> Layer = Layer_bit & which_one;
		//int Layer = (ID0_bit >> 13)&which_one;
		//int Layer = (ID0_bit >> 13)&0x3f;
		int Layer = 0;
		if (LengthBin_layers == 6)
			Layer = LayerID6.to_ulong();
		if (LengthBin_layers == 8)
			Layer = LayerID6.to_ulong();

		//std::cout << "Layer_bit = " << Layer_bit << std::endl;
		//std::cout << "Layer_bit & which_one = " << Layer << std::endl;
		//std::cout << "Layer = " << Layer << std::endl;
		//std::cout << "Layer (CellID) = " << Layer << std::endl;
		return Layer;
}

double* CaloIDs::GetCellPos(int ID1) {

	std::bitset<32> ID1_bit;
	ID1_bit = IntToBitsetConversion(ID1);
	std::bitset<16> posx_bit;
	std::bitset<16> posy_bit;
	for (int n = 0; n < 32; ++n) {
		if (n <= 15)
			posx_bit[n] = ID1_bit[n];
		if (n >= 16)
			posy_bit[n - 16] = ID1_bit[n];
	}
	/*std::cout << "posx_bit = ";
	 for(int n=15; n>=0; --n){
	 std::cout << posx_bit[n];
	 }
	 std::cout<<std::endl;*/
	/*std::cout << "posy_bit = ";
	 for(int n=15; n>=0; --n){
	 std::cout << posy_bit[n];
	 }
	 std::cout<<std::endl;*/

	double *pos = new double[2];
	pos[0] = posx_bit.to_ulong();
	pos[1] = posy_bit.to_ulong();

	return pos;
	delete[] pos;
}

double* CaloIDs::GetCellPos(long CellID) {

	std::bitset<64> ID_bit;
	ID_bit = LongToBitsetConversion(CellID);
	std::bitset<16> posx_bit;
	std::bitset<16> posy_bit;
	for (int n = 32; n < 64; ++n) {
		if (n <= 32 + 15)
			posx_bit[n - 32] = ID_bit[n];
		if (n >= 32 + 16)
			posy_bit[n - (32 + 16)] = ID_bit[n];
	}

	double *pos = new double[2];
	pos[0] = posx_bit.to_ulong();
	pos[1] = posy_bit.to_ulong();

	return pos;
	delete[] pos;
}

std::bitset<32> CaloIDs::IntToBitsetConversion(int ID) {

	std::bitset<32> ID_bit;
	ID_bit = std::bitset<std::numeric_limits<unsigned int>::digits>(ID);

	return ID_bit;
}

std::bitset<64> CaloIDs::LongToBitsetConversion(long ID) {

	std::bitset<64> ID_bit;
	ID_bit = std::bitset<std::numeric_limits<unsigned long>::digits>(ID);

	return ID_bit;
}
