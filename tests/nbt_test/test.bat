@echo off
setlocal enabledelayedexpansion

:: ��ȡ����ĳ�����
set "program=%1"

:: ���û�д��������������ʾ���˳�
if "%program%"=="" (
    echo ���ṩ���Գ�������Ϊ����!
    exit /b 1
)

:: �������ļ���
set count=0
for %%f in (*.litematic) do (
    set /a count+=1
)

echo �ܹ��ҵ� %count% �� .litematic �ļ�
echo.

:: ��ʼ����������
set index=0

:: ������ǰ�ļ����µ����� .litematic �ļ�
for %%f in (*.litematic) do (
    set /a index+=1
    echo ���� #!index!: %%f
    "%program%" "%%f"
    echo.
)

endlocal
@echo on
