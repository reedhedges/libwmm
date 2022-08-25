
#include "wmm.hh"
#include <cmath>
#include <cassert>
#include <cstdio>

void close(double d1, double d2)
{
  assert(fabs(d2-d1) < 0.00001);
}

int main()
{
  WMM wmm("WMM.COF");
  double d = wmm.calculateDeclination(42.94694, -72.10028, 0, std::chrono::year(2022)/8/25); //{2022, 8, 25});
  close(d, -13.95237);
  printf("declination=%f\n", d);
  d = wmm.calculateDeclination(42.94694, -72.10028, 500, std::chrono::year(2022)/8/25); //{2022, 8, 25});
  close(d, -13.95076);
  printf("declination=%f\n", d);
  d = wmm.calculateDeclination(41.70306, -70.7625, 500, std::chrono::year(2022)/8/25); //{2022, 8, 25});
  close(d, -14.20065);
  printf("declination=%f\n", d);
  d = wmm.calculateDeclination(41.70306, -70.7625, 500, std::chrono::year(2023)/6/6); //{2023, 6, 6});
  close(d, -14.14608);
  printf("declination=%f\n", d);
  d = wmm.calculateDeclination(51.50722, 0.1275, 500, std::chrono::year(2024)/1/1); //{2024, 1, 1});
  close(d, 0.90249);
  printf("declination=%f\n", d);
  puts("all ok");
  return 0;
}
  
  
  
