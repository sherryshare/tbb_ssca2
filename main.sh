#!/bin/sh

echo "begin"
./cpu_user.sh & ./run.sh
wait
echo "end"
