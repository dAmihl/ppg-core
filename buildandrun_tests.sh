mkdir build/
cmake.exe --build build/ --target PPGTests --config Release && ./build/build_results/bin/Release/PPGTests.exe -s 
#./create_includes.sh
echo "Hit return to continue"
read dummy_variable