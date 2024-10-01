#!/bin/bash

docker rm -f web
docker run -d --name web -p 7777:80 -v ./:/usr/share/nginx/html/ nginx