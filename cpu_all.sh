#!/bin/bash
interval=0.1
cpu_num=`cat /proc/stat | grep cpu[0-9] -c`

start_idle=()
start_total=()
cpu_rate=()
headfile="ssca2"
model="tbb"
filenumber=0
cpu_rate_file=./$headfile$model$filenumber.txt
while [ -f ${cpu_rate_file} ]
do
    filenumber=`expr $filenumber + 1`
    cpu_rate_file=./$headfile$model$filenumber.txt
done


for((i=0;i<${cpu_num};i++))
{       
    echo "$i 0 0" >> ${cpu_rate_file}
}

start_time=`date +%s.%N`
while(true)
do
    for((i=0;i<${cpu_num};i++))
    {
        start=$(cat /proc/stat | grep "cpu$i" | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}')
        start_idle[$i]=$(echo ${start} | awk '{print $4}')
        start_total[$i]=$(echo ${start} | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}')
    }

    sleep ${interval}
    for((i=0;i<${cpu_num};i++))
    {
        end=$(cat /proc/stat | grep "cpu$i" | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}')
        end_idle=$(echo ${end} | awk '{print $4}')
        end_total=$(echo ${end} | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}')
        idle=`expr ${end_idle} - ${start_idle[$i]}`
        total=`expr ${end_total} - ${start_total[$i]}`
        idle_normal=`expr ${idle} \* 100`
        cpu_usage=`expr ${idle_normal} / ${total}`
        cpu_rate[$i]=`expr 100 - ${cpu_usage}`
        #echo "The CPU$i Rate : ${cpu_rate[$i]}%"
        now_time=`date +%s.%N`
        echo "$i $(echo "$now_time $start_time" | awk '{printf("%0.3f", $1-$2)}') ${cpu_rate[$i]}" >> ${cpu_rate_file}
    }

done
