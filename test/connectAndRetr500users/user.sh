#!/bin/bash

### NOTA: hay que modificar el path de los mails o  
### borrar el archivo pop3/commands/server/authorization/pass.c borrar la linea 40:
### strcat(path, "/curl/");

# Function to generate ADD_USER commands
generate_add_user_commands() {
  for ((i=1; i<=500; i++)); do
   echo "ADD_USER user$i:1234"
    mkdir "/home/laucha/testProtosUsers/user$i"
    dd if=/dev/urandom of="/home/laucha/testProtosUsers/user$i/email" bs=4096 count=25600
  done
}

# Prepare the command sequence
commands="username admin\r\npassword admin\r\n$(generate_add_user_commands)\r\n"

# Execute the commands using nc
echo -e "$commands" | nc -w 5 -C 127.0.0.1 8200

for ((i=1; i<=500; i++)); do
    printf "USER user$i\r\nPASS 1234\r\nRETR 1\r\n" | nc localhost 8114 &
done
