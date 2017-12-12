_algovi_complete()
{
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    if [[ $COMP_CWORD == 1 ]]
    then
        options="test tester help generate"
        COMPREPLY=($(compgen -W "${options}" -- ${cur}))
    elif [[ $COMP_CWORD == 2 ]]
    then
        prev="${COMP_WORDS[COMP_CWORD-1]}"
        if [[ $prev == test ]]
        then
            options="--status -s --init -i --add -a --remove -r visual switch rm"
            COMPREPLY=($(compgen -W "${options}" -- ${cur}))
        elif [[ $prev == tester ]]
        then
            options="--checker -c --src -s --test -t --verbose --statistics --oneline"
            COMPREPLY=($(compgen -W "${options}" -- ${cur}))
        fi
    else
        COMPREPLY=($(compgen -o default ${cur}))
    fi
    return 0
}

complete -o filenames -o bashdefault -F _algovi_complete algovi
