#!/bin/bash

D=`dirname $0`
set -m

/usr/bin/chromium-browser $@ &
echo $! >$D/chromium.pid

fg
