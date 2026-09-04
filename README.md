# Mechatronics Notebook



1. Linux (Temel)

2. C / C++ (Temel)

3. Embedded Linux & NVIDIA Jetson

4. ROS2 Robotik Yazılım Geliştirme: Artık robotik yazılım katmanına geçiyoruz. DDS, node/topic/service/action, QoS, lifecycle, executor modeli. Diğer her şeyin çatısı bu.

5. Sensör Füzyonu (Lidar, IMU, Radar)

6. SLAM ve Konumlandırma Algoritmaları

7. Otonom Sistemler ve Hareket Planlama

8. Gerçek Zamanlı Kontrol

9. İHA Kontrol ve Navigasyon


Modül 1: C / C++ ve Modern Yazılım Mimarisi

Neden İlk Sırada? Tüm otonom sistemin temeli. Bellek yönetimi (smart pointers, RAII), cache friendliness, zero-copy architecture, concurrency (pthread, std::thread, lock-free queues) ve C++17/20 özellikleri.

Modül 2: Linux & Embedded Linux & NVIDIA Jetson

Senior Odak: Process vs. Thread, IPC mekanizmaları (Shared Memory, Unix Domain Sockets), Kernel vs. User Space, Jetson mimarisi (CUDA, TensorRT, JetPack, memory bandwidth, thermal throttling), BSP ve custom Yocto/Buildroot mantığı.

Modül 3: Gerçek Zamanlı Kontrol (Real-Time Systems)

Senior Odak: Hard vs. Soft Real-time, PREEMPT_RT patch, determinizm, priority inversion, rate monotonic scheduling, jitter ve timer hassasiyeti.

Modül 4: ROS 2 Robotik Yazılım Geliştirme

Senior Odak: ROS 2 vs ROS 1 mimari farkları, DDS (Data Distribution Service) katmanı ve QoS (Quality of Service) ayarları, Lifecycle Nodes, Custom Executors, Zero-copy transport (iceoryx), ROS 2 Control katmanı.

Modül 5: Sensör Fizyonu (LiDAR, IMU, Radar)

Senior Odak: Kalman Filtreleri ailesi (KF, EKF, UKF), Sensör kalibrasyonu (extrinsic/intrinsic), sensör gecikmelerinin (time synchronization) kompanse edilmesi, LiDAR-IMU tight/loose coupling.

Modül 6: SLAM ve Konumlandırma Algoritmaları (Özel Odak)

Senior Odak: Visual SLAM (ORB-SLAM) vs LiDAR SLAM (LOAM, LIO-SAM, Cartographer), Graph-based SLAM, Loop Closure, Pose Graph Optimization (g2o, Ceres), Drift yönetimi ve haritalama stratejileri.

Modül 7: Otonom Sistemler ve Hareket Planlama (Özel Odak)

Senior Odak: Global Planner (A*, Dijkstra, Hybrid A*, RRT/RRT*), Local Planner / Obstacle Avoidance (DWA, TEB Local Planner, MPC), Kinematik/Dinamik kısıtlar (Ackermann, Differential Drive), Costmap2D katmanları.

Modül 8: İHA Kontrol ve Navigasyon

Senior Odak: Cascaded PID/LQR/MPC kontrolcüler, PX4 / ArduPilot mimarisi ve HAL (Hardware Abstraction Layer), MAVLink protokolü, state machine yönetimi, GNSS-denied ortamlarda navigasyon.