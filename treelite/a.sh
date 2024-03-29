targets=("adult" "bank" "letter" "magic" "satlog" "sensorless" "spambase" "wine-quality")

for target in "${targets[@]}"; do
     clang -c ./$target/main.c -o ./$target/main.o
     clang -c ./$target/quantize.c -o ./$target/quantize.o
     clang run.c -I/home/chku/RFExp/treelite/$target/ ./$target/quantize.o ./$target/main.o -o run.out
     echo "$target"
     for ((i = 1; i <= 10; i++)); do
          ./run.out $target
     done
     echo -e "\n"
done
