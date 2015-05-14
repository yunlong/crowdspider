all: dspider 3gsoft testfanfou kugwa muzhiwan
muzhiwan: muzhiwan.cpp 
	    g++ -g -pipe -o muzhiwan muzhiwan.cpp  -lcrowdspider -L ../libspider  -I ../libspider -D_UNIX \
			-L ../libhtmlparser -lhtmlparser -I ../libhtmlparser -lssl

3gsoft: 3gsoft.cpp 
	    g++ -g -pipe -o 3gsoft 3gsoft.cpp  -lcrowdspider -L ../libspider  -I ../libspider -D_UNIX \
			-L ../libhtmlparser -lhtmlparser -I ../libhtmlparser -lssl

kugwa: kugwa.cpp
	    g++ -g -pipe -o kugwa kugwa.cpp  -lcrowdspider -L ../libspider  -I ../libspider -D_UNIX \
			-L ../libhtmlparser -lhtmlparser -I ../libhtmlparser -lssl

dspider: dspider.cpp
	g++ -g -pipe -o dspider dspider.cpp -lcrowdspider -L ../libspider -I ../libspider  -D_UNIX	\
	-L ../libhtmlparser -lhtmlparser -I ../libhtmlparser -lssl

testfanfou: testfanfou.cpp
	g++ -g -pipe -o testfanfou testfanfou.cpp MySQLDB.cpp  -lcrowdspider -L ../libspider -I ../libspider -D_UNIX -L ../libhtmlparser -lhtmlparser -I ../libhtmlparser -lssl -L/usr/lib/mysql -lmysqlclient

clean:
	rm -f httpcli dspider *.o  testindex testfanfou collectmovie 
