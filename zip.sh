#! /bin/bash
read -e -i 'Grid Visible Block Edges 1.15.2-1.21.4 Release1.0 '"$(date +%F)"'.zip' -p 'Version Name: ' VERSION
find * -path source -prune -o -print | zip -@ '../'"$VERSION"
