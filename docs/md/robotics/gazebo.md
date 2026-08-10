# Gazebo

- `ign gazebo` → `gz sim`
- `ign topic` → `gz topic`
- `ign model` → `gz model`
- **Yükleme Dizini:** GZ_SIM_SYSTEM_PLUGIN_PATH ortam değişkenine kendi plugin klasörünüzü ekleyin.
- **GUI Plugin’leri:** GZ_GUI_PLUGIN_PATH içinde aranır.
- **Resource Path:** GZ_SIM_RESOURCE_PATH tüm model, texture, plugin vb. yollarını içerir.
- **World:** Sahnedeki tüm öğeleri, GUI ayarlarını, plugin’leri ve fiziksel ortamı tanımlar.
- **Entity:** Her nesneye (model, link, joint, ışık, kamera vb.) atanan benzersiz sayı (ID).
- **Component:** Entity’ye işlevsellik ekler. Örneğin:
    - Pose (konum/orientasyon)
    - Name (görünürlük, seçme)
    - Material (görselleştirme)

!!! note "Not"
    Config dosyaları: `~/.gz/sim/<VERSION>/` altında. Varsayılan world ayarı için **GZ_SIM_SERVER_CONFIG_PATH** kullanın.

```bash
gz sim     # GUI’li/GUI’siz simülasyonu başlatır
gz topic   # Topic yönetimi
gz model   # Model sorgu ve kontrol
gz service # Servis çağrıları
```

```bash
# GUI’siz, sadece fizik sunucusu
gz sim -s

# GUI’li çalıştırma
gz sim

# Sürüm ve log ayarları
gz sim -v 4           # log seviyesi (0–4 arası)
gz sim --iterations 50 model_photo_shoot.sdf
```

- `-s / --server-only` → sadece server mod
- `-r / --run` → sahneyi çalışır halde başlatır
- `-v / --verbose` → log seviyesini belirler
- `--iterations` N → SDF modelinden N farklı açıda ekran görüntüsü alır


```bash title="Model Yönetimi"
gz model --list                          # Yüklü ve aktif modelleri listeler
gz model -m <model> --pose               # Modelin dünya içindeki pozunu gösterir
gz model -m <model> --link               # Modelin tüm link’lerini listeler
gz model -m <model> --joint              # Modelin tüm joint’lerini listeler
gz model -m <model> --link <link_name>   # Belirli link’in bileşen bilgisi
gz model -m <model> --joint <joint_name> # Belirli joint’in bileşen bilgisi
```

```bash title="Topic Yönetimi"
gz topic -l                         # Tüm topic’leri listeler
gz topic -h                         # Yardım (kullanım detayları)  
gz topic -i -t /stats              # /stats topic meta verisini gösterir
gz topic -e -t /stats              # /stats topic mesajlarını ekrana basar
gz topic --echo --topic /stats -n 1  # Sadece ilk mesajı gösterir

gz topic -t "/cmd_vel" -m gz.msgs.Twist -p "linear: {x: 0.5}, angular: {z: 0.05}"
```

- `-t / --topic` → topic adı
- `-m / --msg-type` → mesaj tipi
- `-p / --payload` → mesaj içeriği (YAML formatı)

```bash title="Topic Yönetimi"
gz service -l                          # Tüm servisleri listeler
# Simülasyonu durdurma/başlatma
gz service -s /world/<world>/control \
           --reqtype  gz.msgs.WorldControl \
           --reptype gz.msgs.Boolean \
           --timeout 3000 \
           --req 'pause: false'

# Simülasyonu sıfırlama (reset)
gz service -s /world/<world>/control \
           --reqtype  gz.msgs.WorldControl \
           --reptype gz.msgs.Boolean \
           --timeout 3000 \
           --req 'reset: {all: true}'

# Kamera görünümü kontrolü
gz service -s /gui/move_to/pose \
           --reqtype  gz.msgs.GUICamera \
           --reptype gz.msgs.Boolean \
           --timeout 5000 \
           --req 'name: "box", pose: {position: {x:1, y:23, z:5}, orientation: {x:0, y:0, z:0, w:1}}, projection_type: "orbit"'
```

| Format | Açıklama                                                             |
| ------ | -------------------------------------------------------------------- |
| STL    | Sadece geometri (triangül mesh) tanımlar; renk, doku içermez.        |
| OBJ    | Geometri + yüzey normalleri + doku koordinatları destekler.          |
| DAE    | COLLADA; sahne, geometri, ışık, animasyon ve malzeme bilgisi içerir. |


### ros_gz_bridge (Gazebo ↔ ROS 2)

ROS 2 ile Gazebo arasında mesaj, servis ve action köprüsü kurar.

```bash
ros2 run ros_gz_bridge parameter_bridge /chatter@std_msgs/msg/String@ignition.msgs.StringMsg
```

- `/topic@ROS_MSG_TYPE@GZ_MSG_TYPE` formatı
    - `@` çift yönlü (publish ve subscribe)
    - `]` Gazebo → ROS 2
    - `[` ROS 2 → Gazebo


### Video Kaydı ve Scene Broadcaster

