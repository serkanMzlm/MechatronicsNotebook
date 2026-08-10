# Serial Haberleşme
Arduino tarafında göndermek istediğimiz paket yapısını doğru bir şekilde alıp ROS2 kullanılarak publish edilir.

- build `colcon build`
- run   `ros2 run serial_comm serial_comm_node`
- default olarak **/dev/ttyACM0** dosyasını açmayı dener.
- **CMakeLists.txt** dosyasında bulunan bazı makrolar. `DEBUG` serial portan gelen verinin aynı şekilde erkana basılmasını sağlar. Bu kısım kapatılırsa paket yapısına uyuşması durumunda veri alınır. `BAUDRATE` kısmı ise haberleşme hızını ve `FILE_NAME` açılıcak olan dosya ismini belirtir.
- Herhangi bir byte sonrasını kontrol etmek istiyorsanız `DFLAG_DISABLE` değerini `false` ve kontrol edeceğiniz byte değerinin bir önce ki değerini `CONTROL_BYTE` kısmına yazın. 

```
add_definitions(-DDEBUG)             
add_definitions(-DBAUDRATE=B115200)
add_definitions(-DCONTROL_BYTE=112)
add_definitions(-DFLAG_DISABLE=true)
add_definitions(-DFILE_NAME="/dev/ttyACM0")
```