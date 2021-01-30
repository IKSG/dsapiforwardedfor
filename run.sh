#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

docker build -f docker/run.dockerfile -t dsapiforwardedfor-run .

docker run \
	-it \
	--rm \
	-v `pwd`:/usr/src/myapp \
	-p 1352:1352 \
	-p 8082:8082 \
	--name dsapiforwardedfor-run \
	dsapiforwardedfor-run