#!/bin/bash

declare initial_dir=$(pwd)
declare bot_dir=$(pwd)/Bot
declare spelunkbots_dir=$(pwd)/Spelunkbots
declare neat_dir=$(pwd)/Bot/vendor/NEAT

buildDLL() {
	echo "Building Bot and SpelunkBots DLLs..."

	cd "$bot_dir"
	make
	cd "$spelunkbots_dir"
	make

	echo "done."
}

moveDLL() {
	echo "Moving DLLs..."

	cd "$initial_dir"
	cp ./**/*.dll ../Source/spelunky_1_1/

	echo "done."
}

buildVendor() {
	echo "Building Vendor Libraries..."

	echo "Building NEAT..."
	cd "$neat_dir"
	make

	echo "done."
}

main() {
	echo "========================="
	echo "BUILDING DLL SOLUTION..."
	echo "========================="

	buildVendor
	buildDLL
	moveDLL

	echo "========================="
	echo "DONE."
	echo "========================="
}

main
