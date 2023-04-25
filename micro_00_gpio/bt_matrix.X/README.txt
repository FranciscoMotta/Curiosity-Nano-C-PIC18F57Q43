El presente proyecto trata acerca de leer una matriz de pulsadores
sin usar el driver 74LC922, la forma en la que se implementó el código es 
bloqueante, es decir, tiene que esperar a que se pulse un boton de la matriz
para que el flujo del programa continue con su proceso.

-----------------------------------------
|    1    |     2    |    3    |    A    |
-----------------------------------------
|    4    |     5    |    6    |    B    |
-----------------------------------------
|    7    |     8    |    9    |    C    |
-----------------------------------------
|    *    |     0    |    #    |    D    |
-----------------------------------------
          | | | | | | | |  <- pines físicos de mi matriz de botones
          | | | | | | | | 
          1 2 3 4 5 6 7 8 
          
Pinout:
C1: 3   C2: 5   C3: 6    C4: 7
F1: 8   F2: 1   F3: 2    F4: 4

Leyenda: 
- Cx: Columna X
- Fx: Fila X

