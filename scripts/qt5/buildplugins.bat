pushd \
call C:\Qt\Qt5.4.2\5.4\mingw491_32\bin\qtenv2.bat
popd

cd ..
cd plugins

for /D %%s in (*) do (

cd %%s
cd source
if exist %%s.pro (
qmake "CONFIG+=warn_off" %%s.pro
mingw32-make.exe clean /f >nul 2>&1
mingw32-make.exe release
mingw32-make.exe release
)
cd ..\..

)

cd ..
cd scripts
