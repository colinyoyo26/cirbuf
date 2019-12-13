
for i in `seq 32 60 10000`
do
    printf "$i "
    ./bench $i 
done >> data.txt

gnuplot tests/plotscript.gp && rm *.txt