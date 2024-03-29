targets=("adult" "bank" "letter" "magic" "satlog" "sensorless" "spambase" "wine-quality")

for target in "${targets[@]}"; do
     /home/chku/llvm-project/build/bin/llc -relocation-model=pic -filetype=obj -o "$target".o "$target".ll 
     /home/chku/llvm-project/build/bin/clang -shared -o lib"$target".so "$target".o
     /home/chku/llvm-project/build/bin/clang -o a.out a.c -L/home/chku/treehierarchy/skll/ -lm -l"$target"
     echo "$target"
     for ((i = 1; i <= 10; i++)); do
          ./a.out $target
     done
     echo -e "\n"
done