# 定義目標列表
targets=("abalone" "airline" "airline-ohe" "covtype" "epsilon" "higgs" "year_prediction_msd" "letters" )

# 迭代目標列表
for target in "${targets[@]}"; do
     #/home/chku/treebeard/llvm-project/build/bin/llc -O3 -relocation-model=pic -filetype=obj -o "$target".o "$target"_xgb_model_save.json.ll 
     /home/chku/treebeard/llvm-project/build/bin/clang -shared -o lib"$target".so "$target".o
     /home/chku/treebeard/llvm-project/build/bin/clang -o a.out b.c -L/home/chku/treebeard/llvm-project/mlir/examples/treebeard/xgll/ -lm -l"$target"
     echo "$target"
     for ((i = 1; i <= 10; i++)); do
          ./a.out $target
     done
     echo -e "\n"
done
