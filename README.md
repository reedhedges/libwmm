
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

You can use either CMake or just GNU Make to build. 

If using Make, various output 
directories and names of commands can be set via environment variables when running `make`,
see top of Makefile.

If using CMake, some options are available, including: 
  * `CMAKE_BUILD_TYPE`: "Release", "Debug", or "RelWithDebug"
  * `LIBWMM_ENABLE_IPO`: if ON, enable inter-procedural optimization (link-time optimization) whenever `CMAKE_BUILD_TYPE` starts with the pattern "Rel.\*" (i.e. in "Release" or "RelWithDebug" modes.) 

libwmm is compiled as a static library.

To get initial or updated WMM.COF file, download from 
<https://www.ngdc.noaa.gov/geomag/WMM/> and extract from archive.   
Supply path to this file when creating a WMM object.

SHDF model file support has been removed, only WMM.COF file is supported.

### To Do

  * Verify all CMake build types (Debug, Release, etc.). 
  * More tests including impossible or unusual locations (poles?), verify behavior in "blackout" zones.
  * Improve error handling in GeomagnetismLibrary.c:
    * Some functions return an error code > 0 on errors, and 0 on success. Some return 1 (true) on success, 0 on error. Fix.
    * Store error messages instead of printing them to stdout.
    * Ensure that error code return values from functions aren't being ignored.
  * Add option to GeomagnetismLibrary to read COF file from user-supplied stream (FILE pointer) or callback functions.
  * Could add API that doesn't use `std::chrono::year_month_day` so it doesn't require C++20.
  * Maybe move data from EGM9615 into separate .c file with extern interface so it doesn't need to be parsed every time wmm.cc is compiled. Try to prevent IDEs etc.  from parsing it. (Since it's big.) 

### Copyright and Disclaimers

This library is provided with no license or under any copyright, into the public domain.  
It is provided AS IS and WITHOUT ANY WARRANTY.

The GeomagnetismLibrary WMM source code (`GeomagnetismLibrary.c`, `GeomagnetismHeader.h`, `EGM9615.h`, `wmm_point.c`) 
is provided by the US government, in the public domain and not licensed or under copyright.
The information and software may be used freely by the public. As required by 17 U.S.C. 403,
third parties producing copyrighted works consisting predominantly of the material produced by
U.S. government agencies must provide notice with such work(s) identifying the U.S. Government material
incorporated and stating that such material is not subject to copyright protection.

The NOAA-supplied code has been modified to add functions that only calculate declination and skip other parameters,
to remove unused functions. Parts of the API have been changed for better consistency, and to reduce some memory allocation.
Additional error checking has also been added and minor errors/warnings fixed for C++20/C17.

