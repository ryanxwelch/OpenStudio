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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Luminaire.hpp"
#include "../../model/Luminaire_Impl.hpp"
#include "../../model/LuminaireDefinition.hpp"
#include "../../model/LuminaireDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hpp>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateLuminaire( Luminaire & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::Lights);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  LuminaireDefinition definition = modelObject.luminaireDefinition();

  idfObject.setString(LightsFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(LightsFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(LightsFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(LightsFields::ScheduleName, schedule->name().get());
  }

  idfObject.setString(LightsFields::DesignLevelCalculationMethod, "LightingLevel");

  double multiplier = modelObject.multiplier();

  idfObject.setDouble(LightsFields::LightingLevel, multiplier*definition.lightingPower());

  if (!definition.isReturnAirFractionDefaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFraction, definition.returnAirFraction());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(LightsFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionVisibleDefaulted()){
    idfObject.setDouble(LightsFields::FractionVisible, definition.fractionVisible());
  }

  if (!modelObject.isFractionReplaceableDefaulted()){
    idfObject.setDouble(LightsFields::FractionReplaceable, modelObject.fractionReplaceable());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(LightsFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  if (!definition.isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature,
                        definition.returnAirFractionCalculatedfromPlenumTemperature());
  }

  if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,
                        definition.returnAirFractionFunctionofPlenumTemperatureCoefficient1());
  }

  if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,
                        definition.returnAirFractionFunctionofPlenumTemperatureCoefficient2());
  }

  return idfObject;
}

} // energyplus

} // openstudio

