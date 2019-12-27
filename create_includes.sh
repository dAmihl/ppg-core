rm -rf include/
rm -rf lib/
mkdir include/
mkdir lib/
cp src/*.h include/
cp build/build_results/bin/Release/ppg-core.lib lib/
cp build/build_results/bin/Release/ppg-core.dll lib/