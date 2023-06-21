#!/bin/zsh

# for i in ; do sh diff.sh "$i"; done

./ft_nm $1 > ft_nm.txt
nm $1 > nm.txt

echo "Diff START"
diff -uN ft_nm.txt nm.txt
echo "Diff END"

