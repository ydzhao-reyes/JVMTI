gcc -fPIC -shared replaceAgent.c -I$JAVA_HOME/include -I$JAVA_HOME/include/darwin -Wall -Wno-deprecated  -o libreplaceAgent.so
