#!/usr/bin/env bash#!/bin/bash

cat testfiles/test_one.txt | ././../cmake-build-debug/lz77 -c | ././../cmake-build-debug/lz77 -d > testfiles/output
if diff -s -a testfiles/output testfiles/test_one.txt >> /dev/null
then
    echo "Testfile one:         SUCCESFUL"
else
    echo "Testfile one:         FAILED"
fi

cat testfiles/test_two.txt | ././../cmake-build-debug/lz77 -c | ././../cmake-build-debug/lz77 -d > testfiles/output
if diff -s -a testfiles/output testfiles/test_two.txt >> /dev/null
then
    echo "Testfile two:         SUCCESFUL"
else
    echo "Testfile two:         FAILED"
fi

cat testfiles/test_three.txt | ././../cmake-build-debug/lz77 -c | ././../cmake-build-debug/lz77 -d > testfiles/output
if diff -s -a testfiles/output testfiles/test_three.txt >> /dev/null
then
    echo "Testfile three:       SUCCESFUL"
else
    echo "Testfile three:       FAILED"
fi


cat testfiles/test_four.txt | ././../cmake-build-debug/lz77 -c | ././../cmake-build-debug/lz77 -d > testfiles/output
if diff -s -a testfiles/output testfiles/test_four.txt >> /dev/null
then
    echo "Testfile four:        SUCCESFUL"
else
    echo "Testfile four:        FAILED"
fi

cat testfiles/30mb.txt | ././../cmake-build-debug/lz77 -o -c | ././../cmake-build-debug/lz77 -o -d > testfiles/output
if diff -s -a testfiles/output testfiles/30mb.txt >> /dev/null
then
    echo "Testing optimize:     SUCCESFUL"
else
    echo "Testing optimize:     FAILED"
fi