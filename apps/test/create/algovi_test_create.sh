#!/bin/bash

trap : SIGINT

main()
{
	arg_size=$#
    args=("$@")
    #if (( ${arg_size} != 2 ))
    #then
        algovi test --status
        if [ $? -eq 0 ]
        then
            test_num=$(printf "%03d" $(($(/opt/algovi/bin/algovi_test_utils --count) + 1)))
            input_file="./tests/${test_num}.dat"
            output_file="./tests/${test_num}.ans"
            echo "Input: [ctrl+d to submit]"
            cat > ${input_file}
            echo "Output [ctrl+d to submit]"
            cat > ${output_file}
            echo "Test was saved with number ${test_num}"
        else
            echo "No test archive"
            echo "Use 'algovi test init' to create archive"
        fi
    #fi
}

main $@
