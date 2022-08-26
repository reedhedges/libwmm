
#pragma once

#include <chrono>
#include <filesystem>
#include <string>

namespace libwmm_private {
  class WMM_private;
}


namespace libwmm
{

  class WMM
  {
  private:
    libwmm_private::WMM_private *priv;
  public:
    /** Throws std::runtime_error if @a filename could not be loaded, memory could not be allocated, or other error. */
    explicit WMM(const std::filesystem::path& filename);
    /** Calculate magnetic declination for given position and time.
        @param latitude WGS-84 latitude
        @param longitude WGS-84 longitude
        @param altitude in meters above sea level
        @param date calculate declination for given date
        Throws std::runtime_error if declination could not be calculated or @a date is invalid */
    double calculateDeclination(double latitude, double longitude, double altitude, std::chrono::year_month_day date) const;
    std::string modelName();
    ~WMM() noexcept;
  };
   
}
