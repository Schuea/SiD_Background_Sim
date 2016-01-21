#ifndef USEFULFUNCTIONS
#define USEFULFUNCTIONS

#include <utility>

float FindMax(float const value, float max);
float FindMax(int const value, float max);
float FindMax(float const value, int max);
int FindMax(int const value, int max);

float FindMin(float const value, float min);
float FindMin(float const value, int min);
float FindMin(int const value, float min);
int FindMin(int const value, int min);

struct CompareFirst
{
  CompareFirst(int val) : val_(val) {}

  bool operator()(const std::pair< int,int >& elem) const {
    return val_ == elem.first;
  }
  private:
    int val_;
};
#endif
