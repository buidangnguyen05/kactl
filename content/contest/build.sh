# Builds a file and executes immediately.
# note: run 'chmod +x build.sh' before using
filename="${1%.cpp}"
g++ -Wall -Wconversion -Wfatal-errors -g -std=c++17 -fsanitize=undefined,address "$1" -o "$filename"
./"$filename"