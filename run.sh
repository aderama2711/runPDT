echo $HOSTNAME
g++ main.cpp
./a.out &

nfd-start &>~/nfd.log&

sleep 10s
nfdc cs config capacity $1
cd ~/NLSR-configuration/IDN\ IP/$HOSTNAME
./run-$HOSTNAME.sh 
