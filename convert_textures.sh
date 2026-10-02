#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

INPUT_DIR="${SCRIPT_DIR}/assets/models"
OUTPUT_DIR="${SCRIPT_DIR}/assets/models"

if ! command -v ktx> /dev/null; then
    echo "[ERROR] 'ktx' was not found in your PATH."
    echo "Please install KTX-Software or add it to your PATH."
    exit 1
fi

echo "Starting PNG to KTX2 conversion..."
echo "Input Directory: ${INPUT_DIR}"
echo "--------------------------------"

shopt -s nullglob

for PNG_FILE in "${INPUT_DIR}"/*.png; do
    FILENAME_NO_EXT="$(basename "${PNG_FILE}" .png)"
    KTX_FILE="${INPUT_DIR}/${FILENAME_NO_EXT}.ktx2"

    echo "Converting: $(basename "${PNG_FILE}") ->${FILENAME_NO_EXT}.ktx2"
    
    ktx create --format R8G8B8A8_SRGB --assign-tf srgb "${PNG_FILE}" "${KTX_FILE}"
done

echo "--------------------------------"
echo "Texture conversion complete!"
read -p "Press [Enter] to continue..."
