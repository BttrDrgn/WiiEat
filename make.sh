#!/bin/bash

echo "Detecting new files..."
# Get the directory of the script
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define source directories
src_dir="$script_dir/src"
assets_dir="$script_dir/assets"

# Define temporary file to store current file structure
temp_file="$script_dir/.files.tmp"

# Check if the temporary file exists
if [[ ! -f "$temp_file" ]]; then
    # If the temporary file doesn't exist, create it and dump the file structure
    find "$src_dir" "$assets_dir" -type f > "$temp_file"
    chmod 600 "$temp_file"
    echo "Initial file structure dumped."
else
    # If the temporary file exists, compare it with the current file structure
    find "$src_dir" "$assets_dir" -type f > "$temp_file.new"
    
    # Compare the two file structures
    if ! cmp -s "$temp_file" "$temp_file.new"; then
        echo "Changes detected. Running make clean..."
        make clean
        mv -f "$temp_file.new" "$temp_file"
    else
        echo "No changes detected."
        rm -f "$temp_file.new"
    fi
fi

echo "Compiling WiiEat... (with $(nproc) cores)"
if ! make -j$(nproc); then
    echo "Compilation failed. Exiting script."
    exit 1
fi

# Set the directory containing the .dol files
dol_directory="dist/apps/WiiEat"

# Find the first .dol file in the directory
first_dol=$(find "$dol_directory" -name "*.dol" | head -n 1)

# Check if a .dol file was found
if [[ -z "$first_dol" ]]; then
    echo "No .dol files found in $dol_directory"
    exit 1
fi

# Check if the argument is "remote"
if [[ "$1" == "remote" ]]; then
    # Check if an IP address was provided
    if [[ -z "$2" ]]; then
        echo "Usage: $0 remote <IP_ADDRESS>"
        exit 1
    fi

    # Set the WIILOAD environment variable for TCP mode
    export WIILOAD="tcp:$2"

    echo "Running wiiload with $WIILOAD and $first_dol..."
    wiiload "$first_dol"
    exit 0
fi

# Ensure Dolphin is available
if ! command -v Dolphin &>/dev/null; then
    echo "MSYS2 was not launched with -use-full-path or Dolphin was not found in PATH."
    echo "Exiting script."
    exit 1
fi

# Conditionally use taskkill if on Windows
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    taskkill //F //IM Dolphin.exe //T
fi

# Run Dolphin with the first .dol file in background mode
echo "Running Dolphin with $first_dol..."
Dolphin -b -e "$first_dol" &
