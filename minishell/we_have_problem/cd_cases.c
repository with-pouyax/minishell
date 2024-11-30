syntax_error
-----------------------------------------------------
cd +
cd ?


no such file or directory:
----------------------------------------------------
cd -
cd woof
cd bark bark
cd $OLDPWD/directory
cd $PWD/directory
cd "$PWD/directory"
cd /Users



nothing happens:
----------------------------------------------------
cd /
cd '/'
cd //////
cd ./././
cd ../../..
cd '/////' >/dev/null
cd .
cd ..

