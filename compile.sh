#!/bin/sh

docker run --rm -v "C:\Users\blend\workspace\PlatformIO\framepad-gamepad:/home/qmk/qmk_firmware" qmkfm/qmk_cli sh -c "cd /home/qmk/qmk_firmware && make handwired/framepad_controller:default"
