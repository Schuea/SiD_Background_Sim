/*
 * CellHits.h
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#ifndef CELLHITS_H_
#define CELLHITS_H_

#include <iostream>

#include <vector>
#include <map>
#include <cmath>

class CellHits {
public:
	CellHits(): CellID(), HitCount(),
		CellPosition(), Position_Radius(), Position_Phi(),
		AverageOccupancy_Rad(), AverageOccupancy_Phi(), BunchNumber(0){}
	~CellHits(){}

  std::vector< int > Get_CellID() const;
  std::vector< int > Get_HitCount() const;
  std::vector< std::pair< float, float > > Get_CellPosition() const;
  std::vector< float > Get_Position_Radius() const;
  std::vector< float > Get_Position_Phi() const;
  std::map< int, std::pair< std::vector< int >, float > > Get_AverageOccupancy_Rad() const;
  std::map< float, std::pair< std::vector< int >, float > > Get_AverageOccupancy_Phi() const;
  int Get_BunchNumber() const;

  void CheckCellID(int const id, float const x, float const y);
  void Check_Rad_Position();
  void Check_Phi_Position();
  template< class T >
  void Calculate_Average( std::map< T, std::pair< std::vector< int >, float > > & AverageMap){
  	for (auto iterator = AverageMap.begin(); iterator != AverageMap.end(); iterator++) {
      std::cout << "AverageMap.iterator.first = " << iterator->first << std::endl;
  		float average = 0;
  		for (int i = 0; i < iterator->second.first.size(); ++i) {
  			average += iterator->second.first.at(i);
  		}
  		average /= float(iterator->second.first.size());
      std::cout << "Average = " << average << std::endl;
  		iterator->second.second = average;
  	}
  }
  void Set_BunchNumber(int const bunchnumber);

protected:
  std::vector< int > CellID;
  std::vector< int > HitCount;
  std::vector< std::pair< float, float > > CellPosition;
  std::vector< float > Position_Radius;
  std::vector< float > Position_Phi;
  std::map< int, std::pair< std::vector< int >, float > > AverageOccupancy_Rad;
  std::map< float, std::pair< std::vector< int >, float > > AverageOccupancy_Phi;
  int BunchNumber;
};

#endif /* CELLHITS_H_ */
