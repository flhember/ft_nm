#!/bin/zsh
# for file in $@ /bin/* /sbin/* /usr/bin/* /usr/sbin/* /usr/lib/* /usr/lib/*/* /usr/libexec/* /usr/libexec/*/* /usr/share/*
for file in $@  
do
	echo "diff on $file"
	nm $file > resnm 2> errnm
	../ft_nm $file > resftnm 2> errftnm
	diff resnm resftnm
done
