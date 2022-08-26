
Simple C++20 Wrapper Library for WMM Declination Calculations
-------------------------------------------------------------

Small C++20 library that wraps the "GeomagnetismLibrary" provided by NOAA
providing a simpler, more standard, and faster interface for getting just a compass
declination for a given WGS84 latitude, longitude, altitude and date. The
GeomagnetismLibrary code converts a given WGS84 latitude, longitude and altitude
to a spherical EGM9615 coordinate, then calculates declination and other values
for that coordinate based on data read from a magnetic model file (WMM.COF).

Requires C++20 since it uses `std::chrono::year_month_day` type added in C++20.
(This could be changed if neccesary.)  

Various output directories and names of commands can be set via environment variables when running `make`,
see top of Makefile.

libwmm is compiled as a static library with optimization and link-time optimization (LTO/IPO) enabled.

To get initial or updated WMM.COF file, download from 
<https://www.ngdc.noaa.gov/geomag/WMM/> and extract from archive.   
Supply path to this file when creating a WMM object.

### To Do

  * More tests including impossible or unusual locations (poles?), verify behavior in "blackout" zones.
  * Change GeomagnetismLibrary.c to store error messages instead of printing them to stdout.
  * Add option to GeomagnetismLibrary to read COF file from user-supplied stream (FILE pointer) or callback functions.
  * Could add API that doesn't use `std::chrono::year_month_day` so it doesn't require C++20.
  * Maybe move data from EGM9615 into separate .c file with extern interface so it doesn't need to be parsed every time wmm.cc is compiled. Try to prevent IDE's and analysis tools like
clang-tidy from parsing it. (Since it's big.) 

### Copyright and Disclaimers

This library is provided with no license or under any copyright, into the public domain.  
It is provided AS IS and WITHOUT ANY WARRANTY.

The GeomagnetismLibrary WMM source code (`GeomagnetismLibrary.c`, `GeomagnetismHeader.h`, `EGM9615.h`, `wmm_point.c`) 
is provided by the US government, in the public domain and not licensed or under copyright.
The information and software may be used freely by the public. As required by 17 U.S.C. 403,
third parties producing copyrighted works consisting predominantly of the material produced by
U.S. government agencies must provide notice with such work(s) identifying the U.S. Government material
incorporated and stating that such material is not subject to copyright protection.

`GeomagnetismLibrary.c` has been modified to
add functions that only calculate declination and skip other parameters,
and to remove unused functions. Additional error checking has also been added and 
minor errors/warnings fixed for C++20/C17.

