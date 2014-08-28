
#!/system/bin/sh
wl down
wl mpc 0
wl country ALL
wl frameburst 1
wl scansuppress 1
wl up
wl channel $1

