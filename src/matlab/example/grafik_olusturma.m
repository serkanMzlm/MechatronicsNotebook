
test_height = "20";
x_label     = "Time (s)";
path = "./" + test_height + "m/" ;

screens = get(0, 'MonitorPositions');
first_screen = screens(2, :);

%% 1. Grafik Throttle - Height Grafiği
%figure('units','normalized','outerposition',[0 0 1 1]);
figure('units','normalized','outerposition',[first_screen(1), first_screen(2), first_screen(3), first_screen(4)]); 
hold on
yyaxis left;
plot(time_debug, height, LineWidth=5);

xlim([0 30]);
xticks(0:2:30);
ylim([0 21]);
yticks(0:2:21);

xlabel(x_label)
ylabel("Height (m)")
title("Throttle Effect of Vehicle Altitude (" + test_height + "m)")

yyaxis right;
plot(time_debug, thrust, LineWidth=5);
ylabel("Throttle")
legend("position_z", "throttle")
grid on;

set(gca, 'FontSize', 20)  
set(gca, 'LineWidth', 2)  % Grafik Çizgileri ve Grid Kalınlığı
% set(gcf, 'Position', get(0, 'Screensize')); % tam ekranın alınmasını
% sağlar.

print(gcf, path + "m_throttle_height", '-dpng', '-r400');

%% 2. Grafik Setpoint - Height Grafiği
figure('units','normalized','outerposition',[first_screen(1), first_screen(2), first_screen(3), first_screen(4)]);
hold on
plot(time_debug, height,'b', LineWidth=5);
plot(time_debug, setpoint_z, 'r', LineWidth=5);

xlim([0 30]);
xticks(0:2:30);
ylim([0 21]);
yticks(0:2:21);

xlabel(x_label)
ylabel("Height (m)")
title("Setpoint and Vehicle Height in the Z Axis (" + test_height + "m)")
legend("position_z", "setpoint_z")
grid on;

set(gca, 'FontSize', 20)  
set(gca, 'LineWidth', 2)  % Grafik Çizgileri ve Grid Kalınlığı

print(gcf, path + "m_setpoint_height", '-dpng', '-r400');

%% 3. Grafik Hız - Yükseklik 
figure('units','normalized','outerposition',[first_screen(1), first_screen(2), first_screen(3), first_screen(4)]);
yyaxis left;
plot(time_debug, height, LineWidth=5);

xlim([0 30]);
xticks(0:2:30);
ylim([0 21]);
yticks(0:2:21);

xlabel(x_label)
ylabel("Height (m)")
title("Height and Velocity for Z axis (" + test_height + "m)")

yyaxis right;
plot(time_debug, velocity_z, LineWidth=5);
ylabel("Velocity (m/s)")
legend("position_z", "velocity_z")
grid on;

set(gca, 'FontSize', 20)  
set(gca, 'LineWidth', 2)  % Grafik Çizgileri ve Grid Kalınlığı

print(gcf, path + "m_velocity_height", '-dpng', '-r400');

%% 4. Grafik EKF - Sensor Verisi
figure('units','normalized','outerposition',[first_screen(1), first_screen(2), first_screen(3), first_screen(4)]); 
hold on
plot(time_debug, height,'b', LineWidth=5);
plot(time_debug, position_z, 'r', LineWidth=5);

xlim([0 30]);
xticks(0:2:30);
ylim([0 21]);
yticks(0:2:21);

xlabel(x_label)
ylabel("Height (m)")
title("EKF output of Sensor Data for Z-Axis (" + test_height + "m)")
legend("sensor_z", "ekf_z")
grid on;

set(gca, 'FontSize', 20)  
set(gca, 'LineWidth', 2)  % Grafik Çizgileri ve Grid Kalınlığı

print(gcf, path + "m_ekf_sensor", '-dpng', '-r400');

%% 5. Grafik Acc - log

figure('units','normalized','outerposition',[first_screen(1), first_screen(2), first_screen(3), first_screen(4)]); 
hold on
plot(time_imu_debug, imu_z, LineWidth=5);

xlim([0 30]);
xticks(0:2:30);

xlabel(x_label)
ylabel("Acceleration (m/s^2)")
title("Acceleration Time (" + test_height + "m)")

set(gca, 'YScale', 'log')
set(gca, 'FontSize', 20)  
set(gca, 'LineWidth', 2)  % Grafik Çizgileri ve Grid Kalınlığı


grid on;
legend("acc_z")
print(gcf, path + "m_acc_log", '-dpng', '-r400');
%% 6. Grafik Acc

figure('units','normalized','outerposition',[first_screen(1), first_screen(2), first_screen(3), first_screen(4)]);
hold on
plot(time_imu_debug, imu_z, LineWidth=5);

xlim([0 30]);
xticks(0:2:30);

xlabel(x_label)
ylabel("Acceleration (m/s^2)")
title("Acceleration Time (" + test_height + "m)")

set(gca, 'FontSize', 20)  
set(gca, 'LineWidth', 2)  % Grafik Çizgileri ve Grid Kalınlığı


grid on;
legend("acc_z")
print(gcf, path + "m_acc", '-dpng', '-r400');

