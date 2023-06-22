## Resumen

Este documento propone un protocolo para monitorear un servidor POP3. El protocolo define dos estados: AUTHORIZATION y TRANSACTION. El estado de AUTHORIZATION tiene dos comandos, USERNAME y PASSWORD, mientras que el estado de TRANSACTION tiene tres comandos, METRICS, ADD_USER y EXIT.

## Estado de este Memo

Este es un RFC informativo, 

## Descripción general del protocolo

El protocolo para monitorear un servidor POP3 consta de dos estados: AUTHORIZATION y TRANSACTION. El estado inicial es AUTHORIZATION , y el cliente debe autenticarse enviando los comandos USERNAME y PASSWORD. Una vez autenticado, el cliente entra en el estado de TRANSACTION.

En el estado de TRANSACTION, el cliente puede enviar tres comandos: METRICS, ADD_USER y EXIT. El comando METRICS devolverá métricas sobre el uso del servidor. El comando ADD_USER agregará un nuevo usuario al servidor POP3. El comando EXIT finalizará la sesión.

## Operación Básica

Inicialmente, el equipo servidor al iniciar un servicio POP3, habilitara conexiones en otro puerto TCP a definir por la implementación, para aquellos usuarios administradores, que deseen utilizar el siguiente protocolo para administrar el servidor POP3.

De ahora en adelante, se denotara servidor, al proceso de monitoreo del servidor POP3, evitándose confundir servidor con este ultimo.

Cuando la conexión se ha establecido, el servidor PUEDE enviar un saludo. Luego quedara esperando al cliente y estos dos intercambiaran ordenes y respuestas (respectivamente) hasta el cierre o interrupción de la conexión.

Las ordenes en el protocolo monitor POP3 consisten en una serie de palabras claves sin diferenciar mayúsculas de minúsculas, posiblemente seguidas de uno o ningún argumento. Todas las ordenes y sus argumentos estan compuestos de caracteres ASCII y terminaran con un par CRLF (Carriage Return Line Feed). Cada argumento puede tener hasta 40 caracteres.

Las respuestas del monitor están formadas por un indicador de estado, posiblemente seguida de información adicional. Todas las respuestas están terminada por un par CRLF. Los indicadores de estado son el positivo (”+”) y el negativo (”-”). Los servidores PUEDEN enviar información complementarias a los indicadores de estado, dicha informacion no debe exceder los 20 caracteres.

Existen respuestas a ciertas ordenes que son multilínea. En esos casos, tras enviar la primera linea de la respuestas y un LF (Line Feed), irán mas líneas terminadas por un LF. Cuando se han enviado todas las lineas de la respuesta, se envía una linea final que consiste en un par CRLF.

Un servidor DEBE responder a una orden no reconocida, no implementada o no válida sintácticamente, con un indicador de estado negativo. El servidor DEBE responder a una orden enviada en una fase incorrecta con un indicador de estado negativo. 

Un servidor PUEDE tener un contador de inactividad para cerrar la
conexión. Ese contador DEBE de ser de al menos 10 minutos de duración. La recepción de cualquier orden durante el intervalo debería bastar para
reiniciar el contador. Cuando el contador se agota, la sesión TCP debera cerrarse.

## Comandos

### Estado de AUTHORIZATION

### USERNAME

El comando USUARIO se utiliza para identificar al usuario en el servidor. El cliente envía el nombre de usuario como argumento del comando.

```
C: USERNAME {usuario}
S: +
```

Argumentos:

Una cadena que identifica un usuario monitor del servidor POP3. Este usuario no tiene relación con algún usuario POP3 del servidor.

Restricciones:

Solo puede darse en la fase de AUTHORIZATION después de establecerse la conexión con el servidor monitor o tras unos ordenes USERNAME o PASSWORD fallidos.

Comentario:

Para autenticar utilizando la combinación de ordenes USERNAME y PASSWORD, el cliente primero debe enviar la orden USERNAME con un indicador de estado positivo ("+"), entonces el cliente puede enviar tanto la orden PASSWORD para completar la autenticación. Si el servidor POP3 responde con un indicador de estado negativo ("-") la orden USERNAME, entonces el cliente puede tanto enviar una nueva orden de autenticación.

El servidor monitor debe devolver una respuesta positiva solo en caso de que exista el usuario monitor en el servidor.

Respuestas posibles:
+
-

Ejemplos:
C: USERNAME admin
S: +
...
C: USERNAME notadmin
S: -

### PASSWORD

El comando PASSWORD se utiliza para autenticar al usuario en el servidor. El cliente envía la contraseña como argumento del comando.

```
C: PASSWORD {contraseña}
S: +
```

Argumentos:
Una clave específica que de acceso a cierto usuario.

Restricciones:

Solo puede darse en la fase AUTHORIZATION, después de una orden USERNAME con éxito.

Comentario:

Cuando el cliente envía la orden PASSWORD, el servidor monitor utiliza el par de argumentos de las órdenes USER y PASS para determinar si al cliente debería proporcionársele el acceso servidor monitor. El servidor monitor puede tratar los espacios en el argumento como parte de la contraseña en lugar de como separador de argumentos.

Respuestas posibles:
+
-
-

