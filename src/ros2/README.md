## ROS2 HUMBLE

### Paket Oluşturma
- Baket Oluşturma: `--dependencies` kısmından sonra yazılan paketler sisteme eklenmesi sağlanır. (cmake ve xml dosyasına)
    - `RCL` Ros Client Library.
```
ros2 pkg create --build-type  ament_cmake  cpp_node
ros2 pkg create --build-type  ament_cmake  cpp_node --dependencies rclcpp  (cpp)
ros2 pkg create --build-type  ament_python cpp_node --dependencies rclpy   (py)

(Paketin altında bir executable dosya oluşturur.)
ros2 pkg create --build-type ament_cmake  --node-name [node_name] [pkg_name] 
ros2 pkg create --build-type ament_python --node-name [node_name] [pkg_name]
```
```
(Belirtilen paketin executable dosyalarını gösterir.)
ros2 pkg executables [package_name] 
(Aktif paketleri gösterir.)
ros2 pkg list
```

### Build & Colcon
- ROS2 build yaparken kullanılan `colcon build` default olarak **makefiles** kullanılır. Build işlemini değiştirmek için CMake dosyasına parametre verilebilir. `colcon build --cmake-args -G Ninja  --packages-select cpp_topic`
- ROS2 build dosyası oluşturulurken release modunda oluşur. Bu modu değiştirmek hataları daha iyi görebilmek için `RelWithDebInfo` moduna alınabilir. `colcon build --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo`
- `symlink-install` yapmak **cpp** için pek bir anlam ifade etmez çünkü executable dosya build edilmeden yeni yazılan kodlar çalışmıyıcaktır. **Python**'da ise dosyada yaptığımız değişiklikler doğrudan çalışmasını etkiliyecektir. (**cpp** tarafında launch ve param dosyaları için kullanılabilir.)
```
colcon build     (Alt dizinlerde bulunan bütün CMakeLists dosyalarını bulup derler.)
colcon build --packages-select [pkg_name]  (Sadece belirtilen bir paketi build eder.)
colcon build --packages-skip [pkg_name]    (Belirtilen paketleri atlar.)
colcon build --symlink-install  (Direkt paketlerin içinde bulunan dosyaları taşımak yerine link yapılması sağlanır.)
colcon build --packages-skip-build-finished  (daha önceden build edilip bitmiş paketleri atlar.)

colcon graph    (Paketler arasında bağımlılıkları gösterir.)
```
### Run
```
. /opt/ros/humble/setup.bash 
source /opt/ros/humble/setup.bash                               (source veya . kullanılabilir.)
source /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash    (colcon build parametrelerini ekranda görme)
rosdep install -i --from-path src --rosdistro humble -y         (eksik paketlerin kontrol eder.)
```
```
ros2 run [pkg_name] [executable_name]
ros2 launch [pkg_name] [launch_name]
ros2 run demo_odes_cpp talker --ros-args -remap __node:=new_talker
ros2 run [pkg_name] [ex_name] -r [topic_name]:=new_topic_name
ros2 run [pkg_name] [ex_name] -r __node:=new_node -r [topic_name]:=new_topic
ros2 run [pkg_name] [ex_name] -r [service_name]:=new_service
```
- Kodlarda yazılan `RCLCPP_DEBUG` kısımları sadece parametre olarak bu kısımların gözükmesini istediğinizde ekranda görüntülenir.
- `--log-level debug` hangi tür çıktının alınmasını istediğimizi belirtiriz. (debug, info, error, warn)
```
ros2 run [pkg] [executable_file] --ros-args --log-level debug      (Bütün pakette bulunan)
ros2 run [pkg] [executable_file] --ros-args --log-level [node_name]:=debug  (Sadece belirli bir node)
```
- Eğer **param** kullanılıyorsa bu kısım ya `launch` dosyasında ya da çalıştırılması sırasında atama yapılması lazım
```
(Belirli bir parametreye çalıştırma sırasında atama yapılır.)
ros2 run [pkg_name] [node_name] --ros-args -p  [param_name]:= ...
(Belirli bir .yml dosyasından çekmesini sağlar.)
ros2 run [pkg_name] [ex_file] --ros-args --params-file [file_name].yaml
```

