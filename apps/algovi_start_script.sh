#!/bin/bash

help_message="
 Algovi commands help message

 status [ folder ]                   actual status
 test
      --init                         init structure for test archive
      --create                       create test
      --status                       status of test archive
 tester                              launch testing process\n";


main()
{
    arg_size=$#
    args=("$@")
    if (( ${arg_size} < 1 ))
    then
        printf "${help_message}"
        return
    fi
    if [[ ${args[0]} == status ]]
    then
        /opt/algovi/bin/algovi_status
    elif [[ ${args[0]} == test ]]
    then
        if (( ${arg_size} < 2 ))
        then
            /opt/algovi/bin/algovi_test
        elif [[ ${args[1]} == visual ]]
        then
            /opt/algovi/bin/algovi_test_visual ${args[@]:2}
        else
            declare -A test_func_map=(["-r"]="remove" ["--remove"]="remove"
                                      ["-a"]="add"    ["--add"]="add"
                                      ["-s"]="status" ["--status"]="status"
                                      ["-i"]="init"   ["--init"]="init") 
            #inarray=$(echo "init status create" | grep -o "'${args[1]}'" | wc -w)
            if [[ -v test_func_map["${args[1]}"] ]]
            then
                /opt/algovi/bin/algovi_test_${test_func_map["${args[1]}"]} ${args[@]:2}
            else
                /opt/algovi/bin/algovi_test ${args[@]:1}
            fi
        fi
    elif [[ ${args[0]} == tester ]]
    then
        exec /opt/algovi/bin/algovi_tester ${args[@]:1}
    fi
}

main $@
