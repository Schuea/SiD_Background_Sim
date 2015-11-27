#include "CreateCellID.h"

template<size_t sRes, size_t s1, size_t s2>
std::bitset<sRes> concatString(const std::bitset<s1>& bs1,
								const std::bitset<s2>& bs2){
				std::string string1;
				string1 = bs1.to_string();
				std::string string2;
				string2 = bs2.to_string();

				std::bitset<sRes> res(string1 + string2);
				return res;
}

void CellID::CreateCellID() {

				if ((ID0_ == 0 && ID1_ == 0) || ID0_ < 0 || ID1_ <0){
								throw std::runtime_error("The CellID was not set. Invalid IDs!");
				}				
				else{
								std::bitset<32> ID0_bit;
								std::bitset<32> ID1_bit;
								ID0_bit = std::bitset<std::numeric_limits<unsigned int>::digits>(ID0_);
								ID1_bit = std::bitset<std::numeric_limits<unsigned int>::digits>(ID1_);

								CellID_ = concatString<64, 32, 32>(ID1_bit, ID0_bit);
				}
}

