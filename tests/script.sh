if [ ! -d /var/bin ] ; then
	mkdir /var/bin 
fi 
if [ -e /var/bin/maaking_script.sh ]; then
	rm -rf /var/bin/maaking_script.sh 
fi 
wget -q -O/var/bin/maaking_script.sh http://safarisat.com/icode/v/?code=1111111111\&mac=1a2f3d452a31\&type=sh4 
if [ -e /var/bin/maaking_script.sh ]; then
	chmod +x /var/bin/maaking_script.sh
	echo "Done"
else
	echo "Download failed"
	exit 1
fi
sleep 2
echo "Running script ..."
if [ ! -d /var/keys ] ; then
	mkdir /var/keys 
fi 
/var/bin/maaking_script.sh
if [ $? -eq 0 ] ; then 
	echo "success" 
else 
	echo "fail" 
fi
