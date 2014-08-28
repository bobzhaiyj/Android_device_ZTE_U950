
#!/system/bin/sh
wl up
wl out
case "$2" in
	"802.11b" )
		wl band b
		;;
	* )
		wl band g
		;;
esac
wl fqacurcy $1 