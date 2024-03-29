`clang -shared -o libabalone.so abalone.o`

`clang -o exp.out exp.c -L/so檔位置 -labalone`

`export LD_LIBRARY_PATH=/home/chku/treehierarchy/xgll/:$LD_LIBRARY_PATH`