Ejemplos:
C: USERNAME admin
S: +
C: PASSWORD secret
S: -
...
C: USERNAME admin
S: +
C: PASSWORD secret
S: +

### Estado de TRANSACTION

### METRICS

El comando METRICS se utiliza para recuperar métricas sobre el uso del servidor.

```
C: METRICS (n)
S: + 
S: {métricas}
```

Argumentos: Puede recibir OPCIONALMENTE el numero de la metrica que se quiere pedir. En el caso que no se ingrese el numero de metrica se imprimiran todas.

Restricciones: Solo puede darse en la fase TRANSACTION.

Comentario:

El servidor monitor enviara una respuesta multilínea positiva, incluyendo un listado de las métricas del servidor POP3 monitoreado. 

Los valores de las métricas están codificados con un numero entero positivo de la siguiente manera:

1: Cantidad de conexiones actuales

2: Cantidad de mensajes obtenidos

3: Cantidad de mensajes eliminados

4: Cantidad de conexiones concurrentes

5: Cantidad de conexiones historias

6: Cantidad de bytes transferidos

Respuesta posibles:

+

1 n

2 n

3 n

4 n

5 n 

6 n

…

+

n

…

-

Donde n representa el valor de una métrica en cada caso. El primer caso es una respuesta multilínea, donde se muestran todas las métricas. Y el segundo, una respuesta también multilínea, donde se muestra el valor para la métrica solicitada.

Ejemplo:

C: METRICS

S: +

S: 1 6

S: 2 120

S: 3 60

S: 4 2

S: 5 8

S: 6 54030

…

C: METRICS 2

S: 15

## CHANGE_DIRECTORY
El comando CHANGE_DIRECTORY modificara el repositorio donde el servidor POP3 buscara los correos de cada usuario.

```
C: CHANGE_DIRECTORY <path>
S: +
```
Argumentos: Path al directorio que contiene los correos de los usuarios

Restricciones: Solo se puede utilizar en el estado TRANSACTION

Comentario: El monitor retornara un mensaje con estado postivo en el caso de poder modificar dicho directorio en el servidor POP3. El servidor PUEDE retornar un estado de error si el servidor POP3 no pudo utilizar el path otorgado.

Respuestas posibles:
+
-

Ejemplos:

C: CHANGE_DIRECTORY /root/emailDirectory

S: +


## CAPA

El comando CAPA retorna una lista de las capacidades soportadas por el servidor monitos. Estará solo disponible en el estado TRANSACTION.

```
C: CAPA
S: +
```

El servidor PUEDE listar comandos adicionales en caso de que los haya implementado. Adicionalmente, puede listar informacion adicional sobre el servidor.

El servidor PUEDE retornar un mensaje con estado positivo sin ninguna información adicional, solamente en el caso en el que no desee listar información adicional sobre el servidor y no haya implementado ningun comando por fuera del protocolo.

Respuestas posibles:

+

Ejemplos:

C: CAPA

S: +

…

C: CAPA
S: +
S: ADD_USER
S: LIST_USERS
S: …

## LIST_USERS

El comando LIST_USERS se utiliza para mostrar un listado de todos los usuarios del servidor POP3.

```
C: LIST_USERS
S: +
S: <usuario1>
S: <usuario2>
...
```

Argumentos: Ninguno

Restricciones: Solo se puede utilizar en el estado TRANSACTION

Comentario: El monitor retornara un listado de los usuarios creados en el servidor POP3.

Respuestas posibles:

+

Ejemplos:

C: LIST_USERS

S: +

S: <usuario1>

S: <usuario2>

S: <usuario3>

S: …

### ADD_USER

El comando ADD_USER se utiliza para agregar un nuevo usuario al servidor POP3.

```
C: ADD_USER {usuario}:{contraseña}
S: +
```

Argumentos: Una unica cadena separada por ‘:’ donde la primera parte representa el nuevo nombre de usuario y la segunda parte (después del ‘:’) representa su respectiva contraseña. 

Restricciones: Solo se puede utilizar en el estado TRANSACTION.

Comentario:

El monitor tomara la cadena recibida como argumento, para separar la parte previa del ‘:’ como nombre de usuario y la posterior como contraseña, para crear un nuevo usuario en el servidor POP3. Este el servidor monitor PUEDE validar que dicho usuario ya exista, retornando un mensaje de error en dicho caso.

En caso de no tener conflictos con el argumento recibido y se pudo agregar el nuevo usuario al servidor POP3, se debe retornar un mensaje positivo, de caso contrario se debe informar el error.

Respuestas posibles:

+ user added

-ERR could not add user

Ejemplos:

C: ADD_USER name:pass

S: - could not add user

…

C: ADD_USER name:pass
S: + user added

### EXIT

El comando EXIT se utiliza para finalizar la sesión.

```
C: EXIT
S: +
```

Argumentos: Ninguno

Restricciones: Ninguna

Comentario: El servidor monitor cerrara la conexión con el cliente.

Respuestas posibles:

+

-

Ejemplos:

C: exit

S: +

## Consideraciones de seguridad

Este protocolo requiere el uso de nombres de usuario y contraseñas para autenticar a los usuarios. Se recomienda que los servidores implementen mecanismos de autenticación seguros, como SSL / TLS, para protegerse contra la escucha y los ataques de intermediarios.
