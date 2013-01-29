#!/bin/bash
# count letter frequency in a text file
grep -aEoi "[a-z]" $1 | sort | uniq -c | sort -rn
