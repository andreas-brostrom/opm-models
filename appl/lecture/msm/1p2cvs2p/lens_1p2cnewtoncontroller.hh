// $Id$
/*****************************************************************************
 *   Copyright (C) 2008 by Bernd Flemisch, Andreas Lauser                    *
 *   Institute of Hydraulic Engineering                                      *
 *   University of Stuttgart, Germany                                        *
 *   email: <givenname>.<name>@iws.uni-stuttgart.de                          *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version, as long as this copyright notice    *
 *   is included in its original form.                                       *
 *                                                                           *
 *   This program is distributed WITHOUT ANY WARRANTY.                       *
 *****************************************************************************/
/*!
 * \file
 * \brief A 1p2c specific controller for the newton solver.
 *
 * This controller 'knows' what a 'physically meaningful' solution is
 * which allows the newton method to abort quicker if the solution is
 * way out of bounds.
 */
#ifndef DUMUX_LENS_1P2C_NEWTON_CONTROLLER_HH
#define DUMUX_LENS_1P2C_NEWTON_CONTROLLER_HH

#include <dumux/nonlinear/newtoncontroller.hh>

namespace Dumux {
/*!
 * \ingroup TwoPTwoCBoxModel
 * \brief A 2p2c specific controller for the newton solver.
 *
 * This controller 'knows' what a 'physically meaningful' solution is
 * which allows the newton method to abort quicker if the solution is
 * way out of bounds.
 */
template <class TypeTag>
class LensOnePTwoCNewtonController
    : public NewtonController<TypeTag >
{
    typedef LensOnePTwoCNewtonController<TypeTag>  ThisType;
    typedef NewtonController<TypeTag>      ParentType;
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(NewtonController)) Implementation;

    typedef typename GET_PROP_TYPE(TypeTag, PTAG(Scalar)) Scalar;
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(Model)) Model;
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(NewtonMethod)) NewtonMethod;

    typedef typename GET_PROP(TypeTag, PTAG(SolutionTypes)) SolutionTypes;
    typedef typename SolutionTypes::SolutionFunction SolutionFunction;


    typedef typename GET_PROP_TYPE(TypeTag, PTAG(OnePTwoCIndices)) Indices;

    enum {
        konti = Indices::konti,
        transport = Indices::transport
    };

public:
    LensOnePTwoCNewtonController()
    {
        this->setRelTolerance(1e-7);
        this->setTargetSteps(9);
        this->setMaxSteps(18);

        relDefect_ = 1e100;
        //load interface-file

        Dumux::InterfaceProblemProperties interfaceProbProps("interface1p2c.xml");

        maxTimeStepSize_ = interfaceProbProps.IPP_MaxTimeStepSize;
    };

    //! Suggest a new time stepsize based either on the number of newton
    //! iterations required or on the variable switch
    void newtonEndStep(SolutionFunction &u, SolutionFunction &uOld)
    {
        // call the method of the base class
        ParentType::newtonEndStep(u, uOld);

        relDefect_ = 0;
        for (unsigned i = 0; i < (*u).size(); ++i) {
            Scalar normP = std::max(1e3,std::abs((*u)[i][konti]));
            normP = std::max(normP, std::abs((*uOld)[i][konti]));

            Scalar normTrans = std::max(1e-3,std::abs((*u)[i][transport]));
            normTrans = std::max(normTrans, std::abs((*uOld)[i][transport]));

            relDefect_ = std::max(relDefect_,
                                  std::abs((*u)[i][konti] - (*uOld)[i][konti])/normP);
            relDefect_ = std::max(relDefect_,
                                  std::abs((*u)[i][transport] - (*uOld)[i][transport])/normTrans);
        }
    }

    //! Suggest a new time stepsize based either on the number of newton
    //! iterations required or on the variable switch
    Scalar suggestTimeStepSize(Scalar oldTimeStep) const
    {
        // use function of the newtoncontroller
        return std::min(maxTimeStepSize_, ParentType::suggestTimeStepSize(oldTimeStep));
    }

    //! Returns true iff the current solution can be considered to
    //! be acurate enough
    bool newtonConverged()
    {
        return relDefect_ <= this->tolerance_;
    };

protected:
    Scalar relDefect_;
private:
    Scalar maxTimeStepSize_;
};
}

#endif
