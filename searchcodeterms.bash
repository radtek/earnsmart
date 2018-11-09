for f in $(find ./ -type f \( -iname \*.h -o -iname \*.cpp \)); do 
if grep -q "$1" "$f";
 	then echo $f; 
fi; done
