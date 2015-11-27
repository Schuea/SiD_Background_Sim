#include <bitset>
#include <string>
#include <limits>

#include "CreateCellID.h"
#include "CreateCellIDTest.h"

#include "gtest/gtest.h"

typedef CreateCellIDTest CreateCellIDDeathTest;

TEST_F(CreateCellIDTest, GetCellID){
				CellID testclass1(1,2);
				testclass1.CreateCellID();
				EXPECT_NO_THROW(testclass1.CreateCellID());
				std::bitset<64> expected_bitset1 (std::string("0000000000000000000000000000001000000000000000000000000000000001"));
				EXPECT_TRUE(expected_bitset1 == testclass1.GetCellID());

				CellID testclass2(1,0);
				testclass2.CreateCellID();
				EXPECT_NO_THROW(testclass2.CreateCellID());
				std::bitset<64> expected_bitset2 (std::string("0000000000000000000000000000000000000000000000000000000000000001"));
				EXPECT_TRUE(expected_bitset2 == testclass2.GetCellID());

				CellID testclass3(0,1);
				testclass3.CreateCellID();
				EXPECT_NO_THROW(testclass3.CreateCellID());
				std::bitset<64> expected_bitset3 (std::string("0000000000000000000000000000000100000000000000000000000000000000"));
				EXPECT_TRUE(expected_bitset3 == testclass3.GetCellID());

				CellID testclass4(std::numeric_limits<int>::max(),1);
				testclass4.CreateCellID();
				EXPECT_NO_THROW(testclass4.CreateCellID());
				std::bitset<64> expected_bitset4 (std::string("0000000000000000000000000000000101111111111111111111111111111111"));
				EXPECT_TRUE(expected_bitset4 == testclass4.GetCellID());

				CellID testclass5(std::numeric_limits<int>::max(),std::numeric_limits<int>::max());
				testclass5.CreateCellID();
				EXPECT_NO_THROW(testclass5.CreateCellID());
				std::bitset<64> expected_bitset5 (std::string("0111111111111111111111111111111101111111111111111111111111111111"));
				EXPECT_TRUE(expected_bitset5 == testclass5.GetCellID());
}

TEST_F(CreateCellIDTest, GetCellIDThrow){
				CellID testclass1(0,0);
				EXPECT_THROW(testclass1.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass1.GetCellID(),std::runtime_error);

				CellID testclass2(-1,0);
				EXPECT_THROW(testclass2.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass2.GetCellID(),std::runtime_error);

				CellID testclass3(0,-1);
				EXPECT_THROW(testclass3.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass3.GetCellID(),std::runtime_error);

				CellID testclass4(-1,-1);
				EXPECT_THROW(testclass4.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass4.GetCellID(),std::runtime_error);

				CellID testclass5(-1,std::numeric_limits<int>::max());
				EXPECT_THROW(testclass5.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass5.GetCellID(),std::runtime_error);

				CellID testclass6(-std::numeric_limits<int>::max(),1);
				EXPECT_THROW(testclass6.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass6.GetCellID(),std::runtime_error);

				CellID testclass7(-1,std::numeric_limits<int>::max());
				EXPECT_THROW(testclass7.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass7.GetCellID(),std::runtime_error);

				CellID testclass8(-std::numeric_limits<int>::max(),-std::numeric_limits<int>::max());
				EXPECT_THROW(testclass8.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass8.GetCellID(),std::runtime_error);

				CellID testclass9(1.5,0);
				EXPECT_THROW(testclass9.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass9.GetCellID(),std::runtime_error);

				CellID testclass10(0,0.5);
				EXPECT_THROW(testclass10.CreateCellID(),std::runtime_error);
				EXPECT_THROW(testclass10.GetCellID(),std::runtime_error);
}

int main(int argc, char **argv){
				::testing::InitGoogleTest(&argc,argv);
				return RUN_ALL_TESTS();
}
