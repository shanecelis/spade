set -xe

JERRYSCRIPT_PARENT=${1-~/jerryscript_build}

cd $JERRYSCRIPT_PARENT
JERRYSCRIPT_PARENT="$(pwd)"; # Deals with relative directories.
rm -rf example_build
rm -rf example_install

python3 jerryscript/tools/build.py \
  --builddir=$(pwd)/example_build \
  --cmake-param="-DCMAKE_INSTALL_PREFIX=$(pwd)/example_install/" \
  --cmake-param="-G Unix Makefiles" \
  --mem-heap=350 \
  --debug \
  --clean \
  --error-messages=ON \
  --mem-stats=ON \
  --line-info=ON \
  --jerry-cmdline=OFF
make -C $(pwd)/example_build install

cd $JERRYSCRIPT_PARENT/spade/pc/jerry
cp -r $JERRYSCRIPT_PARENT/example_build/lib ./
# rm -rf include
cp -r $JERRYSCRIPT_PARENT/example_install/include ./
