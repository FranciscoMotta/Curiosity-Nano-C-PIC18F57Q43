El siguiente proyecto trata acerca del desarrollo de un código
que permite leer un matriz de pulsadores 4x4 mediante el driver
74LS922 el cual codifica los botones pulsados a un numero de 4 bits.

A diferencia de proyecto anterior, bt_matrix.X, este proyecto será 
más simple en la lectura de la matriz de pulsadores, debido a que el driver
74LS922 facilita su uso a los desarrolladores, ya que reducimos el 
número de pines de 8 a 5, además, este código no será bloqueante en 
su funcionamiento. 

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
   ----------------------------
   |          74LS922         | 
   |                          |  
   ----------------------------
              | | | | |
              | | | | | 
   -----------------------------|
   |---|      PIC18F57Q43       | 
   |---|    Curiosity Nano      | 
   -----------------------------|
   
   Pinout:
C1: 3   C2: 5   C3: 6    C4: 7
F1: 8   F2: 1   F3: 2    F4: 4

Leyenda: 
- Cx: Columna X
- Fx: Fila X
              
