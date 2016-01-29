/*
 * CellHits.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#include "CellHits.h"

std::vector<int> CellHits::Get_CellID() const {
	return CellID;
}
std::vector<int> CellHits::Get_HitCount() const {
	return HitCount;
}
std::vector<std::pair<float, float> > CellHits::Get_CellPosition() const {
	return CellPosition;
}
std::vector<float> CellHits::Get_Position_Radius() const {
	return Position_Radius;
}
std::vector<float> CellHits::Get_Position_Phi() const {
	return Position_Phi;
}
std::map<int, std::pair<std::vector<int>, std::pair<float, float> > > CellHits::Get_AverageOccupancy_Rad() const {
	return AverageOccupancy_Rad;
}
std::map<float, std::pair<std::vector<int>, std::pair<float, float> > > CellHits::Get_AverageOccupancy_Phi() const {
	return AverageOccupancy_Phi;
}
int CellHits::Get_BunchNumber() const {
	return BunchNumber;
}

void CellHits::CheckCellID(int const id, float const x, float const y) {
	bool cell_exists(false);
	int vector_element(-1);
	for (int i = 0; i < CellID.size(); ++i) {
		if (CellID.at(i) == id) {
			cell_exists = true;
			vector_element = i; //Check at which position in vector the ID is stored
			break;
		}
	}
	if (cell_exists) {
		HitCount.at(vector_element) += 1;
	} else {
		CellID.push_back(id);
		HitCount.push_back(1);
		CellPosition.push_back(std::pair<float, float>(x, y));
		Position_Radius.push_back(sqrt(pow(x, 2) + pow(y, 2)));
		Position_Phi.push_back(atan2(y, x));
		//Position_Phi.push_back(acos(x / sqrt(pow(x, 2) + pow(y, 2))));
	}
}
void CellHits::Check_Rad_Position(std::string subdetector_) {
	//std::size_t found = subdetector_.find_last_of("Endcap");

	for (int j = 0; j < Position_Radius.size(); ++j) {
		std::cout << "Position_Radius.at(" << j << ") = " << Position_Radius.at(j) << std::endl;

		if (subdetector_ == "SiVertexEndcap") {
			if (Position_Radius.at(j) < 10)
				AverageOccupancy_Rad[10].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 10 && Position_Radius.at(j) < 20)
				AverageOccupancy_Rad[20].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 20 && Position_Radius.at(j) < 30)
				AverageOccupancy_Rad[30].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 30 && Position_Radius.at(j) < 40)
				AverageOccupancy_Rad[40].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 40 && Position_Radius.at(j) < 50)
				AverageOccupancy_Rad[50].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 50 && Position_Radius.at(j) < 60)
				AverageOccupancy_Rad[60].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 60 && Position_Radius.at(j) < 70)
				AverageOccupancy_Rad[70].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 70 && Position_Radius.at(j) < 80)
				AverageOccupancy_Rad[80].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 80)
				AverageOccupancy_Rad[90].first.push_back(HitCount.at(j));
		} else {
		//if (found == std::string::npos) {
			if (Position_Radius.at(j) < 200)
				AverageOccupancy_Rad[100].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 200 && Position_Radius.at(j) < 400)
				AverageOccupancy_Rad[300].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 400 && Position_Radius.at(j) < 600)
				AverageOccupancy_Rad[500].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 600 && Position_Radius.at(j) < 800)
				AverageOccupancy_Rad[700].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 800 && Position_Radius.at(j) < 1000)
				AverageOccupancy_Rad[900].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 1000 && Position_Radius.at(j) < 1200)
				AverageOccupancy_Rad[1100].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 1200 && Position_Radius.at(j) < 1400)
				AverageOccupancy_Rad[1300].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 1400 && Position_Radius.at(j) < 1600)
				AverageOccupancy_Rad[1500].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 1600 && Position_Radius.at(j) < 1800)
				AverageOccupancy_Rad[1700].first.push_back(HitCount.at(j));
			if (Position_Radius.at(j) >= 1800 && Position_Radius.at(j) < 2000)
				AverageOccupancy_Rad[1900].first.push_back(HitCount.at(j));
		}
	}
	Calculate_Average(AverageOccupancy_Rad);
}
void CellHits::Check_Phi_Position() {
	for (int j = 0; j < Position_Phi.size(); ++j) {
		std::cout << "Position_Phi.at(" << j << ") = " << Position_Phi.at(j) << std::endl;
		if (Position_Phi.at(j) < 0.4)
			AverageOccupancy_Phi[0.2].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 0.4 && Position_Phi.at(j) < 0.8)
			AverageOccupancy_Phi[0.6].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 0.8 && Position_Phi.at(j) < 1.2)
			AverageOccupancy_Phi[1.0].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 1.2 && Position_Phi.at(j) < 1.6)
			AverageOccupancy_Phi[1.4].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 1.6 && Position_Phi.at(j) < 2.0)
			AverageOccupancy_Phi[1.8].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 2.0 && Position_Phi.at(j) < 2.4)
			AverageOccupancy_Phi[2.2].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 2.8 && Position_Phi.at(j) < 3.2)
			AverageOccupancy_Phi[3.0].first.push_back(HitCount.at(j));
	}
	Calculate_Average(AverageOccupancy_Phi);
}

void CellHits::Set_BunchNumber(int const bunchnumber) {
	BunchNumber = bunchnumber;
}
