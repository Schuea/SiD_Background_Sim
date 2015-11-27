#ifndef SUBDETECTOR_H_
#define SUBDETECTOR_H_

class Subdetector{

				public:
								Subdetector() {}

								~Subdetector() {}

								std::string GetStartLayerBin() const {
												return StartLayerBin_;
								}				
								std::string GetLengthLayerBin() const {
												return LengthLayerBin_;
								}				
								std::string GetNumberOfLayers() const {
												return NumberOfLayers_;
								}				
								std::array[3] GetROOTEnergyHisto_binning const{
												return ROOTEnergyHisto_binning;
								}				
								std::array[3] GetROOTHisto_binning1D const{
												return ROOTHisto_binning1D;
								}				
								std::array[6] GetROOTHisto_binning2D const{
												return ROOTHisto_binning2D;
								}				
								std::array[9] GetROOTHisto_binning3D const{
												return ROOTHisto_binning3D;
								}				

				protected:
								int LengthLayerBin_;
								int StartLayerBin_;
								int NumberOfLayers_;
								std::array[3] ROOTEnergyHisto_binning;
								std::array[3] ROOTHisto_binning1D;
								std::array[6] ROOTHisto_binning2D;
								std::array[9] ROOTHisto_binning3D;

};
class EcalBarrel : public Subdetector{

				public:
								EcalBarrel() {}

								~EcalBarrel() {}

								void Initialize();
};
class EcalEndcap : public Subdetector{

				public:
								EcalEndcap() {}

								~EcalEndcap() {}

								void Initialize();
};
class HcalBarrel : public Subdetector{

				public:
								HcalBarrel() {}

								~HcalBarrel() {}

								void Initialize();
};
class HcalEndcap : public Subdetector{

				public:
								HcalEndcap() {}

								~HcalEndcap() {}

								void Initialize();
};
class MuonBarrel : public Subdetector{

				public:
								MuonBarrel() {}

								~MuonBarrel() {}

								void Initialize();
};
class MuonEndcap : public Subdetector{

				public:
								MuonEndcap() {}

								~MuonEndcap() {}

								void Initialize();
};
class BeamCal : public Subdetector{

				public:
								BeamCal() {}

								~BeamCal() {}

								void Initialize();
};
class LumiCal : public Subdetector{

				public:
								LumiCal() {}

								~LumiCal() {}

								void Initialize();
};
#endif /*SUBDETECTOR_H_*/
