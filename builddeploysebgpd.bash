sudo supervisorctl stop sebgpd
cd sebgpd
make all
cd ..
cp external/mariadbpp/dist/Release/GNU-Linux/libmariadbpp.so lib/Release
cp seglobal/dist/Release/GNU-Linux/libseglobal.so lib/Release
cp sedata/dist/Release/GNU-Linux/libsedata.so lib/Release
cp semysql/dist/Release/GNU-Linux/libsemysql.so lib/Release
cp semath/dist/Release/GNU-Linux/libsemath.so lib/Release
cp seadmin/dist/Release/GNU-Linux/libseadmin.so lib/Release
cp seann/dist/Release/GNU-Linux/libseann.so lib/Release
cp secubist/dist/Release/GNU-Linux/libsecubist.so lib/Release
cp seeconomy/dist/Release/GNU-Linux/libseeconomy.so lib/Release
cp seeoddata/dist/Release/GNU-Linux/libseeoddata.so lib/Release
cp senet/dist/Release/GNU-Linux/libsenet.so lib/Release
cp setraining/dist/Release/GNU-Linux/libsetraining.so lib/Release
cp sepredict/dist/Release/GNU-Linux/libsepredict.so lib/Release
cp sesecurities/dist/Release/GNU-Linux/libsesecurities.so lib/Release
cp sexbrl/dist/Release/GNU-Linux/libsexbrl.so lib/Release
cp semain/dist/Release/GNU-Linux/libsemain.so lib/Release
sudo mkdir -p /usr/local/smartgaze/earnsmart/lib
sudo mkdir -p /usr/local/smartgaze/earnsmart/bg
sudo cp $(pwd)/lib/Release/*.* /usr/local/smartgaze/earnsmart/lib
sudo ldconfig /usr/local/smartgaze/earnsmart/lib
sudo cp $(pwd)/lib/Release/*.* /usr/local/smartgaze/earnsmart/lib
sudo ldconfig /usr/local/smartgaze/earnsmart/lib
sudo cp sebgpd/dist/Release/GNU-Linux/sebgpd /usr/local/smartgaze/earnsmart/bg
sudo supervisorctl start sebgpd

