#ifndef SUBDETECTOR_H_
#define SUBDETECTOR_H_

class Subdetector{

  public:
    Subdetector() {}

    ~Subdetector() {}

    std::string GetName() const{
      return Name_;
    }
    std::string GetStartLayerBin() const {
      return StartLayerBin_;
    }				
    std::string GetLengthLayerBin() const {
      return LengthLayerBin_;
    }				
    std::string GetNumberOfLayers() const {
      return NumberOfLayers_;
    }				
    std::vector<float> GetROOTEnergyHisto_binning const{
      return ROOTEnergyHisto_binning;
    }				
    std::vector<float> GetROOTHisto_binning1D const{
      return ROOTHisto_binning1D;
    }				
    std::vector<float> GetROOTHisto_binning2D const{
      return ROOTHisto_binning2D;
    }				
    std::vector<float> GetROOTHisto_binning3D const{
      return ROOTHisto_binning3D;
    }				

  protected:
    std::string Name_;
    int LengthLayerBin_;
    int StartLayerBin_;
    int NumberOfLayers_;
    std::vector<float> ROOTEnergyHisto_binning;
    std::vector<float> ROOTHisto_binning1D;
    std::vector<float> ROOTHisto_binning2D;
    std::vector<float> ROOTHisto_binning3D;

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
class SiVertexEndcap : public Subdetector{

  public:
    SiVertexEndcap() {}

    ~SiVertexEndcap() {}

    void Initialize();
};
class SiVertexBarrel : public Subdetector{

  public:
    SiVertexBarrel() {}

    ~SiVertexBarrel() {}

    void Initialize();
};
class SiTrackerEndcap : public Subdetector{

  public:
    SiTrackerEndcap() {}

    ~SiTrackerEndcap() {}

    void Initialize();
};
class SiTrackerBarrel : public Subdetector{

  public:
    SiTrackerBarrel() {}

    ~SiTrackerBarrel() {}

    void Initialize();
};
class SiTrackerForward : public Subdetector{

  public:
    SiTrackerForward() {}

    ~SiTrackerForward() {}

    void Initialize();
};
#endif /*SUBDETECTOR_H_*/
