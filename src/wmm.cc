#include <stdexcept>
#include <filesystem>
#include <chrono>
#include <string>
#include <cassert>
#include <climits>

#include "libwmm/wmm.hh"

extern "C" {
#include "GeomagnetismHeader.h"
#include "EGM9615.h"
}

using namespace libwmm;

namespace libwmm_private 
{
  struct WMM_private
  {
      MAGtype_MagneticModel MagneticModel;
      MAGtype_MagneticModel TimedMagneticModel;
      MAGtype_Ellipsoid Ellip;
      MAGtype_Geoid Geoid;
  };
}

using namespace libwmm_private;

std::string WMM::modelName()
{
  return std::string{priv->MagneticModel.ModelName};
}

WMM::WMM(const std::filesystem::path& filename)
{
    priv = new WMM_private;
    assert(priv);

    // todo move some or all of this into WMM_private constructor?

    int err = MAG_robustReadMagModel(filename.string().c_str(), &(priv->MagneticModel));
    if(err)
    {
        delete priv;
        throw std::runtime_error(std::string("Error loading WMM data file \"") + std::string(filename) + std::string("\""));
    }

    int nMax = 0;
    if(nMax < priv->MagneticModel.nMax) 
      nMax = priv->MagneticModel.nMax;
    int NumTerms = ((nMax + 1) * (nMax + 2) / 2);
    err = MAG_AllocateModelTerms(&(priv->TimedMagneticModel), NumTerms); /* For storing the time modified WMM Model parameters */
    if(err)
    {
      delete priv;
      throw std::bad_alloc();
    }
    MAG_SetDefaults(&(priv->Ellip), &(priv->Geoid)); /* Set default values and constants */
    /* Check for Geographic Poles ?? */



    /* Set EGM96 Geoid parameters */
    priv->Geoid.GeoidHeightBuffer = GeoidHeightBuffer;  // GeoidHeightBuffer is array, this copies pointer not whole data table 
    priv->Geoid.Geoid_Initialized = 1;
    /* Set EGM96 Geoid parameters END */
}

double WMM::calculateDeclination(double latitude, double longitude, double altitude, std::chrono::year_month_day date) const
{
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;

    // TODO could check bounds of latitude, longitude and altitude

    CoordGeodetic.phi = latitude;
    CoordGeodetic.lambda = longitude;
    CoordGeodetic.HeightAboveGeoid = altitude / 1000.0; // convert from meters to km
    MAG_ConvertGeoidToEllipsoidHeight(&CoordGeodetic, &(priv->Geoid));
    if(!date.ok())
      throw std::runtime_error("WMM::calculateDeclination: Invalid date given");
    UserDate.Year = int(date.year());
    assert(unsigned(date.month()) <= INT_MAX);
    UserDate.Month = (int) unsigned(date.month());
    assert(unsigned(date.day()) <= INT_MAX);
    UserDate.Day = (int) unsigned(date.day());
    char errbuf[256];
    if(!MAG_DateToYear(&UserDate, errbuf))
    {
      throw std::runtime_error(std::string("Error converting given date: ") + std::string(errbuf));
    }
    // todo do we need to call MAG_DateToYear?

    if(UserDate.DecimalYear > priv->MagneticModel.CoefficientFileEndDate || UserDate.DecimalYear < priv->MagneticModel.epoch)
    {
      throw std::runtime_error(std::string("WMM::calculateDeclination: given year ") + 
        std::to_string(UserDate.DecimalYear) + 
        std::string(" is out of range of WMM data [") + 
        std::to_string(priv->MagneticModel.epoch) +
        std::string(", ") +
        std::to_string(priv->MagneticModel.CoefficientFileEndDate) +
        std::string("]")
      );
    }

    MAG_GeodeticToSpherical(priv->Ellip, CoordGeodetic, &CoordSpherical); /*Convert from geodetic to Spherical Equations: 17-18, WMM Technical report*/
    MAG_TimelyModifyMagneticModel(UserDate, &(priv->MagneticModel), &(priv->TimedMagneticModel)); /* Time adjust the coefficients, Equation 19, WMM Technical report */
    double declination = 0;
    int err = MAG_declination(priv->Ellip, CoordSpherical, CoordGeodetic, &(priv->TimedMagneticModel), &declination);
    if(err)
      throw std::runtime_error("Unable to calculate declination");

    //MAG_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);
    //MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);

    return declination;
}


WMM::~WMM() noexcept
{
    MAG_FreeMagneticModelMemory(&(priv->TimedMagneticModel)); // todo move into WMM_private destructor
    MAG_FreeMagneticModelMemory(&(priv->MagneticModel)); // todo move into WMM_private destructor
    delete priv;
}

