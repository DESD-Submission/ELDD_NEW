
num1=12
num2=41

if [ $num1 -gt $num2 ]
then
	echo "1" | tee /dev/bbb_gpio0
else
	echo "0" | tee /dev/bbb_gpio0
fi

