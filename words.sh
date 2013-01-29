#!/bin/bash
# count word occurrence in a text file
grep -aEoi "[a-z]*" $1 | sort | uniq -c | sort -rn | less
