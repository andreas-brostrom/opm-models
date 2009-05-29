/*****************************************************************************
 *   Copyright (C) 2009 by Andreas Lauser                                    *
 *   Institute of Hydraulic Engineering                                      *
 *   University of Stuttgart, Germany                                        *
 *   email: andreas.lauser _at_ iws.uni-stuttgart.de                         *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version, as long as this copyright notice    *
 *   is included in its original form.                                       *
 *                                                                           *
 *   This program is distributed WITHOUT ANY WARRANTY.                       *
 *****************************************************************************/
#ifndef DUMUX_TAGS_HH
#define DUMUX_TAGS_HH

#include <dumux/auxiliary/properties.hh>

#include <dune/grid/common/genericreferenceelements.hh>

/*!
 * \file
 *
 * \brief In this file we declare all type tags and property tags
 *        which we use somewhere in the code...
 */
namespace Dune
{

namespace Properties {
//////////////////////////////////////////////////////////////////
// Type tags
//////////////////////////////////////////////////////////////////

//! The type tag for models based on the box-scheme
NEW_TYPE_TAG(BoxScheme);

//! The type tag for the 2p2c models
NEW_TYPE_TAG(BoxTwoPTwoC, INHERITS_FROM(BoxScheme));


//////////////////////////////////////////////////////////////////
// Property tags
//////////////////////////////////////////////////////////////////

//! Property tag for scalar values
NEW_PROP_TAG(Scalar);

//! Number of equations in the system of PDEs
NEW_PROP_TAG(NumEq);

//! Property tag for types associated with the solution of the PDE.
//! This means vectors of primary variables, solution functions on the
//! grid, and elements, and shape functions.
NEW_PROP_TAG(SolutionTypes);

//! The type of the grid
NEW_PROP_TAG(Grid);

//! The type of the grid view
NEW_PROP_TAG(GridView);

//! Reference elements to be used
NEW_PROP_TAG(ReferenceElements);

//! The type of the problem
NEW_PROP_TAG(Problem);

//! The type of the discretization
NEW_PROP_TAG(Model);

//! The type of the finite-volume geometry in the box scheme
NEW_PROP_TAG(FVElementGeometry);

//! The type of the local jacobian operator
NEW_PROP_TAG(LocalJacobian);

//! Data structures defined on the vertices of the grid
NEW_PROP_TAG(VertexData);
//! Data structures defined on the elements of the grid
NEW_PROP_TAG(ElementData);
//! Data required to calculate a flux over a face 
NEW_PROP_TAG(FluxData);

//! The type of the newton method
NEW_PROP_TAG(NewtonMethod);
//! The type of the newton controller
NEW_PROP_TAG(NewtonController);

//! The default value of the upwind parameter
NEW_PROP_TAG(UpwindAlpha);

//! The value of the upwind parameter for the mobility
NEW_PROP_TAG(MobilityUpwindAlpha);

//! The traits for the 2p2c models. TODO: this is too coarse
NEW_PROP_TAG(TwoPTwoCTraits);

//! Set the default type for scalar values to double
SET_PROP_DEFAULT(Scalar)
{ typedef double   type; };

//! Use the leaf grid view if not defined otherwhise
SET_PROP_DEFAULT(GridView)
{
private:
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(Grid)) Grid;

public:
    typedef typename Grid::LeafGridView type; 
};

//! Use GenericReferenceElements by default
SET_PROP_DEFAULT(ReferenceElements)
{
private:
    typedef typename GET_PROP_TYPE(TypeTag, PTAG(Grid)) Grid;

    typedef typename Grid::ctype CoordScalar;
    static const int dim = Grid::dimension;

public:
    typedef Dune::GenericReferenceElements<CoordScalar, dim> ReferenceElements; 
    typedef Dune::GenericReferenceElement<CoordScalar, dim>  ReferenceElement; 
};
}
}

#endif
