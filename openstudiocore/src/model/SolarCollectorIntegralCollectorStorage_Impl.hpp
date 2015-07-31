/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP
#define MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

class PlanarSurface;
class SolarCollectorPerformanceIntegralCollectorStorage;

namespace detail {

  /** SolarCollectorIntegralCollectorStorage_Impl is a StraightComponent_Impl that is the implementation class for SolarCollectorIntegralCollectorStorage.*/
  class MODEL_API SolarCollectorIntegralCollectorStorage_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    SolarCollectorIntegralCollectorStorage_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    SolarCollectorIntegralCollectorStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    SolarCollectorIntegralCollectorStorage_Impl(const SolarCollectorIntegralCollectorStorage_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~SolarCollectorIntegralCollectorStorage_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    virtual bool addToNode(Node & node) override;

    //@}
    /** @name Getters */
    //@{

    SolarCollectorPerformanceIntegralCollectorStorage solarCollectorPerformance() const;

    boost::optional<PlanarSurface> surface() const;

    std::string bottomSurfaceBoundaryConditionsType() const;

    bool isBottomSurfaceBoundaryConditionsTypeDefaulted() const;

    boost::optional<double> maximumFlowRate() const;

    //@}
    /** @name Setters */
    //@{

    bool setSolarCollectorPerformance(const SolarCollectorPerformanceIntegralCollectorStorage& solarCollectorPerformance);

    void resetSolarCollectorPerformance();

    bool setSurface(const PlanarSurface& surface);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorIntegralCollectorStorage");

    bool setSolarCollectorPerformanceNoClone(const SolarCollectorPerformanceIntegralCollectorStorage& solarCollectorPerformance);

    friend class openstudio::model::SolarCollectorIntegralCollectorStorage;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP

