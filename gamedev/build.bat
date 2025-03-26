gcc -o %1/%1.exe %1/main.c -lraylib -lgdi32 -lwinmm
if %errorlevel% neq 0 exit
cd %1
.\%1.exe