#!/bin/sh

if [ -f vm_src.pid ]; then
    kill $(cat vm_src.pid)
    echo "Src killed"
fi

if [ -f vm_snk.pid ]; then
    kill $(cat vm_snk.pid)
    echo "Snk killed"
fi
