FROM debian:buster

MAINTAINER gdrake <gdrake@student.42.fr>

EXPOSE 80 443

RUN apt-get update
# Installing nginx
RUN apt-get install -y nginx
# Installing MariaDB (MySQL opensource fork) 
# RUN apt-get install -y mariadb-server
# Installing PHP and dependencies
# RUN apt-get install -y php7.3 php7.3-fpm php7.3-mysql php7.3-mbstring
# Necessary tools
RUN apt-get install -y wget unzip curl
# Copying configs and scripts
COPY srcs /srcs

RUN /srcs/main_init_script.sh

CMD bash /srcs/restart_main_services.sh
