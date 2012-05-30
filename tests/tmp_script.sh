rm -rf ./script.sh 
wget -q -Oscript.sh http://maaking.com/plugin/?code=123456\&mac=01DA3E5D4532 
if [ -e ./script.sh ]; then
	chmod +x ./script.sh
	echo "Done"
else
	echo "Download failed"
	exit 1
fi
sleep 2
echo "Running script ..."
./script.sh
if [ $? -eq 0 ] ; then 	echo "success"else	echo "fail"