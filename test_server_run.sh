#!/bin/bash

echo "#running server with proper port number passed, expecting no error"
command="echo 'q' | ./server 8080"
echo "TEST: $command"
echo "================================================================================"
eval $command
retcode=$?
echo "--------------------------------------------------------------------------------"
if (( $retcode != 0 )); then
    echo "BAD ($retcode)"
    exit
else
    echo "PASS ($retcode)"
fi
echo "================================================================================"
echo
echo

echo "#running server without selecting port, expecting error"
command="echo 'q' | ./server"
echo "TEST: $command"
echo "================================================================================"
eval $command
retcode=$?
echo "--------------------------------------------------------------------------------"
if (( $retcode == 0 )); then
    echo "BAD ($retcode)"
    exit
else
    echo "PASS ($retcode)"
fi
echo "================================================================================"
echo
echo

echo "#running server and selecting negative port number, expecting error"
command="echo 'q' | ./server -8080"
echo "TEST: $command"
echo "================================================================================"
eval $command
retcode=$?
echo "--------------------------------------------------------------------------------"
if (( $retcode == 0 )); then
    echo "BAD ($retcode)"
    exit
else
    echo "PASS ($retcode)"
fi
echo "================================================================================"
echo
echo

echo "#running server and selecting large port number, expecting error"
command="echo 'q' | ./server 10293847"
echo "TEST: $command"
echo "================================================================================"
eval $command
retcode=$?
echo "--------------------------------------------------------------------------------"
if (( $retcode == 0 )); then
    echo "BAD ($retcode)"
    exit
else
    echo "PASS ($retcode)"
fi
echo "================================================================================"
echo
echo

echo "#running server and passing non-number argument as port number, expecting error"
command="echo 'q' | ./server asldfkj"
echo "TEST: $command"
echo "================================================================================"
eval $command
retcode=$?
echo "--------------------------------------------------------------------------------"
if (( $retcode == 0 )); then
    echo "BAD ($retcode)"
    exit
else
    echo "PASS ($retcode)"
fi
echo "================================================================================"
