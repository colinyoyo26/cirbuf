
for i in `seq 10 2000 100000`
do
    printf "$i "
    ./bench $i 
done >> data.txt

gnuplot tests/plotscript.gp && rm *.txt