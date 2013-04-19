#!/bin/bash

d=`dirname $0`
build=/build/x86-generic

sudo cp $d/src/lib/32/libmagnet.so $build/lib

sudo mkdir -p $build/opt/google/chrome/plugins
sudo cp $d/src/libnpmagnet.so $build/opt/google/chrome/plugins

cp $d/src/test.html $build/tmp
cp $d/src/testclient $build/tmp

cp -a $d/../magnet-extension $build/tmp

tar -cvzf $d/npapi-magnet.tgz \
    $build/tmp/test.html \
    $build/tmp/testclient \
	$build/lib/libzmq.la \
	$build/lib/libzmq.so \
	$build/lib/libzmq.so.3 \
	$build/lib/libzmq.so.3.0.0 \
	$build/lib/libmagnet.so \
	$build/lib/libczmq.la \
    $build/lib/libczmq.so \
    $build/lib/libczmq.so.1 \
    $build/lib/libczmq.so.1.0.0 \
    $build/tmp/magnet-extension \
	$build/opt/google/chrome/plugins/libnpmagnet.so
