rmdir /s /q "docs\"
mkdir "docs\"
xcopy "doxygen_temp\html\*" "docs\" /E /Q
pause