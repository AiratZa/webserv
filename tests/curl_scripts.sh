#!/bin/bash

function pause(){
   read -p "$*"
}

#.././webserv python_tests/ui_tests/WEBSERV.CONF
curl --resolve first.com:8080:127.0.0.1 http://first.com:8080/ -v
pause 'Press [Enter] key to continue...'

curl --resolve second.com:8080:127.0.0.1 http://second.com:8080/ -v
pause 'Press [Enter] key to continue...'