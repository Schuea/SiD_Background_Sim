#include "UsefulFunctions.h"


float FindMax(float const value, float max){
  if (value > max)  max = value;
  return max;
}
float FindMax(int const value, float max){
  return FindMax(float(value),max);
}
float FindMax(float const value, int max){
  return FindMax(value,float(max));
}
int FindMax(int const value, int max){
  return int(FindMax(float(value),float(max)));
}

float FindMin(float const value, float min){
  if (value < min)  min = value;
  return min;
}
float FindMin(float const value, int min){
  return FindMin(value,float(min));
}
float FindMin(int const value, float min){
  return FindMin(float(value),min);
}
int FindMin(int const value, int min){
  return int(FindMin(float(value), float(min)));
}
