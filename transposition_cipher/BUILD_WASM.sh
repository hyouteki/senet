#!/bin/bash
set -x -e

MODULE_NAME="module"
WARNS=("all" "extra" "switch-enum")
EXPORT_SYMBOLS=("hash", "hash_len")

PORT=8000

clang -Os -fno-builtin ${WARNS[@]/#/-W} --target=wasm32 \
	  --no-standard-libraries ${EXPORT_SYMBOLS[@]/#/"-Wl,--export="} \
	  -Wl,--no-entry -Wl,--allow-undefined \
	  -o ${MODULE_NAME}.wasm ${MODULE_NAME}.c

wasm2wat ${MODULE_NAME}.wasm > ${MODULE_NAME}.wat

kill -9 $(lsof -t -i:$PORT) || true
python3 -m http.server $PORT &
SERVER_PID=$!

open localhost:$PORT

function cleanup() {
	kill $SERVER_PID
	wait $SERVER_PID 2>/dev/null
	kill -9 $(lsof -t -i:$PORT) || true
}
trap cleanup EXIT

wait
# sleep 5
