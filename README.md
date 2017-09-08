## **Trabajo Fin de Grado Ingeniería Informática (UGR)** ##
**Breve Descripción/Introducción:**
El proyecto fue propuesto desde el primer momento como una recopila-
ción bibliográfica de herramientas para la docencia universitaria en Inteligen-
cia Artificial. 
El problema que encontramos es que no hay una recopilación
actualizada de applets que se usan para la enseñanza de la IA. La mayoria de
los enlaces están obsoletos por lo que decidimos crear una página actualizada
para que cualquier usuario pueda consultarla. En principio, lo más sencillo
serı́a comprobar página por página para ver si tiene contenido relevante o
si el enlace está roto entre otras, pero lógicamente, se trata de un trabajo
pesado y tedioso. Por lo tanto, decidimos automatizar la búsqueda y crear
un programa el cual decida si las páginas contienen información relevante a
partir de unas palabras clave que dicho programa va autoaprendiendo según
explora más páginas. Para hacerlo más extenso, y no solo buscar applets, el
programa encuentra las páginas relacionadas con la IA y las cataloga según
la palabra clave que ha encontrado en ella.
El resultado se muestra en una página web sencilla que utiliza php.

##Manual de Uso
Para compilarlo debemos ejecutar dentro del directorio del
programa el siguiente comando:

```
g ++ - std = c ++11 -o buscar main . cpp pagina . cpp
```
Para ejecutarlo tenemos dos opciones:
- A) Lanzar el ejecutable creado pasándole de argumento la URL de
la página que queremos que empiece a analizar. De esta manera solo
cargará las palabras clave que tenı́a almacenadas previamente(si ya se
habı́a ejecutado anteriormente), creando desde cero los demás archivos
y la página web con los resultados.

```
./ buscar http://www.ejemplo.com
```

- B) Lanzar el ejecutable creado sin pasarle ningun argumento. Esta
opción cargará la información que tienen todos los archivos. Se usará
cuando:
• Necesitemos que el programa continúe analizando por donde se
quedó antes de detenerse sin perder información.
• Si queremos que cargue una serie de páginas determinadas al
comienzo. Para ello, copiar los enlaces de dichas páginas en el
archivo “abiertos.txt” para analizarlas.

```
./ buscar
```
Una vez ejecutado el programa, este irá analizando los diferentes nodos y
mostrando en la terminal la información que está recogiendo.
![visualizacion](http://i65.tinypic.com/2dkj0p5.png)
