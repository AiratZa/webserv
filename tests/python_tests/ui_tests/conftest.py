import pytest
from selenium import webdriver
from webdriver_manager.chrome import ChromeDriverManager

import subprocess as sp
import os
import requests

import time

DEFAULT_PAUSE_BEFORE_BROWSER_CLOSE = 1




@pytest.fixture()
def webserv():
    # Setup

    proc = sp.Popen(["./WEBSERV.CONF"], executable='/Users/jnannie/Desktop/finalweb/webserv')

    yield  # здесь происходит тестирование
    time.sleep(1)
    proc.kill()


@pytest.fixture(scope="function")
def browser(request):
    browser = webdriver.Chrome(ChromeDriverManager().install())
    browser.set_window_size(1400, 1000)

    # Return browser instance to test case:
    yield browser

    time.sleep(DEFAULT_PAUSE_BEFORE_BROWSER_CLOSE)
    browser.quit()
