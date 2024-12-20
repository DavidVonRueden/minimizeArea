Info<< "Reading field z\n" << endl;
volScalarField z
(
    IOobject
    (
        "z",
        runTime.name(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field grad_z\n" << endl;
volVectorField grad_z
(
    IOobject
    (
        "grad_z",
        runTime.name(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);