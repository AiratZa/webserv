#!/bin/sh
{ echo "POST / HTTP/1.1\nHost: localhost:8080\nUser-Agent: Go-http-client/1.1\nTransfer-Encoding: chunked\nContent-Type: test/file\nAccept-Encoding: gzip\n\n0\n"; sleep 1; echo "0\n"; sleep 1; } | telnet localhost 8080

#{ echo "GET / HTTP/1.1\nHost: localhost:8080\nUser-Agent: Go-http-client/1.1\nTransfer-Encoding: chunked\nContent-Type: test/file\nAccept-Encoding: gzip\n\n0\n\n"; sleep 2; } | telnet localhost 8080
#{ echo "GET / HTTP/1.1\nHost: localhost:8080\n\n"; sleep 1; } | telnet localhost 8080
