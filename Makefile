# To run the resulting program, try something like:
#    ./sudppipe -l ./replace_plugin.so -L xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx,<hosted-graphite-key> <destination-server> 2004 2004


CFLAGS = -Wall -funsigned-char -Wno-unused-result 

all: sudppipe replace_plugin.so

sudppipe: sudppipe.c
	$(CC) $(CFLAGS) -g -O2 -o "$@" "$<" -ldl

replace_plugin.so: replace_plugin.c
	$(CC) $(CFLAGS) -g -O2 -shared -fPIC -o "$@" "$<"


clean:
	rm sudppipe replace_plugin.so
