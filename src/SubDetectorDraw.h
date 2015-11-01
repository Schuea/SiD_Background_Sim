/*
 * SubDetectorDraw.h
 *
 *  Created on: Oct 22, 2015
 *      Author: schuea
 */

#ifndef SUBDETECTORDRAW_H_
#define SUBDETECTORDRAW_H_


class SubDetectorDraw{

protected:
	std::array<int, 3> binning1D;
	std::array<int, 6> binning2D;
	std::array<int, 9> binning3D;
public:
	virtual SubDetectorDraw(){}
	virtual ~SubDetectorDraw(){}
	std::array<int, 3> getBinning1D() const{return binning1D;}
	std::array<int, 6> getBinning2D() const{return binning2D;}
	std::array<int, 9> getBinning3D() const{return binning3D;}
};

class EcalBarrelDraw : public SubDetectorDraw{

public:
	virtual EcalBarrelDraw() :
	SubDetectorDraw(){
		binning1D = { 50, 0, 50 };
		binning2D = { 50, -1500, 1500, 50, -1500, 1500 };
		binning3D = { 50, -1700, 1700, 50, -1500, 1500, 50, -1500, 1500 };
	}
	virtual ~EcalBarrelDraw(){}
};

class EcalEndcapDraw : public SubDetectorDraw{

public:
	virtual EcalEndcapDraw() :
	SubDetectorDraw(){
		binning1D = { 50, 0, 50 };
		binning2D = { 50, -1500, 1500, 50, -1500, 1500 };
		binning3D = { 50, -1850, 1850, 50, -1500, 1500, 50, -1500, 1500 };
	}
	virtual ~EcalEndcapDraw(){}
};

class HcalBarrelDraw : public SubDetectorDraw{

public:
	virtual HcalBarrelDraw() :
	SubDetectorDraw(){
		binning1D = { 50, 0, 50 };
		binning2D = { 50, -1500, 1500, 50, -1500, 1500 };
		binning3D = { 50, -1700, 1700, 50, -1500, 1500, 50, -1500, 1500 };
	}
	virtual ~HcalBarrelDraw(){}
};

class HcalEndcapDraw : public SubDetectorDraw{

public:
	virtual HcalEndcapDraw() :
	SubDetectorDraw(){
		binning1D = { 50, 0, 50 };
		binning2D = { 50, -1500, 1500, 50, -1500, 1500 };
		binning3D = { 150, -5100, 5100, 50, -1350, 1350, 50, -1350, 1350 };
	}
	virtual ~HcalEndcapDraw(){}
};

class MuonBarrelDraw : public SubDetectorDraw{

public:
	virtual MuonBarrelDraw() :
	SubDetectorDraw(){
		binning1D = { 50, 0, 50 };
		binning2D = { 50, -1500, 1500, 50, -1500, 1500 };
		binning3D = { 50, -1700, 1700, 50, -1500, 1500, 50, -1500, 1500 };
	}
	virtual ~MuonBarrelDraw(){}
};

class MuonEndcapDraw : public SubDetectorDraw{

public:
	virtual MuonEndcapDraw() :
	SubDetectorDraw(){
		binning1D = { 50, 0, 50 };
		binning2D = { 150, -5700, 5700, 150, -5700, 5700 };
		binning3D = { 150, -5600, 5600, 150, -5700, 5700, 150, -5700, 5700 };
	}
	virtual ~MuonEndcapDraw(){}
};

class BeamCalDraw : public SubDetectorDraw{

public:
	virtual BeamCalDraw() :
	SubDetectorDraw(){
		binning1D = { 150, 0, 150 };
		binning2D = { 100, -135, 135, 100, -135, 135 };
		binning3D = { 150, -3200, 3200, 100, -135, 135, 100, -135, 135 };
	}
	virtual ~BeamCalDraw(){}
};

class LumiCalDraw : public SubDetectorDraw{

public:
	virtual LumiCalDraw() :
	SubDetectorDraw(){
		binning1D = { 150, 0, 150 };
		binning2D = { 50, -200, 200, 50, -200, 200 };
		binning3D = { 50, -1900, 1900, 50, -200, 200, 50, -200, 200 };
	}
	virtual ~LumiCalDraw(){}
};


#endif /* SUBDETECTORDRAW_H_ */
