g++ -fPIC -shared Test.cpp -I$JAVA_HOME/include -I$JAVA_HOME/include/darwin -Wall -Wno-deprecated  -o libtestagent.so
