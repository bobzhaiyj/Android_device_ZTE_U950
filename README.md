U950的CM11 device tree.
要获取vendor，请连接手机打开adb调试并运行./exact-files.sh
默认使用预编译内核
正常编译不会出现问题。

默认情况下，会卡在开机启动画面上，要进入系统请移除以下两个文件：
system/lib/hw/audio_policy.tegra.so
system/lib/hw/audio.primary.tegra.so

不工作的有：
wifi
bluetooth
rild
gps/agps
audio

Wifi不工作的可能原因是init配置错误...
