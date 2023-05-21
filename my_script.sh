#!/bin/sh
echo "Enter password:"
read -r test
len=${#test}
if [ "$len" -lt 8 ]
then
	echo "Password is less then 8 symbols!"
	exit -1
fi

for (( i=0; i<"$len"; i++ ))
do
	password[i]=${test:$i:1}
done

SMALLSymNum=0
BigSymNum=0
NumbNum=0
SpecSymbNum=0

for (( i = 0; i < len; i++ ))
do
	for BIGchar in {A..Z}
	do
		if [ "${password[i]}" = "$BIGchar" ]
		then
			BigSymNum=$(($BigSymNum+1))
			break
		fi
	done
	for NUMB in {0..9}
	do
		if [ "${password[i]}" = "$NUMB" ]
		then
			NumbNum=$(($NumbNum+1))
			break
		fi
			
	done
	for char in {a..z}
	do
		if [ "${password[i]}" = "$char" ]
		then
			SMALLSymNum=$(($SMALLSymNum+1))
			break
		fi
	done
	
done

if [ "$(($len-$BigSymNum-$NumbNum-$SMALLSymNum))" != 0 ]
then
	SpecSymbNum=$(($len-$BigSymNum-$NumbNum-$SMALLSymNum))
fi

if [ $SpecSymbNum = 0 ]
then
	echo "Add special symbols!"
elif [ $SMALLSymNum = 0 ]
then
	echo "Add lowercase letters!"
elif [ $BigSymNum = 0 ]
then
	echo "Add capital letters!"
elif [ $NumbNum = 0 ]
then
	echo "Add numbers!"
else
	echo "Good password."
	echo "Password length:" $len
	echo "Number of lowercase letter:" $SMALLSymNum
	echo "Number of capital letter:" $BigSymNum
	echo "Number of numbers:" $NumbNum
	echo "Number of special symbols:" $SpecSymbNum
fi
exit 0