## VS Code 
### Debug
Paketlerde bulunan hataları daha rahat analiz edilmesi için **vs code** üzerinden ROS2 debug yapılabilir.
    - `colcon build --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo`  paketlerin runtime sırasında karşılaşına hataların daha rahat anlaşılmasını sağlar.
    - ROS2 paketini vs code ile runtime sırasında erişmek için belirli bir port üzerine yayın açmamız lazım bunu `ros2 run --prefix "gdbserver localhost:2000" [pkg_name] [node_name]` şeklinde sağlarız. (gdbserver yüklü değilse `sudo apt install gdbserver` komutu ile yüklenir.)
    - VS code üzerinden `launch.json` dosyasının içine belirtilen porta dinleme yapılacağı belirtilmelidir. Paket çalıştırılınca VS code debug moduna geçer ve hata ayıklamaya başlar.


    {
    "version": "0.2.0",
    "configurations":[
            {
                "name":"C++ Debugger",
                "request": "launch",
                "type": "cppdbg",
                "miDebuggerServerAddress":"localhost:2000",
                "cwd": "/",
                "program": "~/ex_ros/install/ex_1" 
            }
        ]
    }


### Sürekli Tekrarlanan Kodları JSON Dosyasına Yaptıma
Belirli komutların sürekli girmek yerine bu işlemi **vs code** sayesinde kolaylaştırılabilir. VS code bulunan **tasks.json** dosyası vs code tarafından yapılandırma sağlamak için kullanılır. Örneğin derleme, test etme, paketleme gibi görevleri tanımlayabiliriz. Birden fala görev yapılabilir otomatik olarak en son çalıştırılan çalışır. `launch.json` dosyası ise  uygulamayı debug yapabilmek için gerekli ayarların yapılmasını sağlar. Birden fala görev yapılabilir otomatik olarak en son çalıştırılan çalışır.
    - `Terminal > run build task` kısmında çalıştırılır.

    {
    "version": "2.0.0",
    "tasks":[
        {
            "label": "build",
            "type": "shell",
            "command": "source /opt/ros/humble/setup.bash && colcon build  --symlink-install"
        } ,
        {
            "label": "debug",
            "type": "shell",
            "command": "source /opt/ros/humble/setup.bash && colcon build  --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo"
        } 
        ]
    }

### ROS Extension
`ctrl+shift+p` kısayolları kullanılıp **ROS** yazılır.

---
### Genel Notlar
- **vs_code_json** dosyasının içinde örnek json şablonları bulunmaktadır.

```
rqt
rqt_graph
```
- `launch` dosyalarının isimlendirmesinde `launch` geçmek zorunde değildir. Alta bulunan isimlendirmelerin hepsi çalışır.
```
ex_1_launch.py
ex_2.launch.py
ex_3.py
```
- ROS2'da kullandığımız `RCLCPP_DEBUG, RCLCPP_INFO, RCLCPP_WARN, RCLCPP_ERROR` komut çıktılarını ekranda görürken aynı zamanda bilgisayarda `~/.ros/log` klasörünede yazılır.
- `COLCON_PREFIX_PATH, CMAKE_PREFIX_PATH` ve `AMENT_PREFIX_PATH` bu değişkenleri **setup.bash** çalıştırılınca doldurulur. Bu değişkenlerin içi boşaltılması için `export CMAKE_PREFIX_PATH=` şeklinde boşaltılabilir.

**DDS:** ROS2'nun iletişim katmanıdır. ROS2'da çihazlar birbirini görmesi için çihazların **domain ID** kısımları aynı olmasıdır.( default = 0) Linux işletim sisteminde **domain ID** için güvenli aralık 0-101'dir bu aralığın olma sebebi diğer aralıklarda linux aktif bir port kullanıyor olabilir. Domain ID değiştirmek için `export ROS_DOMAIN_ID=9`komutu girilir.
```
(Linux işletim sisteminde hangi aralıkta port acılabileceği. Max-Min)
cat /proc/sys/net/ipv4/ip_local_port_range 
```

**Local Host:** ROS2 paketlerinin ağda gözükmesini engeller. Sadece cihaz içinde bulunan paketlerin birbirini görmesini sağlar. `export ROS_LOCALHOST_ONLY=1 `


---
- https://docs.ros2.org/latest/api/rclcpp/
- https://index.ros.org/packages/
- https://github.com/ros2/launch
