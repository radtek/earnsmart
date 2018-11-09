First run these 

apt-get -y install g++ gdb libboost-all-dev libmariadb-client-lgpl-dev libxml++2.6-dev libcgicc5-dev libgsoap-dev 
apt-get -y install libfcgi-dev libjsoncpp-dev libtar-dev libcurl4-openssl-dev libzthread-dev libpthread-workqueue-dev 
apt-get -y install python-mysqldb-dbg libapache2-mod-fastcgi spawn-fcgi libpcre3-dev zlib1g-dev libgcrypt20-dev 
apt-get -y install libicu-dev python libgsl-dev libgsl-dbg libmrss0-dev libtidy-dev libgeoip-dev
apt-get -y install libmicrohttpd-dev libmicrohttpd-dbg libjsonrpccpp-server0 libjsonrpccpp-server0-dbg
apt-get -y install libssl-dev libgnutls-dev uuid libmagickwand-dev libcrypto++-dev libcrypto++-utils libcrypto++-doc
apt-get -y install libcppunit-dev uuid-dev libpoco-dev libgsl-dev libgsl-dbg libcurlpp-dev libvmime-dev
apt-get -y install libwxgtk3.0-dev libwxgtk-webview3.0-dev libwxgtk-media3.0-dev libwxgtk3.0v5-dbg
apt-get -y install libfccp-dev

Then:

1. Go to cmake.org and download the lastest cmake file (https://cmake.org/download/). Latest at this writing is https://cmake.org/files/v3.4/cmake-3.4.1-Linux-x86_64.sh.
	a. run the script
	b. open synaptics and purge earlier version of cmake from ubuntu (could cmake 3.2.2)
	c. copy files from cmake directory created by step (a) above to corresponding directories in linux file system.

2.   


