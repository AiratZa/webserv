import pytest
import requests
import time

test_links = ["http://localhost:8080",
              "http://localhost:8081"
              ]

def test_multiple_servers(webserv, browser):
    for link in test_links:
        browser.get(link)
        r = requests.get(link)

        assert r.status_code == 200
        assert str(r.content).find(link) > 0
        time.sleep(1)

# def test_multiple_servers(webserv, browser):
#     for link in test_links:
#         browser.get(link)
#         r = requests.get(link)
#
#         assert r.status_code == 200
#         assert str(r.content).find(link) > 0
#         time.sleep(1)
