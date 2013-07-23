del *.exe  /q

copy D:\project\pmsvoc\showC\Release\showC.exe 
copy D:\project\pmsvoc\bound\Release\toTrace.exe
copy D:\project\pmsvoc\edgeshow\Release\edgeshow.exe
copy D:\project\pmsvoc\samplekpts\Release\kptSample.exe
copy D:\project\pmsvoc\kptintg\Release\kptIntegr.exe


del E:\CarData\voc2007\testing\car\*.m /s /f  /q
del E:\CarData\voc2007\testing\car\*.py /s /f  /q
del E:\CarData\voc2007\testing\car\*.exe /s /f  /q

del E:\CarData\voc2007\training\car\*.m /s /f  /q
del E:\CarData\voc2007\training\car\*.py /s /f  /q
del E:\CarData\voc2007\training\car\*.exe /s /f  /q



copy *.m E:\CarData\voc2007\testing\car\
copy *.py E:\CarData\voc2007\testing\car\
copy *.exe E:\CarData\voc2007\testing\car\

copy *.m E:\CarData\voc2007\training\car\
copy *.py E:\CarData\voc2007\training\car\
copy *.exe E:\CarData\voc2007\training\car\
