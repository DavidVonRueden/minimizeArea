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

Application
    minimizeArea

Description

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "volFields.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createFields.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


    // const volVectorField cell_centers_boundaries = mesh.C();
    label patchID = mesh.boundaryMesh().findIndex("pipe");
    const polyPatch& cPatch = mesh.boundaryMesh()[patchID]; 
    const vectorField cell_centers_boundaries = cPatch.faceCellCentres();
    auto cell_id_boundary = cPatch.faceCells();
    while(runTime.loop())
    {   
        forAll(cPatch, boundaryI)
        {   
            // Info << cell_id_boundary[boundaryI] << endl;
            z[cell_id_boundary[boundaryI]] = Foam::sin(2*Foam::constant::mathematical::pi*(cell_centers_boundaries[boundaryI].x() + cell_centers_boundaries[boundaryI].y()) + runTime.value());
            Info << z[cell_id_boundary[boundaryI]] << endl;
            // Info << "X: " << cell_centers_boundaries[boundaryI].x()<< endl;
            // z.boundaryField()[boundaryI] = Foam::sin(2*Foam::constant::mathematical::pi*(cell_centers_boundaries[boundaryI].x() + cell_centers_boundaries[boundaryI].y()));
            // Info <<z.boundaryField()[boundaryI] << endl;
        }
        
        // z.boundaryField()[patchID] = sin(2*Foam::constant::mathematical::pi*(cell_centers.boundaryField()[patchID].x() + cell_centers.boundaryField()[patchID].y() + runTime.time()));
        // forAll(boundary_cells, boundaryI)
        // {
        //     z.boundaryField()[boundaryI] = sin(2*Foam::constant::mathematical::pi*(cell_centers.boundaryField()[boundaryI].x() + cell_centers.boundaryField()[boundaryI].y() + runTime.time()));
        // }
        // // forAll(cell_centers, cellI)
        // // {
        // //     z[cellI] = Foam::sqrt(sqr(cell_centers[cellI].x()) + sqr(cell_centers[cellI].y()));
        // // }

        runTime.write();
        // Info<< nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        //     << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        //     << nl << endl;

    }

    // Info << cell_centers << endl;
    return 0;
}


// ************************************************************************* //
