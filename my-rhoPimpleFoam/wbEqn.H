 
//NOTE: passive scalar equation "wb" (mass fraction).
        fvScalarMatrix wbEqn
        (
            fvm::ddt(rho,wb)
          + fvm::div(phi,wb)
          ==
            fvc::laplacian(rho,Db*wb)
          + fvc::laplacian(rho*wb,DTb*thermo.T())
        );
        
        wbEqn.relax();
        wbEqn.solve(); 
