./webserv config_parsing/test_configs/1.conf
./webserv "/home/airat/CLionProjects/webserv/config_parsing/test_configs/2.conf"

https://nginx.org/ru/docs/http/ngx_http_core_module.html#server_name

//TODO
// 1. ERROR_PAGE +-
// 2. LOCATION ++
// Если ошибочный ответ обрабатывается проксированным сервером или FastCGI/uwsgi/SCGI/gRPC-сервером,
// и этот сервер может вернуть разные коды ответов, например, 200, 302, 401 или 404, то можно выдавать возвращаемый им код:
// 3. INDEX ++
// 4. LIMIT_EXCEPT++
// 5. client_max_body_size++
// 6. alias
// 7. AUTOINDEX
// 8. MAYBE root



//!!! DONE: LISTEN DIRECTIVE
//listen - map<host, list<port> >

//!!! DONE: SERVER_NAME DIRECTIVE
// server_name LIST

//!!! DONE ERROR_PAGE
// //Директивы наследуются с предыдущего уровня конфигурации при условии, что на данном уровне не описаны свои директивы error_page.
//!!! DONE 3. ERROR PAGE (may be inside location and server)
//!!! DONE error_page код ... [=[ответ]] uri;

//!!! DONE Для 1 кода используется только первая установка
//!!! DONE         установки могут быть на сколько угодно строках

//!!! DONE value "600" must be between 300 and 599 in /etc/nginx/nginx.conf:38

//!!! DONE ЕСЛИ ВНУТРИ LOCATION ЕСТЬ ПЕРЕОПРДЕЛЕНИЕ ЛЮБОЙ ОШИБКИ, ТО ВСЕ ВНЕШНИЙ ПЕРЕОПРЕДЕЛЕНИЯ УЖЕ НЕ ИДУТ В СЧЕТ



//!!! DONE LOCATION

// location
//
//        modificators https://www.journaldev.com/26342/nginx-location-directive
//none: If no modifiers are present in a location block then the requested URI
//        will be matched against the beginning of the requested URI.
//=: The equal sign is used to match a location block exactly against a requested URI.
//
//
//location / {}
//
//location / {}
//
//duplicate location "/" in /etc/nginx/nginx.conf:38
//
//!!!!!
//BUT THIS IS OK
//
//location =/ {}
//
//location / { }



//11. https://nginx.org/ru/docs/http/ngx_http_index_module.html
//
//МОЖЕТ БЫТЬ НЕСКОЛЬКО, ДОБАВЛЯЮТСЯ ДРУГ ЗА ДРУГОМ, НЕ ПЕРЕЗАПИСЫВАЕТСЯ НИЧЕГО
// NGINX старается найти соответствующий файл и без слеша на конце и если что редиректит с кодом  301
//Модуль ngx_http_index_module обслуживает запросы, оканчивающиеся слэшом (‘/’). Такие запросы также могут обслуживаться модулями ngx_http_autoindex_module и ngx_http_random_index_module.
//
//Пример конфигурации
//location / {
//index index.$geo.html index.html;
//}
//Директивы
//        Синтаксис:	index файл ...;
//Умолчание:
//index index.html;
//Контекст:	http, server, location
//        Определяет файлы, которые будут использоваться в качестве индекса. В имени файла можно использовать переменные. Наличие файлов проверяется в порядке их перечисления. В конце списка может стоять файл с абсолютным путём. Пример:
//
//index index.$geo.html index.0.html /index.html;
//Необходимо иметь в виду, что при использовании индексного файла делается внутреннее перенаправление и запрос может быть обработан уже в другом location’е. Например, в такой конфигурации:
//
//location = / {
//index index.html;
//}
//
//location / {
//...
//}
//запрос “/” будет фактически обработан во втором location’е как “/index.html”.



//8.Синтаксис:	limit_except метод ... { ... }
//Умолчание:	—
//Контекст:	location
//        Ограничивает HTTP-методы, доступные внутри location. Параметр метод может быть одним из GET, HEAD,
//POST, PUT, DELETE, MKCOL, COPY, MOVE, OPTIONS, PROPFIND, PROPPATCH, LOCK, UNLOCK или PATCH.
//Если разрешён метод GET, то метод HEAD также будет разрешён.
//
//"limit_except" directive is duplicate in /etc/nginx/nginx.conf:114
//
//БЕЗ ДУБЛЕЙ
//
//limit_except GET {
//        allow 192.168.1.0/32;
//        deny  all;
//}
//
//< HTTP/1.1 403 Forbidden
//        < Server: nginx/1.18.0
//< Date: Sun, 07 Feb 2021 19:53:49 GMT
//        < Content-Type: text/html
//        < Content-Length: 153
//< Connection: keep-alive
//        <
//                <html>
//                <head><title>403 Forbidden</title></head>
//<body>
//<center><h1>403 Forbidden</h1></center>
//<hr><center>nginx/1.18.0</center>
//</body>
//</html>
//


// invalid number of arguments in "limit_except"
//
//            limit_except  {
//                deny all;
//            }


________

//THIS IS OK
//            limit_except  POST POST {
//                deny all;
//            }

____________-

//invalid method "TRACE"
//
//    limit_except  TRACE POST {
//        deny all;
//    }


/*
5. 	client_max_body_size

Синтаксис:	client_max_body_size размер;
Умолчание:
client_max_body_size 1m;
Контекст:	http, server, location
Задаёт максимально допустимый размер тела запроса клиента, указываемый в поле “Content-Length” заголовка запроса.
Если размер больше заданного, то клиенту возвращается ошибка 413 (Request Entity Too Large).
Следует иметь в виду, что браузеры не умеют корректно показывать эту ошибку. Установка параметра размер в 0 отключает проверку размера тела запроса клиента.

 MAX VALUE = 999999999999999999 (999 999 999 999 999 999)

7. measure units http://nginx.org/en/docs/syntax.html
 Sizes can be specified in bytes, kilobytes (suffixes k and K) or megabytes (suffixes m and M), for example, “1024”, “8k”, “1m”.

Offsets may be also specified in gigabytes using g or G suffixes.
bytes just like digit


"client_max_body_size" directive is duplicate in /etc/nginx/nginx.conf:110
CANT BE DUBLICATED

 ---------

 "client_max_body_size" directive invalid value

             client_max_body_size -1;
            client_max_body_size a1;
            client_max_body_size 1a;


 -------------
invalid number of arguments in "client_max_body_size" directive

             client_max_body_size ;

             client_max_body_size 1 m;

 -----------------
  "client_max_body_size" directive is duplicate

              client_max_body_size 1m;
            client_max_body_size 1m;
 */

////==============================================================






/*
1. server may be empty but it will do nothing

2. SET DEFAULT FOR FIRST

9.

Синтаксис:	alias путь;
Умолчание:	—
Контекст:	location
Задаёт замену для указанного location’а. Например, при такой конфигурации

location /i/ {
    alias /data/w3/images/;
}
на запрос “/i/top.gif” будет отдан файл /data/w3/images/top.gif.

"alias" directive is duplicate in /etc/nginx/nginx.conf:78
БЕЗ ДУБЛЕЙ


 -------

 invalid number of arguments in "alias" directive

             alias ;
             alias / /;

---------

 OK
 alias /;

 ------------




10.

Синтаксис:	autoindex on | off;
Умолчание:
autoindex off;
Контекст:	http, server, location
Разрешает или запрещает вывод листинга каталога.

"autoindex" directive is duplicate in /etc/nginx/nginx.conf:79



