import subprocess
import pytest

import os

import requests


@pytest.fixture()
def webserv():
    # Setup
    proc = subprocess.Popen(["./WEBSERV.CONF"], executable='/home/airat/CLionProjects/webserv/webserv')

    yield  # здесь происходит тестирование
    proc.kill()


def test_put_request_file_not_exists_at_start(webserv):
    path = 'nginx_meme.jpg'
    data = open(path, 'rb')
    url = 'http://localhost:8080/put_test_3/' + path
    # url = 'http://localhost/put_test3/'
    cwd = os.getcwd()
    r = requests.put(url, data=data)
    # r = requests.put('http://localhost/put_test3/qws', data=open(path, 'rb'), headers=headers)
    print(r)
    print('hello)')



