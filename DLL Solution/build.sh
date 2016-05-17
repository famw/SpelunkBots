#!/bin/bash

declare initial_dir=$(pwd)/$(dirname "${0}")
declare bot_dir=${initial_dir}/Bot
declare spelunkbots_dir=${initial_dir}/Spelunkbots
declare neat_dir=${bot_dir}/vendor/NEAT
declare spelunky_dir=${initial_dir}/../Source/spelunky_1_1/

build_dll() {
	echo "Building Bot and SpelunkBots DLLs..."

	cd "${bot_dir}"
	make
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

build_vendor() {
	echo "Building Vendor Libraries..."

	echo "Building NEAT..."
	cd "${neat_dir}"
	make

	echo "done."
}

main() {
	echo "========================="
	echo "BUILDING DLL SOLUTION..."
	echo "========================="

	build_vendor
	build_dll
	move_dll

	echo "========================="
	echo "DONE."
	echo "========================="
}

main
