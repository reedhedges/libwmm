
Small C++20 library that wraps the "GeomagnetismLibrary" provided by NOAA
providing a simpler, more standard, and faster interface for getting just a compass
declination for a given WGS84 latitude, longitude, altitude and date.

`GeomagnetismLibrary.c` provided by NOAA has been modified to
add functions that only calculate declination and skip other parameters,
and to remove unused functions. Additional error checking has also been added.

To get initial or updated WMM.COF file, download from 
<https://www.ngdc.noaa.gov/geomag/WMM/> and extract from archive.   
Supply path to this file when creating a WMM object.

By default, libwmm is compiled as a static library with link-time optimization (LTO/IPO) enabled.

This library is provided with no license or under any copyright, into the public domain.  
It is provided AS IS and WITHOUT ANY WARRANTY.

The GeomagnetismLibrary WMM source code (`GeomagnetismLibrary.c`, `GeomagnetismHeader.h`, `EGM9615.h`, `wmm_point.c`) 
is provided by the US government, in the public domain and not licensed or under copyright.
The information and software may be used freely by the public. As required by 17 U.S.C. 403,
third parties producing copyrighted works consisting predominantly of the material produced by
U.S. government agencies must provide notice with such work(s) identifying the U.S. Government material
incorporated and stating that such material is not subject to copyright protection.

