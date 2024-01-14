set MOC=..\..\..\..\Externals\Qt4.7.2\bin\x64\moc.exe

for %%c in (cl-qt*.hpp) do %MOC% %%c -o GeneratedFiles\moc_%%c.cpp
