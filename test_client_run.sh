#!/bin/bash

echo "#running client with proper port number passed, expecting no error"
command="echo 'q' | ./client 8080"
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

echo "#running client without selecting port, expecting error"
command="echo 'q' | ./client"
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

echo "#running client and selecting negative port number, expecting error"
command="echo 'q' | ./client -8080"
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

echo "#running client and selecting large port number, expecting error"
command="echo 'q' | ./client 10293847"
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

echo "#running client and passing non-number argument as port number, expecting error"
command="echo 'q' | ./client asldfkj"
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
