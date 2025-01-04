#! /bin/bash
mkdir -p result
ins_elements="$(<side.json)"','
ins_textures="$(<textures.json)"','
for i in origin/*.json;do
        res="$({
            while read -r line;do
                    [[ "$line" =~ '"from"'|'"to"' ]] && {
                            echo 'From/To: '"$i" >&2
                            echo "$line" | while read -N 1 -r ele;do
                                    case "$ele" in
                                            [0-9.]) nums="$nums$ele" ;;
                                            *)
                                                    [ "$nums" == 0 ] && nums='0.001'
                                                    [ "$nums" == 16 ] && nums='15.999'
                                                    echo -n "$nums$ele";nums='' ;;
                                    esac
                            done
                            echo
                            true
                    } || echo -n "$line"$'\n'
                    [[ "$line" =~ '"elements"' ]] && {
                            echo "$ins_elements"
                            echo 'Elements: '"$i" >&2
                    }
                    [[ "$line" =~ '"textures"' ]] && {
                            echo "$ins_textures"
                            echo 'Found: '"$i" >&2
                    }
            done
        } < "$i")"
        [[ "$res" =~ '"elements"' ]] && {
                echo 'Result: '"${i/origin/result}" >&2
                echo -n "$res" > "${i/origin/result}"
        }
done 2>&1
