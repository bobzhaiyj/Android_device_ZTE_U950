
#!/system/bin/sh
ifconfig wlan0 down
echo -n "vendor/firmware/bcm4330/fw_bcm4330_mfg.bin" > sys/module/bcmdhd/parameters/firmware_path
#sleep 2
ifconfig wlan0 up

