#!/bin/bash

# Function to generate ADD_USER commands
generate_add_user_commands() {
  for ((i=1; i<=500; i++)); do
    echo "ADD_USER user$i:1234"
    mkdir "/home/laucha/testProtosUsers/user$i"
  done
}

# Prepare the command sequence
commands="username admin\r\npassword admin\r\n$(generate_add_user_commands)\r\n"

# Execute the commands using nc
echo -e "$commands" | nc -w 5 -C 127.0.0.1 8200

for ((i=1; i<=500; i++)); do
    echo “USER user$i\r\nPASS pass\r\nLIST\r\n | nc localhost 8114 &
done