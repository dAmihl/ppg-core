rm -rf include/
rm -rf lib/
mkdir include/
mkdir lib/
cp src/*.h include/
cp build/build_results/bin/Debug/ppg-core.lib lib/
cp build/build_results/bin/Debug/ppg-core.dll lib/