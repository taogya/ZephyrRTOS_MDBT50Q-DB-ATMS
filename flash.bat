
set BUILD_PATH=%cd%\build
set ZEPHYR_PRJ_PATH=%HOMEPATH%\zephyrproject
set VENV_PATH=%ZEPHYR_PRJ_PATH%\.venv

call %VENV_PATH%\Scripts\activate

pushd %ZEPHYR_PRJ_PATH%
pyocd flash -t nrf52833 %BUILD_PATH%\zephyr\zephyr.hex
popd
deactivate