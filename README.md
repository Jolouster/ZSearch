Es una librería para implementar las búsquedas en mis notas Zettelkasten.

Se le pasa un string y determina si en éste existe o no etiquetas. Luego busca en:

- Nombres de los archivos (ignorando el ID único basado en fecha y hora de creación de la nota, 12 cifras numéricas seguidas).
  Cuantas más palabras coincidan mayor peso tendrá esta nota en el listado final. El peso será mayor si las palabras están en el mismo orden.
- Etiquetas de las notas.
  Si hay más de una etiqueta, la nota que contenga el mayor número de etiquetas coincidentes tendrá mayor peso.
- Contenido.
  Serán las que menor peso contengan. Los archivos que contengan las mismas palabras buscadas en el mismo orden tendrán mayor peso.


