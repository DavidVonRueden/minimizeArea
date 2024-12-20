/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2024 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "sinBCFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sinBCFvPatchScalarField::
sinBCFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF),
    scalarData_(dict.lookup<scalar>("scalarData", unitAny)),
    data_(dict.lookup<scalar>("data")),
    fieldData_("fieldData", iF.dimensions(), dict, p.size()),
    timeVsData_
    (
        Function1<scalar>::New
        (
            "timeVsData",
            db().time().userUnits(),
            unitAny,
            dict
        )
    ),
    wordData_(dict.lookupOrDefault<word>("wordName", "wordDefault")),
    labelData_(-1),
    boolData_(false)
{


    fixedValueFvPatchScalarField::evaluate();

    /*
    // Initialise with the value entry if evaluation is not possible
    fvPatchScalarField::operator=
    (
        scalarField("value", iF.dimensions(), dict, p.size())
    );
    */
}


Foam::sinBCFvPatchScalarField::
sinBCFvPatchScalarField
(
    const sinBCFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    scalarData_(ptf.scalarData_),
    data_(ptf.data_),
    fieldData_(mapper(ptf.fieldData_)),
    timeVsData_(ptf.timeVsData_, false),
    wordData_(ptf.wordData_),
    labelData_(-1),
    boolData_(ptf.boolData_)
{}


Foam::sinBCFvPatchScalarField::
sinBCFvPatchScalarField
(
    const sinBCFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(ptf, iF),
    scalarData_(ptf.scalarData_),
    data_(ptf.data_),
    fieldData_(ptf.fieldData_),
    timeVsData_(ptf.timeVsData_, false),
    wordData_(ptf.wordData_),
    labelData_(-1),
    boolData_(ptf.boolData_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::sinBCFvPatchScalarField::map
(
    const fvPatchScalarField& ptf,
    const fieldMapper& mapper
)
{
    fixedValueFvPatchScalarField::map(ptf, mapper);

    const sinBCFvPatchScalarField& tiptf =
        refCast<const sinBCFvPatchScalarField>(ptf);

    mapper(fieldData_, tiptf.fieldData_);
}


void Foam::sinBCFvPatchScalarField::reset
(
    const fvPatchScalarField& ptf
)
{
    fixedValueFvPatchScalarField::reset(ptf);

    const sinBCFvPatchScalarField& tiptf =
        refCast<const sinBCFvPatchScalarField>(ptf);

    fieldData_.reset(tiptf.fieldData_);
}


void Foam::sinBCFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    fixedValueFvPatchScalarField::operator==
    (
        data_
      + fieldData_
      + scalarData_*timeVsData_->value(db().time().value())
    );


    fixedValueFvPatchScalarField::updateCoeffs();
}


void Foam::sinBCFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchScalarField::write(os);
    writeEntry(os, "scalarData", scalarData_);
    writeEntry(os, "data", data_);
    writeEntry(os, "fieldData", fieldData_);
    writeEntry(os, db().time().userUnits(), unitAny, timeVsData_());
    writeEntry(os, "wordData", wordData_);
    writeEntry(os, "value", *this);
}


// * * * * * * * * * * * * * * Build Macro Function  * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        sinBCFvPatchScalarField
    );
}

// ************************************************************************* //
