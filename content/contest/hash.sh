# Hashes a file, ignoring all whitespace and comments. Use for
# verifying that code was correctly typed.
# run 'chmod +x hash.sh' before using
# usage: ./hash.sh <filename>
cpp -dD -P -fpreprocessed $1 | tr -d '[:space:]'| md5sum | cut -c-6
