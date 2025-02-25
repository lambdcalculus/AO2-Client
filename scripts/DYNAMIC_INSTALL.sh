#!/bin/bash

# Move to script's directory
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
cd "${SCRIPT_DIR}"

#add .desktop file (which should allow most DE's easy access to the program
mkdir -p ~/.local/share/applications

desktop_file="
[Desktop Entry]
Type=Application
Name=Attorney Online
Comment=The courtroom drama simulator
Exec=$(pwd)/Attorney_Online
Icon=$(pwd)/icon.png"

echo "$desktop_file" > ~/.local/share/applications/'Attorney Online'.desktop

#running the program
chmod +x Attorney_Online
./Attorney_Online
