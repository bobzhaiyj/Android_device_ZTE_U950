
#!/system/bin/sh
wl down
wl mpc 0
wl country ALL
wl frameburst 1
wl scansuppress 1
wl up
wl channel $1
case "$4" in
	"802.11n" )
		wl nrate -m $2
		;;
	* )
		wl rate $2
		;;
esac
wl txpwr1 -o -d $3
wl pkteng_start 00:11:22:33:44:55 tx 100 1500 0

