#!/bin/sh

VALUE=0

while :
do
  echo $VALUE
  ./a.out $VALUE $VALUE $VALUE | nc 192.168.2.2 5000
  sleep 1


  let VALUE=$VALUE+50
done
