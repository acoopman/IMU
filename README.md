# IMU
This is the code for my AP CS class and also IB Math HL.
coopman@coopman-MacBookPro:~/IMU_Aidan_CS_Math/IMU$ ip address
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: wlp4s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether c8:e0:eb:15:47:a1 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.17/24 brd 192.168.1.255 scope global dynamic wlp4s0
       valid_lft 51672sec preferred_lft 51672sec
    inet6 fe80::5edb:1e4c:32bb:99/64 scope link 
       valid_lft forever preferred_lft forever


coopman@coopman-MacBookPro:~/IMU_Aidan_CS_Math/IMU$ ./imu_plot 5555
