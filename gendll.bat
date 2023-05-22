echo off
rmdir /S build /Q
cmake -S . -B build
cd build
cmake --build . --config Release
cd ..
FOR /F "tokens=* USEBACKQ" %%F IN (`python -c "print('-'.join(map(lambda x : x.split(': ')[1], open('DEBIAN_CONTROL').read().split('\n')[:3])))"`) DO (
    SET FILENAME=%%F
)
copy build\Release\bencoder.dll %FILENAME%.dll
copy build\Release\bencoder.lib %FILENAME%.lib



