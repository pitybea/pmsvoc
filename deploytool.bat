del *.exe  /q

copy E:\project\pmsvoc\showC\Release\showC.exe 
copy E:\project\pmsvoc\bound\Release\toTrace.exe
copy E:\project\pmsvoc\edgeshow\Release\edgeshow.exe

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
