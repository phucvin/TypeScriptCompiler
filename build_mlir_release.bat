pushd
cd __build\mlir
cmake --build . --config Release --target install -j 8
cmake --install . --config Release
popd