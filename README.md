# Laboratorio de Sistope

## Lab1:
### I. Objetivos Generales

Este laboratorio tiene como objetivo refrescar los conocimientos de programacion imperativa, mediante
la construccion de una aplicaci ́on simple de procesamiento de imagenes.  La aplicacion debe ser escrita en
lenguaje de programacion C sobre sistema operativo Linux.  Este laboratorio servira de base a los siguientes
laboratorios, los cuales iran paulatinamente incluyendo el uso de servicios del sistema operativo para realizar
la misma funcion que la aplicacion orginal.  Es por eso que es de vital importancia que el diseño y modu-
larizacion de esta aplicacion quede bien definida a partir del lab1, para que luego solo nos preocupemos de
incluir las nuevas funcionalidades asociadas a procesos, hebras y quiza memoria compartida.
### II.    Objetivos Especıficos

1.  Conocer y usar las funcionalidades de
getopt()
como metodo de recepcion de parametros de entradas.
2.  Construir funciones de lectura y escritura de archivos binarios usando
open()
,
read()
, y
write()
.
3.  Construir funciones de procesamiento de imagenes
4.  Practicar tecnicas de documentacion de programas
5.  Conocer y practicar uso de makefile para compilacion de programas.
### III. Desarrollo

La aplicacion consiste en un
pipeline
de procesamiento de imagenes astronomicas.  Cada imagen pasara
por tres etapas de procesamiento tal que al final del
pipeline
se clasifique la imagen como satisfaciendo o no
alguna condicion a definir.  El programa procesara varias imagenes, una a la vez.
Las etapas del
pipeline
son:
1.  Lectura de imagen RGB
2.  Conversi ́on a imagen en escala de grises
3.  Binarizaci ́on de imagen
4.  An ́alisis de propiedad
5.  Escritura de resultados
