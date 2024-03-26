# 定義目標列表
targets=("abalone" "airline" "airline-ohe" "covtype" "epsilon" "higgs" "year_prediction_msd" "letters")

for target in "${targets[@]}"; do
     #/home/chku/llvm-project/build/bin/llc -O3 -relocation-model=pic -filetype=obj -o "$target".o "$target".ll 
     /home/chku/llvm-project/build/bin/clang -shared -o lib"$target".so "$target".o
     /home/chku/llvm-project/build/bin/clang -o run.out run.c -L/home/chku/treehierarchy/xgll/flint/ -lm -l"$target"
     echo "$target"
     for ((i = 1; i <= 10; i++)); do
          ./run.out $target
     done
     echo -e "\n"
done
