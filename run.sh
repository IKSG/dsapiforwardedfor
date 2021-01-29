#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

./build-linux.sh

docker build -f docker/run.dockerfile -t dsapiforwardedfor-run docker

docker run \
	-it \
	--rm \
	-v `pwd`:/usr/src/myapp \
	-p 1352:1352 \
	-p 8082:8082 \
	--name dsapiforwardedfor-run \
	dsapiforwardedfor-run