# POP3

Este repositorio implementa un servidor de mails POP3 que sigue el RFC 1939. Al mismo tiempo, incluye y define un cliente que puede monitorear el servidor. El protocolo de monitoreo es definido por nosotros, su definicion se encuentra en el archivo monitor-rfc.txt

## Generacion de binarios

Parado en la raiz del repositorio para compilar tanto el monitor como el cliente se ejecuta el comando

```
make all
```

Asimismo se pueden eliminar todos los binarios con el comando

```
make clean
```

##Ejecucion del cliente

Parado en la raiz del repositorio se puede compilar el cliente con el comando

```
make client
```

Luego para ejecutar el cliente simplemente es correr el comando

```
bin/client -
```

##Ejecucion del servidor
Parado en la raiz del repositorio se puede compilar el servidor con el comando

```
make client
```

Luego para ejecutar el servidor se puede ejecutar

```
bin/server -u user:pass -a admin:admin-pass -d mails/

```

donde el `-u user/pass` define el usuario con nombre 'user' y contrasenia 'pass', y `-a admin:admin-pass` define el usuario administrador con nombre 'admin' y contrasenia 'admin-pass'. Por ultimo el argumento `-d mails/` viene a ser el path donde se encuentran los mails que va a administrar el servidor.
