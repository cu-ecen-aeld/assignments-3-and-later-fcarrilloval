#!/bin/bash
#Writer Script
#Fernando Carrillo

#Accepts the following arguments: the first argument is a full path to a file (including filename) on the filesystem, referred to below as writefile; the second argument is a text string which will be written within this file, referred to below as writestr

#Exits with value 1 error and print statements if any of the arguments above were not specified

#Creates a new file with name and path writefile with writestr writestr, overwriting any existing file and creating the path if it doesn’t exist. Exits with value 1 and error print statement if the file could not be created.


# Check if exactly 2 arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <file> <string>"
    exit 1
fi

# Extract arguments
writefile="$1"
writestr="$2"

# Get directory and filename
directory=$(dirname "$writefile")
filename=$(basename "$writefile")

# Create the directory if it doesn't exist
mkdir -p "$directory"

# Write the writestr to the file
echo $writestr >> "$writefile"
