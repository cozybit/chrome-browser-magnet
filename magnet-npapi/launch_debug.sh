#!/bin/bash

D=`dirname $0`

export MAGNET_NPAPI_DEBUG=1
screen -m -d -S chromium $D/launch_chromium.sh $D/src/test.html

echo -n "Waiting for chromium to start"

sleep 1; echo -n .
sleep 1; echo -n .
sleep 1; echo .

pluginpid=

function getpluginpid()
{
    pluginpid=`ps aux|grep libnpmagnet|grep -v grep|sed 's@[ ][ ]*@ @g'|cut -d' ' -f2`
}

getpluginpid

if [ -z "$pluginpid" ]; then
    echo -n "Waiting for plugin process to start"
fi

while [ -z "$pluginpid" ]; do
    sleep 1; echo -n .
    getpluginpid
done

echo
echo "Plugin PID: $pluginpid"

gdb --tui --pid=$pluginpid --command=breaks.txt