```xml title="Scene Broadcaster"
<plugin filename="gz-sim-scene-broadcaster-system" name="gz::sim::systems::SceneBroadcaster">
  <state_hertz>25</state_hertz>
</plugin>
```

- `state_hertz` → 1 saniyede kaç kez sahneyi güncelleyeceği (Hz)

```xml title="Video Recorder GUI Plugin"
<plugin filename="VideoRecorder" name="VideoRecorder">
  <gz-gui>
    <property key="resizable" type="bool">false</property>
    <property key="width"     type="double">800</property>
    <property key="height"    type="double">600</property>
  </gz-gui>
  <record_video>
    <use_sim_time>true</use_sim_time>
    <lockstep>true</lockstep>
    <bitrate>4000000</bitrate>
  </record_video>
</plugin>
```

```xml title="Triggered Publisher System"
<plugin filename="gz-sim-triggered-publisher-system"
        name="gz::sim::systems::TriggeredPublisher">
  <input type="gz.msgs.Int32" topic="/keyboard/keypress">
    <match field="data">16777235</match>
  </input>
  <output type="gz.msgs.Twist" topic="/cmd_vel">
    linear: {x: 0.5}, angular: {z: 0.0}
  </output>
</plugin>
```

- input → dinlenecek topic ve mesaj alanı
- match → eşleşme değeri
- output → çıkış topic’i ve mesaj içeriği

## SDF
- Köprüleme (Tag) Biçimleri
    - **Uzun biçim:** `<tag>…</tag>`
    - **Kısa biçim (self‑closing):** `<tag/>` veya `<tag />`

```xml
<?xml version="1.0"?>
<sdf version="1.8">
  … içerik …
</sdf>
```

```xml title=" World ve Include"
<sdf version="1.8">
  <world name="first_world">
    <!-- Başka bir modeli bu dünyaya eklemek için -->
    <include>
      <uri>model://sum</uri>  <!-- model://<model_adı> biçiminde URI -->
    </include>
  </world>
</sdf>
```

- `<world>`: Sahnenin tamamını tanımlar (zemin, ışıklar, plugin’ler vb.).
- `<include>`: Önceden tanımlı bir modeli veya başka bir world dosyasını içe aktarır.
- `<uri>`: Yüklenecek modelin yolu (örn. model://sum, model://pioneer2dx).

### Model, Link ve Joint
-  `<model>` name özelliği ile tanımlanır.
    - İçinde bir veya birden çok `<link>, <joint>, <pose>` vb. barındırır.

```xml 
<model name="robot_model">
  <pose>0 0 0 0 0 0</pose>      <!-- Modelin dünya koordinatlarındaki pose’u -->
  <link name="link1">
    <pose relative_to="robot_model">0 0 0 0 0 0</pose>
  </link>
  <link name="link2"/>          <!-- pose verilmezse varsayılan konumda -->
</model>
```

- `<link>` Modelin fiziksel parçasıdır (kütle, inertia, collision, visual).
    - Her link’in kendi `<pose>`’u olabilir.

- `<joint>` İki link arasında hareketi sağlar.
    - type özelliği ile farklı eklem mekanizmaları seçilir:

| Tür          | Açıklama                                    |
| ------------ | ------------------------------------------- |
| `fixed`      | Hiç hareket etmez.                          |
| `revolute`   | Tek eksende sınırlı dönme.                  |
| `continuous` | Tek eksende sınırsız dönme.                 |
| `prismatic`  | Tek eksende sınırsız öteleme.               |
| `ball`       | Üç eksende serbest dönme (küresel).         |
| `universal`  | İki eksende serbest dönme.                  |
| `screw`      | Eşzamanlı öteleme ve dönme (vida hareketi). |

```xml
<joint name="joint1" type="revolute">
  <parent>link1</parent>       <!-- Sabit parça -->
  <child>link2</child>         <!-- Hareket eden parça -->
  <axis>
    <xyz>0 0 1</xyz>           <!-- Dönüş ekseni (x,y,z) <axis xyz="0 0 1"/> -->
  </axis>
</joint>
```

###  Pozisyon ve Yön (Pose & Origin)
- `<pose>`: Altı elemanlı liste—x y z roll pitch yaw (birim metre ve radyan).
- **relative_to** ile başka bir frame’e göre tanımlanabilir:

```xml
<pose>0 0 1 0 0 0</pose>  
<pose relative_to="world">1 2 0.5 0 0 1.57</pose>

<!--<origin> (URDF benzeri kısa form, SDF’de de çalışır) -->
<origin xyz="0 0 0.5" rpy="0 0 0.785"/>
```

```xml
<?xml version="1.0"?>
<sdf version="1.8">
  <world name="example_world">
    <include>
      <uri>model://house</uri>
    </include>

    <model name="robot">
      <pose>0 0 0 0 0 0</pose>

      <link name="base_link">
        <pose relative_to="robot">0 0 0 0 0 0</pose>
      </link>

      <link name="wheel_link"/>  <!-- default pose -->

      <joint name="wheel_joint" type="continuous">
        <parent>base_link</parent>
        <child>wheel_link</child>
        <axis>
          <xyz>0 1 0</xyz>
        </axis>
      </joint>
    </model>
  </world>
</sdf>
```