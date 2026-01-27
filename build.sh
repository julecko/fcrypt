#!/usr/bin/env bash

set -e

trap 'die "Command failed at line $LINENO"' ERR

die() {
    echo
    echo "[ERROR] $1" >&2
    exit 1
}

# Defaults
BUILD_TYPE="Release"
RUN_EXEC=0
EXEC_NAME="project_name"
RUN_ARGS=()

# Argument parsing
while [[ $# -gt 0 ]]; do
    case "$1" in
        -d)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r)
            RUN_EXEC=1
            shift
            # Collect remaining args for runtime
            while [[ $# -gt 0 ]]; do
                RUN_ARGS+=("$1")
                shift
            done
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

# Build
BUILD_DIR="build"

mkdir -p "$BUILD_DIR"


echo
echo "Configuring ($BUILD_TYPE)..."
cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    || die "CMake configuration failed"

echo
echo "Building..."
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" \
    || die "Build failed"

# Run
if [[ $RUN_EXEC -eq 1 ]]; then
    EXEC="$BUILD_DIR/$EXEC_NAME"

    if [[ ! -x "$EXEC" ]]; then
        echo "Error: executable not found or not executable: $EXEC"
        exit 1
    fi

    echo
    echo "Running $EXEC ${RUN_ARGS[*]}"
    "$EXEC" "${RUN_ARGS[@]}"
fi