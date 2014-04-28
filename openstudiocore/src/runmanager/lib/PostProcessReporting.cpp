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

#include <vector>
#include <utilities/data/Attribute.hpp>
#include <utilities/sql/SqlFile.hpp>

#include "PostProcessReporting.hpp"

namespace openstudio {
namespace runmanager {
namespace detail {


  std::vector<Attribute> PostProcessReporting::go(const SqlFile &t_sqlFile, const std::string& jobType)
  {
    std::vector<Attribute> attributes;
    boost::optional<double> val;
    std::string query;

   //Total Site Energy (GJ)
   val = t_sqlFile.totalSiteEnergy();
    if (val){
      attributes.push_back(Attribute("Total Site Energy", *val, "GJ"));
      attributes.back().setSource(jobType);
    }

    //Net Site Energy (GJ)
    val = t_sqlFile.netSiteEnergy();
    if (val){
      attributes.push_back(Attribute("Net Site Energy", *val, "GJ"));
      attributes.back().setSource(jobType);
    }

    //Total Source Energy (GJ)
    val = t_sqlFile.totalSourceEnergy();
    if (val){
      attributes.push_back(Attribute("Total Source Energy", *val, "GJ"));
      attributes.back().setSource(jobType);
    }

    //Net Source Energy (GJ)
    val = t_sqlFile.totalSourceEnergy();
    if (val){
      attributes.push_back(Attribute("Net Source Energy", *val, "GJ"));
      attributes.back().setSource(jobType);
    }

    //Annual Total Utility Cost ($)
    val = t_sqlFile.annualTotalUtilityCost();
    if (val){
      attributes.push_back(Attribute("Annual Total Utility Cost", *val, "$"));
      attributes.back().setSource(jobType);
    }

    //Annual Electrict Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::Electricity);
    if (val){
      attributes.push_back(Attribute("Annual Electric Total Cost", *val, "$"));
      attributes.back().setSource(jobType);
    }

    //Annual Gas Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::Gas);
    if (val){
      attributes.push_back(Attribute("Annual Gas Total Cost", *val, "$"));
      // HERE
    }

    //Annual District Cooling Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::DistrictCooling);
    if (val){
      attributes.push_back(Attribute("Annual District Cooling Total Cost", *val, "$"));
    }

    //Annual District Heating Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::DistrictHeating);
    if (val){
      attributes.push_back(Attribute("Annual District Heating Total Cost", *val, "$"));
    }

    //Annual Water Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::Water);
    if (val){
      attributes.push_back(Attribute("Annual Water Total Cost", *val, "$"));
    }

    //Net Site Energy Use Intentsity (MJ/m^2)
    boost::optional<double> hours = t_sqlFile.hoursSimulated();
    if (!hours){
    LOG(Warn, "Reporting Net Site Energy Use Intentsity \
              with unknown number of simulation hours");
    }else if(*hours != 8760){
    LOG(Warn, "Reporting Net Site Energy Use Intentsity with " << *hours << " hrs");
    }
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='Site and Source Energy' AND \
                            RowName='Net Site Energy' AND \
                            ColumnName='Energy Per Total Building Area' AND \
                            Units='MJ/m2'";
    val = t_sqlFile.execAndReturnFirstDouble(query);    
    if (val){
      attributes.push_back(Attribute("Net Site Energy Use Intentsity", *val, "MJ/m^2"));
    }    

    //Instantaneous Peak Electricity Demand (W)    
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='DemandEndUseComponentsSummary' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='End Uses' AND \
                            RowName='Total End Uses' AND \
                            ColumnName='Electricity' AND \
                            Units='W'";
    val = t_sqlFile.execAndReturnFirstDouble(query);
    if (val){
      attributes.push_back(Attribute("Instantaneous Peak Electricity Demand", *val, "kW"));
    }         

    //Electricity Total End Uses (GJ)
    val = t_sqlFile.electricityTotalEndUses();
    if (val){
      attributes.push_back(Attribute("Electricity Total End Uses", *val, "GJ"));
    }      
    
    //Natural Gas Total End Uses (GJ)
    val = t_sqlFile.naturalGasTotalEndUses();
    if (val){
      attributes.push_back(Attribute("Natural Gas Total End Uses", *val, "GJ"));
    }      
          
    //District Cooling Total End Uses (GJ)
    val = t_sqlFile.districtCoolingTotalEndUses();
    if (val){
      attributes.push_back(Attribute("District Cooling Total End Uses", *val, "GJ"));
    }      
        
    //District Heating Total End Uses (GJ)
    val = t_sqlFile.districtHeatingTotalEndUses();
    if (val){
      attributes.push_back(Attribute("District Heating Total End Uses", *val, "GJ"));
    }      
        
    //Other Fuel Total End Uses (GJ)
    val = t_sqlFile.otherFuelTotalEndUses();
    if (val){
      attributes.push_back(Attribute("Other Fuel Total End Uses", *val, "GJ"));
    }      
       
    //End Uses   
    boost::optional<EndUses> enduses = t_sqlFile.endUses();
    if (enduses){
      std::vector<Attribute> v = enduses->attribute().valueAsAttributeVector();

      for (std::vector<Attribute>::iterator itr = v.begin();
          itr != v.end();
          ++itr)
      {
        std::vector<Attribute> v2 = itr->valueAsAttributeVector();
        for (std::vector<Attribute>::iterator itr2 = v2.begin();
            itr2 != v2.end();
            ++itr2)
        {
          std::vector<Attribute> v3 = itr2->valueAsAttributeVector();
          for (std::vector<Attribute>::iterator itr3 = v3.begin();
              itr3 != v3.end();
              ++itr3)
          {
            attributes.push_back(Attribute(enduses->attribute().name() + "." + itr->name() + "." + itr2->name() + "." + itr3->name(), 
                  itr3->valueAsDouble(), 
                  *itr3->units()));
          }
        }
      }

    }

    //First Year Capital Costs
    //find the inflation approach (either constant dollar or current dollar)
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
             ReportName='Life-Cycle Cost Report' AND \
             ReportForString='Entire Facility' AND \
             TableName='Life-Cycle Cost Parameters' AND \
             RowName='Inflation Approach' AND \
             ColumnName='Value'";
    if ( boost::optional<std::string> infApprStr = t_sqlFile.execAndReturnFirstString(query) ) {
      //find the base year (first year)
      query = "SELECT Value FROM tabulardatawithstrings WHERE \
               ReportName='Life-Cycle Cost Report' AND \
               ReportForString='Entire Facility' AND \
               TableName='Life-Cycle Cost Parameters' AND \
               RowName='Base Date' AND \
               ColumnName='Value'"; 
      if ( boost::optional<std::string> baseYrString = t_sqlFile.execAndReturnFirstString(query) ) {
        //capital cash flow for this year
        query = "SELECT Value FROM tabulardatawithstrings WHERE \
                 ReportName='Life-Cycle Cost Report' AND \
                 ReportForString='Entire Facility' AND \
                 TableName='Capital Cash Flow by Category (Without Escalation)' AND \
                 RowName='" + *baseYrString + "' AND \
                 ColumnName='Total'";
        val = t_sqlFile.execAndReturnFirstDouble(query);
      }
      if (val){
        attributes.push_back(Attribute("First Year Capital Costs", *val, "$"));
      }
    }

    //Total LifeCycle Cost (present value)
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
             ReportName='Life-Cycle Cost Report' AND \
             ReportForString='Entire Facility' AND \
             TableName='Present Value by Year' AND \
             RowName='TOTAL' AND \
             ColumnName='Present Value of Costs'"; 
    val = t_sqlFile.execAndReturnFirstDouble(query);
    if (val){
      attributes.push_back(Attribute("Total Life Cycle Cost", *val, "$"));
    }

    if (attributes.empty())
    {
      LOG(Warn, "No attributes loaded for report");
    }

    return attributes;
  }



} // detail
} // runmanager
} // openstudio



