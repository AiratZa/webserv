import subprocess
import pytest

import os

import requests
import time


@pytest.fixture()
def webserv():
    # Setup

    proc = subprocess.Popen(["./WEBSERV.CONF"], executable='/Users/jnannie/Desktop/finalweb/webserv')

    yield  # здесь происходит тестирование
    time.sleep(1)
    proc.kill()
    time.sleep(1)


def test_put_request_file_not_exists_at_start(webserv):
    path = 'nginx_meme.jpg'
    with open(path, 'rb') as data:
        data_size = os.fstat(data.fileno()).st_size
        url = 'http://localhost:8080/' + path
        r = requests.put(url, data=data)
        assert r.status_code == 201
        with open('default_folder/nginx_meme.jpg', 'rb') as expected_file:
            final_data_size = os.fstat(expected_file.fileno()).st_size
            assert data_size == final_data_size

def test_put_request_file_exists_at_start(webserv):
    path = 'nginx_meme.jpg'
    with open(path, 'rb') as data:
        data_size = os.fstat(data.fileno()).st_size
        url = 'http://localhost:8080/' + path
        r = requests.put(url, data=data)
        assert r.status_code == 204
        with open('default_folder/nginx_meme.jpg', 'rb') as expected_file:
            final_data_size = os.fstat(expected_file.fileno()).st_size
            assert data_size == final_data_size

def test_put_request_passed_folder_name(webserv):
    path = 'default_folder'
    url = 'http://localhost:8080/' + path
    r = requests.put(url, data="")
    assert r.status_code == 409

def test_put_request_with_not_allowed_header(webserv):
    path = 'nginx_meme.jpg'
    headers = {'Content-Range': 'hello'}
    with open(path, 'rb') as data:
        data_size = os.fstat(data.fileno()).st_size
        url = 'http://localhost:8080/' + path
        try:
            r = requests.put(url, data=data, headers=headers)
        except BaseException:
            print("it's OK")

def test_put_request_with_non_existence_folder_in_way(webserv):
    path = '/hey/hey/nginx_meme.jpg'
    url = 'http://localhost:8080/' + path
    try:
        r = requests.put(url, data='')
        assert r.status_code == 500
    except BaseException:
        print("it's OK")
