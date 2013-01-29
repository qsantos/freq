#!/bin/bash
# count word occurrence in a text file
grep -Eoi "[a-z]*" $0 | sort | uniq -c | sort -rn | less
