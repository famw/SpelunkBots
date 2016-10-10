#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset

declare CC=i686-w64-mingw32.static-gcc 

declare initial_dir=$(pwd)/$(dirname "${0}")
declare bot_dir=${initial_dir}/Bot
declare spelunkbots_dir=${initial_dir}/Spelunkbots
declare neat_dir=${bot_dir}/lib/NEAT
declare spelunky_dir=${initial_dir}/../Source/spelunky_1_1/

declare cmd=${1:-main}

build_dll() {
	echo "Building Bot and SpelunkBots DLLs..."

	echo "Building Bot DLL..."
	cd "${bot_dir}"
	make
	echo "Building SpelunkBots DLL..."
	cd "${spelunkbots_dir}"
	make

	echo "done."
}

move_dll() {
	echo "Moving DLLs..."

    cd "${initial_dir}"
	cp ./**/*.dll "${spelunky_dir}"

	echo "done."
}

build_lib() {
	echo "Building Libraries..."

	echo "Building NEAT..."
	cd "${neat_dir}"
	make

	echo "done."
}

clean_libs() {
	echo "Cleaning NEAT"
	cd "${neat_dir}"
	make clean
}

clean_spelunkbots() {
	echo "Cleaning Bot DLL..."
	cd "${bot_dir}"
	make clean

	echo "Cleaning SpelunkBots DLL..."
	cd "${spelunkbots_dir}"
	make clean
}

clean_all() {
	clean_libs
	clean_spelunkbots
}

main() {
	echo "========================="
	echo "BUILDING DLL SOLUTION..."
	echo "========================="

	build_lib
	build_dll
	move_dll

	echo "========================="
	echo "DONE."
	echo "========================="
}

if [ $cmd == "main" ]; then
    main
elif [ $cmd == "clean" ]; then
    clean_spelunkbots
else
    clean_all
fi
