#!/bin/bash
# https://github.com/embarkmobile/android-maven-example/blob/master/wait_for_emulator

bootanim=""
failcounter=0
until [[ "$bootanim" =~ "stopped" ]]; do
    bootanim=`adb -e shell getprop init.svc.bootanim 2>&1`
    echo "$bootanim"
    if [[ "$bootanim" =~ "not found" ]]; then
        let "failcounter += 1"
        if [[ $failcounter -gt 10 ]]; then
            echo "Failed to start emulator"
            exit 1
        fi
    fi
    sleep 1
done
echo "Done"
