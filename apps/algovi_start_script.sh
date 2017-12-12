#!/bin/bash

help_message="
 Algovi commands help message

 help                                    produce help message
 status [ folder ]                       actual status
 test
      --init [-i]                        init structure for test archive
      --create [-c]                      create test
      --status [-?]                      status of test archive
      --remove [-r]                      remove test from archive

 tester                                  launch testing process
      --src [-s]                         source code to test
      --test [-t] (default=all)          test on specific tests from archive
      --checker [-c] (default=
                        testlib_wcmp)    path to checker
      --validator [-v]                   path to validator
      --generator [-g]                   path to generator
 
  visual
     --src [-s]                          path to source code to test
     --vinput [-i]                       path to source code 'how to reinterpret input'
     --voutput [-o]                      path to source code for visualizer\n";


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
        elif [[ ${args[1]} == switch ]]
        then
            /opt/algovi/bin/algovi_test_switch ${args[@]:2}
        elif [[ ${args[1]} == rm ]]
        then
            /opt/algovi/bin/algovi_test_rm ${args[@]:2}
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
        tmp="none"
        if (( ${arg_size} > 1 ))
        then
            tmp=${args[1]}
        fi
        if [[ ${tmp} == visual ]]
        then
            exec /opt/algovi/bin/algovi_tester_visual ${args[@]:2}
        else
            exec /opt/algovi/bin/algovi_tester ${args[@]:1}
        fi
    fi
}

main $@
