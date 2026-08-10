clear position_z height velocity_z setpoint_z thrust time_debug data;

data = m20serbestnoise;

position_z = data.debugangularx(~isnan(data.debugangularx)) / 1000;
height = data.debugangulary(~isnan(data.debugangulary)) / 1000;
velocity_z = data.debugangularz(~isnan(data.debugangularz)) / 1000;
setpoint_z = data.debuglinearx(~isnan(data.debuglinearx)) / 1000;
thrust = data.debuglineary(~isnan(data.debuglineary));
time_debug = data.debuglinearz(~isnan(data.debuglinearz)) / 1000;

imu_z = data.debug_imulineary(~isnan(data.debug_imulineary));
time_imu_debug = data.debug_imulinearz(~isnan(data.debug_imulinearz)) / 1000;