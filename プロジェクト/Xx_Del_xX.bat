@echo off
echo �f�[�^�폜
del /q Log.txt
rd /s /q Debug
rd /s /q Release
rd /s /q .vs\Game\v15\ipch
rd /s /q .vs\Game\v16\ipch

pause