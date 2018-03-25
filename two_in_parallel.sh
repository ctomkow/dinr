#!/bin/bash

taskset -c 0 ./icmp_send & taskset -c 1 ./icmp_send && fg
