#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

docker build -f docker/build.dockerfile -t dsapiforwardedfor-build docker
docker run \
	--rm \
	-v `pwd`:/usr/src/myapp \
	dsapiforwardedfor-build