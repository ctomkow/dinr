#!/bin/bash

taskset -c 1 ./icmp_send & taskset -c 2 ./icmp_send && fg
