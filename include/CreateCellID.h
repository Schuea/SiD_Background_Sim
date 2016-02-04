#ifndef CELLID_H_
#define CELLID_H_

#include <bitset>
#include <limits>
#include <stdexcept>
#include <iostream>

class CellID{

  public:
    CellID(int const ID);
    CellID(int const ID0, int const ID1);

    virtual ~CellID();
    virtual void CreateCellID();
    virtual std::string GetCellID() const;
    
    unsigned long long CellID_ToLONGconversion(std::string const CellIDstring) const;
  private:
    CellID();
    CellID(float const ID);  //Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(double const ID);	//Float arguments are not allowed -> set them to negative to make it throw exception

    CellID(float const ID0, float const ID1);	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(int const ID0, float const ID1);	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(float const ID0, int const ID1);	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(double const ID0, double const ID1);	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(int const ID0, double const ID1);	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(double const ID0, int const ID1);	//Float arguments are not allowed -> set them to negative to make it throw exception

  protected:
    int ID_;
    int ID0_;
    int ID1_;
};

class CellID64bits : public CellID {
  public:
    CellID64bits(int const ID0, int const ID1);
    void CreateCellID();
    std::string GetCellID() const;
  private:
    std::bitset<64> CellID_bit_;

};

class CellID58bits : public CellID {
  public:
    CellID58bits(int const ID);
    void CreateCellID();
    std::string GetCellID() const;
  private:
    std::bitset<58> CellID_bit_;

};

class CellID54bits : public CellID {
  public:
    CellID54bits(int const ID);
    void CreateCellID();
    std::string GetCellID() const;
  private:
    std::bitset<54> CellID_bit_;

};

#endif /*CELLID_H_*/
