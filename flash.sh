#!/bin/bash

BUILD_PATH="$(pwd)"/build
ZEPHYR_PRJ_PATH="$HOME"/zephyrproject
VENV_PATH="$ZEPHYR_PRJ_PATH"/.venv

. "$VENV_PATH"/bin/activate

pushd "$ZEPHYR_PRJ_PATH"
pyocd flash -t nrf52833 "$BUILD_PATH"/zephyr/zephyr.hex
popd
deactivate