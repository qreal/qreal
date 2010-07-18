for i in $@; do

./generateInterfaces.pl --function-name $i  --header-file int_$i.h

done

