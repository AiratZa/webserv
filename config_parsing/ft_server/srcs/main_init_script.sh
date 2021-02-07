#!/bin/bash

# Getting phpMyAdmin, unzip, moving and setting config
# wget https://files.phpmyadmin.net/phpMyAdmin/5.0.2/phpMyAdmin-5.0.2-all-languages.zip 
# unzip phpMyAdmin-5.0.2-all-languages.zip && rm -rf phpMyAdmin-5.0.2-all-languages.zip
# mv phpMyAdmin-5.0.2-all-languages phpmyadmin && rm -rf phpmyadmin/config.sample.inc.php
# mv /srcs/config.inc.php phpmyadmin/ && mv phpmyadmin /var/www/ && rm -rf /var/www/html

# MySQL DB initial settings
# service mysql start && mysql < /srcs/db_init.sql

# Getting the last version of WP, unzip, moving and setting config
# curl -O https://wordpress.org/latest.tar.gz 
# tar -xzf latest.tar.gz && rm -rf latest.tar.gz
# mv wordpress/ /var/www/
# mv /srcs/wp-config.php /var/www/wordpress


# Generaing new certificate and new key
# openssl req -x509 -nodes -days 365 -newkey\
        # rsa:2048 -keyout my_site.key \
        # -out my_site.crt \
        # -subj "/C=RF/ST=RT/L=Kazan/O=21_SCHOOL/OU=Erehwon/CN=my_site"

# mkdir /etc/nginx/ssl && mv my_site.key my_site.crt /etc/nginx/ssl

# Nginx conf setting (default: autoindex on)
cp /srcs/webserv.conf /etc/nginx/sites-available/default

# Setting rights to www/ dir
chown -R www-data /var/www/*

find /var/www/ -type d -exec chmod 755 {} \;
find /var/www/ -type f -exec chmod 644 {} \;