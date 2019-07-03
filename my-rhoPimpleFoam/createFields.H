#include "createRDeltaT.H"

Info<< "Reading thermophysical properties\n" << endl;
Info<< "Reading transport properties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

autoPtr<fluidThermo> pThermo
(
    fluidThermo::New(mesh)
);
fluidThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");

volScalarField& p = thermo.p();

volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    thermo.rho()
);



//NOTE: Thermal Diffusivity constant for scalar wa
dimensionedScalar DTa
(
    "DTa",
    transportProperties.lookup("DTa")
);
//NOTE: Thermal Diffusivity constant for scalar wb
dimensionedScalar DTb
(
    "DTb",
    transportProperties.lookup("DTb")
);

//NOTE: Diffusivity constant for scalar wa
dimensionedScalar Da
(
    "Da",
    transportProperties.lookup("Da")
);
//NOTE: Diffusivity constant for scalar wb
dimensionedScalar Db
(
    "Db",
    transportProperties.lookup("Db")
);

//NOTE:Definition of scalar wa
Info<< "Reading field wa\n" << endl;
volScalarField wa
(
    IOobject
    (
        "wa",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

//NOTE:Definition of scalar wb
Info<< "Reading field wb\n" << endl;
volScalarField wb
(
    IOobject
    (
        "wb",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

#include "compressibleCreatePhi.H"

pressureControl pressureControl(p, rho, pimple.dict(), false);

mesh.setFluxRequired(p.name());

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);

Info<< "Creating field dpdt\n" << endl;
volScalarField dpdt
(
    IOobject
    (
        "dpdt",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("dpdt", p.dimensions()/dimTime, 0)
);

Info<< "Creating field kinetic energy K\n" << endl;
volScalarField K("K", 0.5*magSqr(U));

#include "createMRF.H"
#include "createFvOptions.H"
