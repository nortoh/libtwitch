#!/bin/bash
USE_VALGRIND=0
USE_LOGGING=0

while getopts "vl" o; do
    case "${o}" in
        v) 
            USE_VALGRIND=1
            ;;
        l)
            USE_LOGGING=1
            LOG_FILE=$OPTARG
            ;;

    esac
done

echo "Cleaning"
make clean 1> /dev/null

echo "Making"
make 1> /dev/null


if [[ $USE_LOGGING == 1 ]]; then
    echo "Using logging to $LOG_FILE"
fi


if [[ $USE_VALGRIND == 1 ]]; then
    echo "Running with valgrind"
    echo "################################"
    valgrind --leak-check=yes ./main
else
    echo "Running normally"
    echo "################################"
    ./main
fi
