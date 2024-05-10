targets=("adult" "bank" "letter" "magic" "satlog" "sensorless" "spambase" "wine-quality")
for target in "${targets[@]}"; do
     clang -O3 -c ./$target/main.c -o ./$target/main.o
     clang -O3 -c ./$target/quantize.c -o ./$target/quantize.o
     clang run.c -I/home/chku/treeliteExp/$target/ $target/main.o $target/quantize.o -o run.out
     # clang run.c -I/home/chku/treeliteExp/$target/ $target/main.o -o run.out
     echo "$target"
     for ((i = 1; i <= 10; i++)); do
          ./run.out $target
     done
     echo -e "\n"
done
