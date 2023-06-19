# POP3
Este repositorio implementa un servidor de mails POP3 que sigue el RFC 1939. Al mismo tiempo, incluye y define un cliente que puede monitorear el servidor. El protocolo de monitoreo es definido por nosotros, su definicion se encuentra en el archivo monitor-rfc.txt

## Documentacion
Para mas informacion sobre la implementacion del servidor y del cliente, asi como la performance con diferentes configuraciones, se puede referir al directorio `/docs` donde se encuentra el informe de este proyecto `informe.pdf` y el RFC del protocolo monitor.

## Generacion de binarios
Parado en la raiz del repositorio para compilar tanto el monitor como el cliente se ejecuta el comando

```
make all
```

Asimismo se pueden eliminar todos los binarios con el comando

```
make clean
```

## Ejecucion del cliente
Parado en la raiz del repositorio se puede compilar el cliente con el comando

```
make client
```

Luego para ejecutar el cliente simplemente es correr el comando

```
bin/client -a ipServer -p portServer -u user:pass 
```
donde `-a ipServer` especifica la direccion ip del servidor que soporta el protocolo monitor, `-p portServer ` especifica el puerto del servidor que soporta el protocolo monitor, `-u user:pass` especifica el usuario **administrador** que ya existe dentro del servidor para poder ingresar como tal. Una vez logeado el usuario puede interactuar con el servidor con los comandos estipulados en el rfc dentro de `docs/monitor.txt`

Otra manera de ejecutar el cliente es directamente por argumentos de ejecucion. Por ejemplo si unicamente se quiere agregar un nuevo usuario pop3 al servidor se ejecutaria, 
```
bin/client -n new-user:new-user-pass -p portServer -u user:pass 
```
Analogamente si se desease ver las metrics se ejecutaria 
```
bin/client -m metric-id -p portServer -u user:pass 
```
donde metric-id es el identificador de la metrics en en rfc del monitor. Si no se incluye el `metric-id` entonces se imprimen todas las metricas (identificadas por su metric-id)

## Ejecucion del servidor
Parado en la raiz del repositorio se puede compilar el servidor con el comando

```
make client
```

Luego para ejecutar el servidor se puede ejecutar

```
bin/server -u user:pass  -u user2:pass2 -a admin:admin-pass -a admin2:admin-pass2 -d mails/
```

donde el `-u user/pass` define el usuario con nombre '_user_' y contrasenia '_pass_', y `-a admin:admin-pass` define el usuario administrador con nombre '_admin_' y contrasenia '_admin-pass_'. Por ultimo el argumento `-d mails/` viene a ser el path donde se encuentran los mails que va a administrar el servidor. Como lo estipula el RFC 1939 dentro del directorio _mails_ por cada usuario del servidor se debe crear un subdirectorio con el nombre de usuario y dentro de este otro directorio con nombre curl, es decir: para cada **user** dentro de mails  _mails/**user**/curl_.

Notar que se puede agregar mas de un usuario al iniciar el servidor (se pueden agregar en ejecucion a traves del cliente monitor), simplemente encadenando `-u userN:passN` por cada uno. El maximo de administradores es 10 y de clientes es 10. Por ultimo notar que `-d mails**/**` termina con un **/** sino no funciona.
