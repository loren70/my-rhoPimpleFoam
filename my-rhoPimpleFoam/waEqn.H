 
//NOTE: passive scalar equation "wa" (mass fraction).
        
        fvScalarMatrix waEqn
        (
            fvm::ddt(rho,wa)
          + fvm::div(phi,wa)
          ==
            fvc::laplacian(rho,Db*wa)
          + fvc::laplacian(rho*wa,DTa*thermo.T())
        );
        
        waEqn.relax();
        waEqn.solve();
