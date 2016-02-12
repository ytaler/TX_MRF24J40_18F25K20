ECG Inalámbrico.
Se utiliza el front end ADS 119x de texas instruments (tema aparte, pero un desastre la hoja de datos..) para la adquisición de las señales.
Para la transmisión de datos se utiliza MRF24J40MA de microchip.
Para el control se utiliza un pic 18F25K20.

Se mide:

* Canal 1: Lead I = LA-RA.
* Canal 2: Lead II = LL-RA.

Se calcula por software:

* Lead III = Lead II - Lead I = LL-LA
* aVR =  - ( Lead I + Lead II ) / 2
* aVL = Lead I - ( Lead II / 2 )
* aVF = Lead II - ( Lead I / 2 )
