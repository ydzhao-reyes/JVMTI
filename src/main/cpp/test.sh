for file in *
do
if [ "${file##*.}"x = "c"x ]
then 
  gcc -fPIC -shared $file  -I$JAVA_HOME/include -I$JAVA_HOME/include/darwin -Wall -Wno-deprecated  -o ../resource/lib${file%%.*}.so	
elif [ "${file##*.}"x = "cpp"x ]
then
  g++ -fPIC -shared $file  -I$JAVA_HOME/include -I$JAVA_HOME/include/darwin -Wall -Wno-deprecated  -o ../resource/lib${file%%.*}.so               
fi
done
