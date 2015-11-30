#ifndef USEFULFUNCTIONS
#define USEFULFUNCTIONS


float FindMax(float const value, float const max){
  if (value > max)  max = value;
  return max;
}
float FindMax(int const value, float const max){
  return FindMax(float(value),max);
}
float FindMax(float const value, int const max){
  return FindMax(value,float(max));
}
int FindMax(int const value, int const max){
  return int(FindMax(float(value),float(max)));
}

float FindMin(float const value, float const min){
  if (value < min)  min = value;
  return min;
}
float FindMin(float const value, int const min){
  return FindMin(value,float(min));
}
float FindMin(int const value, float const min){
  return FindMin(float(value),min);
}
int FindMin(int const value, int const min){
  return int(FindMin(float(value), float(min)));
}

#endif /*USEFULFUNCTIONS*/
