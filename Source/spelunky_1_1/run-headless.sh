#!/usr/bin/env bash

declare -r DISP=":1"
declare -r RESOLUTION="1024x720"
declare -r VNC_PORT=5999

create_display() {
    Xvfb ${DISP} -screen 0 ${RESOLUTION}x24
}

run_spelunkbots() {
    DISPLAY=${DISP} wine spelunkbots.exe
}

run_vnc_server() {
    x11vnc -display ${DISP} -localhost -rfbport ${VNC_PORT}
}

run_vinagre() {
    vinagre localhost:${VNC_PORT}
}

handle_sigint() {
    echo "KILLING"

    for proc in $(jobs -p); do
        kill ${proc}
    done
}

main() {
    create_display &
    run_vnc_server &
    sleep 1s ; run_vinagre &
    run_spelunkbots &

    trap handle_sigint SIGINT

    wait
}

main
