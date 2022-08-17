set local
rd /s/q build_release
mkdir build_release
pushd build_release
cmake ..
cmake  --build . --config=Release 
popd
