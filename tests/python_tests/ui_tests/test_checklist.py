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

def test_error_page_info(webserv, browser):
    # customized error_page info
    link = test_links[0] + "/" + "a";
    browser.get(link)
    r = requests.get(link)
    assert r.status_code == 404
    assert str(r.content).find("404.png") > 0
    time.sleep(1)

    # default error_page info
    link = test_links[1] + "/" + "a";
    browser.get(link)
    r = requests.get(link)
    assert r.status_code == 404
    assert str(r.content).find("404.png") == -1
    time.sleep(1)

def test_error_page_info(webserv, browser):
    # customized error_page info
    link = test_links[0] + "/" + "a";
    browser.get(link)
    r = requests.get(link)
    assert r.status_code == 404
    assert str(r.content).find("404.png") > 0
    time.sleep(1)

    # default error_page info
    link = test_links[1] + "/" + "a";
    browser.get(link)
    r = requests.get(link)
    assert r.status_code == 404
    assert str(r.content).find("404.png") == -1
    time.sleep(1)


def test_limit_except(webserv, browser):
    # not_limit_except
    link = test_links[0]
    browser.get(link)

    r = requests.get(link)
    assert r.status_code != 405

    r = requests.head(link)
    assert r.status_code != 405

    r = requests.put(link)
    assert r.status_code != 405

    r = requests.post(link)
    assert r.status_code != 405
    time.sleep(1)


    # limit_except
    link = "http://localhost:8082/"
    browser.get(link)

    r = requests.get(link)
    assert r.status_code == 405

    r = requests.head(link)
    assert r.status_code == 405

    r = requests.put(link)
    assert r.status_code != 405

    r = requests.post(link)
    assert r.status_code != 405
    time.sleep(1)