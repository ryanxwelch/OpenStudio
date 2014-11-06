/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef AIRFLOW_SURFACENETWORKBUILDER_HPP
#define AIRFLOW_SURFACENETWORKBUILDER_HPP

#include <vector>

#include "AirflowAPI.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

namespace openstudio {

class ProgressBar;
namespace model {
class Model;
class Space;
class ThermalZone;
class SubSurface;
class Surface;
}

namespace airflow {

/** SurfaceNetworkBuilder objects walk the OpenStudio model as a network of surfaces.
 *
 *  The SurfaceNetworkBuilder object walks the OpenStudio model as a network of surface,
 *  calling a different function once for each pair of interior surfaces or subsurfaces and
 *  once for each exterior surface or subsurface. The intended usage is to subclass for a
 *  specific application.
 *
 */
class AIRFLOW_API SurfaceNetworkBuilder
{
public:
  /** @name Constructors and Destructors */
  //@{
  /** Construct a new object. */
  explicit SurfaceNetworkBuilder(ProgressBar *progressBar=0);
  /** Destroy the translator.*/
  virtual ~SurfaceNetworkBuilder(){}

  //@}
  /** @name Network Construction Functions */
  //@{

  bool build(model::Model & model);

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the progress bar currently associated with the object. */
  ProgressBar * progressBar() const;

  /** Sets the progress bar to be updated by the object. */
  void setProgressBar(ProgressBar *progressBar);

  //@}
  /** @name Miscellaneous Functions */
  //@{

  /** Clear any internally stored information in the builder object. */
  virtual void clear()
  {
  }

  /** Returns warning messages generated by the last translation. */
  std::vector<LogMessage> warnings() const;

  /** Returns error messages generated by the last translation. */
  std::vector<LogMessage> errors() const;

  /** Returns error messages generated by the last translation. */
  std::vector<LogMessage> logMessages() const;

  //@}

protected:
  void progress();
  void initProgress(int max, std::string label);

  virtual bool linkExteriorSurface(model::ThermalZone zone, model::Space space, model::Surface surface);
  virtual bool linkExteriorSubSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::SubSurface subSurface);
  virtual bool linkInteriorSurface(model::ThermalZone zone, model::Space space, model::Surface surface,
    model::Surface adjacentSurface, model::Space adjacentSpace, model::ThermalZone adjacentZone);
  virtual bool linkInteriorSubSurface(model::ThermalZone zone, model::Space space, model::Surface surface, model::SubSurface subSurface,
    model::SubSurface adjacentSubSurface, model::Surface adjacentSurface, model::Space adjacentSpace, model::ThermalZone adjacentZone);

private:
  ProgressBar* m_progressBar;
  StringStreamLogSink m_logSink;

  REGISTER_LOGGER("openstudio.airflow.SurfaceNetworkBuilder");
};

} // airflow
} // openstudio

#endif // AIRFLOW_SURFACENETWORKBUILDER_HPP