
all: hashtable no_hash

libhashtable.so: src/ht_insert.c
	cc -ggdb3 -Wall -Wextra -mavx -O3 -shared src/ht_insert.c -o libhashtable.so

hashtable: src/hashtable.c libhashtable.so
	cc -ggdb3 -Wall -Wextra -mavx -O3 src/hashtable.c -o hashtable -lhashtable -L.


libnohashtable.so: src/no_hash_list.c
	cc -ggdb3 -Wall -Wextra -mavx -O3 -shared src/no_hash_list.c -o libnohashtable.so

no_hash: src/no_hash.c libnohashtable.so
	cc -ggdb3 -Wall -Wextra -mavx -O3 src/no_hash.c -o no_hash -lnohashtable -L.
