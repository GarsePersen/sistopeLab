Laboratorio de Sistope

Lab1:
I.    Objetivos Generales
Este laboratorio tiene como objetivo refrescar los conocimientos de programaci ́on imperativa, mediante
la construcci ́on de una aplicaci ́on simple de procesamiento de im ́agenes.  La aplicaci ́on debe ser escrita en
lenguaje de programaci ́on C sobre sistema operativo Linux.  Este laboratorio servir ́a de base a los siguientes
laboratorios, los cuales ir ́an paulatinamente incluyendo el uso de servicios del sistema operativo para realizar
la misma funci ́on que la aplicaci ́on orginal.  Es por eso que es de vital importancia que el dise ̃no y modu-
larizaci ́on de esta aplicaci ́on quede bien definida a partir del lab1, para que luego s ́olo nos preocupemos de
incluir las nuevas funcionalidades asociadas a procesos, hebras y quiz ́a memoria compartida.
II.    Objetivos Espec ́ıficos
1.  Conocer y usar las funcionalidades de
getopt()
como m ́etodo de recepci ́on de par ́ametros de entradas.
2.  Construir funciones de lectura y escritura de archivos binarios usando
open()
,
read()
, y
write()
.
3.  Construir funciones de procesamiento de im ́agenes
4.  Practicar t ́ecnicas de documentaci ́on de programas
5.  Conocer y practicar uso de makefile para compilaci ́on de programas.
III.    Desarrollo
La aplicaci ́on consiste en un
pipeline
de procesamiento de im ́agenes astron ́omicas.  Cada imagen pasar ́a
por tres etapas de procesamiento tal que al final del
pipeline
se clasifique la imagen como satisfaciendo o no
alguna condici ́on a definir.  El programa procesar ́a varias im ́agenes, una a la vez.
Las etapas del
pipeline
son:
1.  Lectura de imagen RGB
2.  Conversi ́on a imagen en escala de grises
3.  Binarizaci ́on de imagen
4.  An ́alisis de propiedad
5.  Escritura de resultados
