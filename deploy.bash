supervisorctl stop all
cp -R lib/*.* /usr/local/earnsmart/lib
ldconfig /user/local/earnsmart/lib
supervisorctl start all
