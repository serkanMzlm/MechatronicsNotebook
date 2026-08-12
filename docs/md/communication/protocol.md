# Haberleşme Protokolleri

!!! note "Genel Bakış"
    Haberleşme protokolleri; cihazlar, sistemler ve uygulamalar arasında veri alışverişinin nasıl gerçekleşeceğini tanımlayan kurallar bütünüdür. Seri/paralel fiziksel katmandan uygulama seviyesi protokollerine kadar katmanlı bir yapı içinde incelenir.

```mermaid
graph TD
    APP[Uygulama\nHTTP / MQTT / SSH] --> TRANS[Taşıma\nTCP / UDP]
    TRANS --> NET[Ağ\nIP / ICMP]
    NET --> DL[Veri Bağlantısı\nEthernet / Wi-Fi]
    DL --> PHY[Fiziksel Katman\nUART / SPI / I²C / CAN / BLE]

    APP2[Uygulama\nCustom Protocol] --> SERIAL[Seri Haberleşme\nUART / RS-232 / RS-485]
    SERIAL --> PHY2[Fiziksel Hat\nTX/RX / Bus]

    style APP fill:#E3F2FD
    style TRANS fill:#E8F5E9
    style NET fill:#FFF9C4
    style PHY fill:#FCE4EC
```

---

## UART (Universal Asynchronous Receiver/Transmitter)

UART, saat hattı olmayan eş zamansız seri haberleşme protokolüdür. İki hat (TX/RX) ve ortak toprak yeterlidir. Mikrodenetleyiciler ile PC/modüller arasındaki en yaygın düşük hızlı seri haberleşme yöntemidir.

### Çerçeve Yapısı

```
IDLE  START   D0   D1   D2   D3   D4   D5   D6   D7   PARITY  STOP
 1  |  0  |  x  |  x  |  x  |  x  |  x  |  x  |  x  |  x  |   x   |  1
     ←----------------- 1 tam çerçeve ------------------------→
```

| Alan         |   Değer   | Açıklama                     |
| ------------ | :-------: | ---------------------------- |
| Start bit    |     0     | Çerçeve başladığını bildirir |
| Veri bitleri |  5–9 bit  | Genellikle 8 bit (1 byte)    |
| Parity bit   | Opsiyonel | Hata tespiti: Odd/Even/None  |
| Stop bit     |  1 veya 2 | Hat tekrar IDLE'a döner      |

### Baud Rate Hesaplama

Alıcı ve verici **aynı baud rate**'i kullanmalıdır. Tolerans ±2–3%'tir.

$$\text{Baud} = \frac{f_{clock}}{16 \times \text{BRR}}$$

| Baud Rate | Veri Hızı (8N1) | Yaygın Kullanım    |
| :-------: | :-------------: | ------------------ |
|    9600   |     ~960 B/s    | GPS, eski modüller |
|   115200  |    ~11.5 KB/s   | Arduino, debug     |
|   460800  |     ~46 KB/s    | ESP32 flash        |
|   921600  |     ~92 KB/s    | Yüksek hızlı debug |
|  4000000  |    ~400 KB/s    | STM32, FTDI FT4232 |

```c title="STM32 USART2 - Register Seviyesi"
/* APB1 clock = 42 MHz, hedef: 115200 baud */
RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   /* Clock aç */
GPIOA->MODER |= (2 << 4) | (2 << 6);    /* PA2=TX, PA3=RX → Alternate */
GPIOA->AFR[0] |= (7 << 8) | (7 << 12); /* AF7 = USART2 */

USART2->BRR = 0x16D;       /* 42MHz / 115200 ≈ 365.0 → 0x16D */
USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

/* Gönder */
while (!(USART2->SR & USART_SR_TXE));
USART2->DR = 'A';
```

```c title="Linux'ta UART (termios)"
#include <termios.h>
#include <fcntl.h>

int uart_open(const char *dev) {
    int fd = open(dev, O_RDWR | O_NOCTTY | O_SYNC);
    struct termios tty = {0};
    tcgetattr(fd, &tty);
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag = CS8 | CREAD | CLOCAL;  /* 8N1 */
    tty.c_iflag = 0;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSANOW, &tty);
    return fd;
}
```

!!! tip "RS-232 vs RS-485"
    - **RS-232**: Noktadan noktaya, ±3–15V mantık, 15m maks.
    - **RS-485**: Diferansiyel çift, 32 cihaza kadar bus, 1200m, gürültüye dayanıklı, endüstri standardı.

---

## SPI (Serial Peripheral Interface)

SPI, 4 hat kullanan senkron, tam çift yönlü (full-duplex) seri haberleşme protokolüdür. Yüksek hız gerektiren sensörler, flash bellek, ekranlar ve ADC'lerde kullanılır.

```mermaid
graph LR
    MCU[Master\nMCU] -->|SCLK| DEV1[Slave 1]
    MCU -->|MOSI| DEV1
    DEV1 -->|MISO| MCU
    MCU -->|CS1 ▼| DEV1

    MCU -->|SCLK| DEV2[Slave 2]
    MCU -->|MOSI| DEV2
    DEV2 -->|MISO| MCU
    MCU -->|CS2 ▼| DEV2
```

| Hat   | Yön | Açıklama                    |
| ----- | :-: | --------------------------- |
| SCLK  | M→S | Saat sinyali; master üretir |
| MOSI  | M→S | Master Out Slave In         |
| MISO  | S→M | Master In Slave Out         |
| CS/SS | M→S | Chip Select; aktif LOW      |

### CPOL / CPHA Modları

| Mod | CPOL | CPHA | Saat Boşta |   Örnekleme    |
| :-: | :--: | :--: | :--------: | :------------: |
|  0  |  0   |  0   |    LOW     | Yükselen kenar |
|  1  |  0   |  1   |    LOW     |  Düşen kenar   |
|  2  |  1   |  0   |    HIGH    |  Düşen kenar   |
|  3  |  1   |  1   |    HIGH    | Yükselen kenar |

```c title="STM32 SPI1 - Polling"
/* Yapılandırma (APB2 = 84 MHz, SPI Clk = 84/16 = 5.25 MHz) */
SPI1->CR1 = SPI_CR1_MSTR          /* Master */
          | SPI_CR1_SSM            /* Yazılımsal CS */
          | SPI_CR1_SSI
          | (3 << SPI_CR1_BR_Pos) /* BR=011 → ÷16 */
          | SPI_CR1_SPE;          /* SPI etkinleştir */

uint8_t spi_transfer(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));   /* TX boş bekle */
    SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_RXNE));  /* RX hazır bekle */
    return SPI1->DR;
}

/* CS yönetimi */
#define CS_LOW()  GPIOA->BSRR = GPIO_BSRR_BR4
#define CS_HIGH() GPIOA->BSRR = GPIO_BSRR_BS4

CS_LOW();
spi_transfer(0x9F);           /* JEDEC ID oku */
uint8_t id = spi_transfer(0); /* Boş byte gönder, yanıt al */
CS_HIGH();
```

```bash title="Linux SPI (spidev)"
# spi-tools ile test
spi-config -d /dev/spidev0.0 -q
# Transfer: 0x9F gönder
python3 -c "
import spidev
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 1000000
resp = spi.xfer2([0x9F, 0x00, 0x00])
print([hex(x) for x in resp])
spi.close()
"
```

---

## I²C (Inter-Integrated Circuit)

I²C, iki hatlı (SDA/SCL) senkron yarı çift yönlü (half-duplex) çoklu master-slave haberleşme protokolüdür. 7-bit adres şeması 128 cihaza kadar bus paylaşımına izin verir.

### Elektrik Özellikleri

| Parametre        |          Değer           |
| ---------------- | :----------------------: |
| Hız (Standard)   |         100 kHz          |
| Hız (Fast)       |         400 kHz          |
| Hız (Fast+)      |          1 MHz           |
| Hız (High Speed) |         3.4 MHz          |
| Pull-up          | Genellikle 4.7 kΩ (3.3V) |
| Mantık düzeyi    |        Open-drain        |

!!! warning "Pull-up Direnci"
    I²C hatları açık-drain çalışır; dışarıdan pull-up direnci **zorunludur**. Direnç değeri bus kapasitansına ve hıza göre seçilir. Düşük hız = yüksek direnç, yüksek hız = düşük direnç.

### Bus Sırası

```mermaid
sequenceDiagram
    participant M as Master
    participant S as Slave (0x68)

    M->>S: START
    M->>S: 7-bit adres (0x68) + W
    S-->>M: ACK
    M->>S: Register adresi (0x3B)
    S-->>M: ACK
    M->>S: REPEATED START
    M->>S: 7-bit adres (0x68) + R
    S-->>M: ACK
    S-->>M: Data byte 0
    M->>S: ACK
    S-->>M: Data byte 1
    M->>S: NACK (son byte)
    M->>S: STOP
```

=== "HAL Örneği (STM32)"

    ```c title="i2c_hal.c"
    #include "stm32f4xx_hal.h"

    extern I2C_HandleTypeDef hi2c1;
    #define MPU6050_ADDR  (0x68 << 1)
    #define REG_ACCEL_X   0x3B

    void mpu6050_read(int16_t *accel) {
        uint8_t buf[6];
        uint8_t reg = REG_ACCEL_X;

        HAL_I2C_Master_Transmit(&hi2c1, MPU6050_ADDR,
                                &reg, 1, HAL_MAX_DELAY);
        HAL_I2C_Master_Receive(&hi2c1, MPU6050_ADDR,
                               buf, 6, HAL_MAX_DELAY);

        accel[0] = (int16_t)(buf[0] << 8 | buf[1]);
        accel[1] = (int16_t)(buf[2] << 8 | buf[3]);
        accel[2] = (int16_t)(buf[4] << 8 | buf[5]);
    }
    ```

=== "Register Seviyesi (STM32)"

    ```c title="i2c_register.c"
    /* START koşulu üret */
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    /* Adres + Write gönder */
    I2C1->DR = (0x68 << 1) | 0;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;   /* ADDR flag temizle */

    /* Register adresi gönder */
    I2C1->DR = 0x3B;
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    /* REPEATED START */
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    /* Adres + Read gönder */
    I2C1->DR = (0x68 << 1) | 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    /* Veri oku */
    I2C1->CR1 &= ~I2C_CR1_ACK;   /* Son byte için ACK kapat */
    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    uint8_t data = I2C1->DR;

    I2C1->CR1 |= I2C_CR1_STOP;
    ```

=== "Linux i2c-tools"

    ```bash
    # Bus'ta cihazları tara
    sudo i2cdetect -y 1

    # Register oku: bus=1, addr=0x68, reg=0x3B
    sudo i2cget -y 1 0x68 0x3B

    # Register yaz: 0x6B registerına 0x00 yaz (PWR_MGMT_1)
    sudo i2cset -y 1 0x68 0x6B 0x00

    # Python ile
    python3 -c "
    import smbus2
    bus = smbus2.SMBus(1)
    data = bus.read_i2c_block_data(0x68, 0x3B, 6)
    print(data)
    bus.close()
    "
    ```

---

## CAN Bus (Controller Area Network)

CAN, çoklu master destekli, diferansiyel çift üzerinden mesaj tabanlı haberleşme protokolüdür. Otomotiv, endüstriyel ve robot sistemlerinde standart haberleşme yöntemidir.

```mermaid
graph LR
    ECU1[ECU 1\nMotor] --- CANB[CAN Bus\nCAN_H / CAN_L]
    ECU2[ECU 2\nFren] --- CANB
    ECU3[ECU 3\nGövde] --- CANB
    ECU4[Teşhis\nCAN-PC] --- CANB
    R1[120Ω] --- CANB
    CANB --- R2[120Ω]
```

### CAN Çerçeve Yapısı (Standard Frame)

| Alan           |   Bit    | Açıklama                                     |
| -------------- | :------: | -------------------------------------------- |
| SOF            |    1     | Start of Frame                               |
| Arbitration ID |    11    | Mesaj kimliği; **düşük ID = yüksek öncelik** |
| RTR            |    1     | Remote Transmission Request                  |
| Control        |    6     | DLC (data length code: 0–8 byte)             |
| Data           | 0–64 bit | Taşınan veri                                 |
| CRC            |    15    | Cyclic Redundancy Check                      |
| ACK            |    2     | Alıcı onayı                                  |
| EOF            |    7     | End of Frame                                 |

### Bit Arbitrasyon

```mermaid
sequenceDiagram
    participant A as Node A\n(ID=0x100)
    participant B as Node B\n(ID=0x080)
    participant BUS as CAN Bus

    A->>BUS: ID bit 8 = 1
    B->>BUS: ID bit 8 = 0
    Note over BUS: Dominant (0) kazanır
    A-->>BUS: Geri çekilir (recessive)
    B->>BUS: Mesajı göndermeye devam eder
    Note over B: Node B kazandı (daha düşük ID)
```

### Hız / Mesafe Tablosu

| Bit Rate | Maks. Kablo Uzunluğu |
| :------: | :------------------: |
|  1 Mbps  |         25 m         |
| 500 kbps |        100 m         |
| 250 kbps |        250 m         |
| 125 kbps |        500 m         |
| 50 kbps  |        1000 m        |
| 10 kbps  |        5000 m        |

### Hata Durumları

```mermaid
stateDiagram-v2
    [*] --> ErrorActive: Başlangıç\n(TEC=REC=0)
    ErrorActive --> ErrorPassive: TEC > 127\nveya REC > 127
    ErrorPassive --> ErrorActive: TEC < 128\nve REC < 128
    ErrorPassive --> BusOff: TEC > 255
    BusOff --> ErrorActive: 128 × 11 recessive bit\n(Manuel reset)
```

!!! danger "Bus-Off Durumu"
    TEC (Transmit Error Counter) 255'i aşarsa node bus-off olur ve bus'tan tamamen kopar. Geri dönüş için manuel reset veya donanım yeniden başlatma gerekir.

=== "STM32 bxCAN TX"

    ```c title="can_tx.c"
    CAN_TxHeaderTypeDef hdr = {
        .StdId = 0x100,
        .IDE   = CAN_ID_STD,
        .RTR   = CAN_RTR_DATA,
        .DLC   = 4
    };
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    uint32_t mailbox;
    HAL_CAN_AddTxMessage(&hcan1, &hdr, data, &mailbox);
    ```

=== "STM32 bxCAN Filtre"

    ```c title="can_filter.c"
    CAN_FilterTypeDef f = {
        .FilterIdHigh         = 0x100 << 5,
        .FilterIdLow          = 0,
        .FilterMaskIdHigh     = 0x7FF << 5,   /* Tam eşleşme */
        .FilterMaskIdLow      = 0,
        .FilterFIFOAssignment = CAN_RX_FIFO0,
        .FilterBank           = 0,
        .FilterMode           = CAN_FILTERMODE_IDMASK,
        .FilterScale          = CAN_FILTERSCALE_32BIT,
        .FilterActivation     = ENABLE
    };
    HAL_CAN_ConfigFilter(&hcan1, &f);
    ```

=== "Linux SocketCAN"

    ```bash
    # Sanal CAN arayüzü
    sudo modprobe vcan
    sudo ip link add vcan0 type vcan
    sudo ip link set vcan0 up

    # CAN çerçevesi gönder
    cansend vcan0 100#01020304

    # Dinle (tüm çerçeveler)
    candump vcan0

    # Hata analizi
    canbusload vcan0
    ```

    ```python title="python-can"
    import can
    bus = can.interface.Bus(channel='vcan0', interface='socketcan')
    msg = can.Message(arbitration_id=0x100,
                      data=[0x01, 0x02, 0x03, 0x04],
                      is_extended_id=False)
    bus.send(msg)
    received = bus.recv(timeout=1.0)
    print(received)
    bus.shutdown()
    ```

---

## TCP / IP

TCP (Transmission Control Protocol), güvenilir, sıralı, çift yönlü bağlantı odaklı taşıma katmanı protokolüdür. IP, paketleri kaynak'tan hedef'e yönlendiren ağ katmanı protokolüdür.

### Bağlantı Kurma - Three-Way Handshake

```mermaid
sequenceDiagram
    participant C as İstemci
    participant S as Sunucu

    C->>S: SYN (seq=100)
    S-->>C: SYN-ACK (seq=200, ack=101)
    C->>S: ACK (seq=101, ack=201)
    Note over C,S: Bağlantı kuruldu

    C->>S: Veri (seq=101, len=50)
    S-->>C: ACK (ack=151)

    C->>S: FIN
    S-->>C: ACK
    S-->>C: FIN
    C->>S: ACK
    Note over C,S: Bağlantı kapatıldı
```

### TCP vs UDP Karşılaştırması

| Özellik       |      TCP       |          UDP          |
| ------------- | :------------: | :-------------------: |
| Bağlantı      |   Bağlantılı   |      Bağlantısız      |
| Güvenilirlik  |   ✓ Garanti    |     ✗ Best-effort     |
| Sıralama      |       ✓        |           ✗           |
| Akış kontrolü |       ✓        |           ✗           |
| Gecikme       |     Yüksek     |       **Düşük**       |
| Boyut         |    Değişken    |      Maks. 64 KB      |
| Kullanım      | HTTP, SSH, FTP | DNS, DHCP, RTSP, Oyun |

```c title="TCP Server (POSIX)"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    int srv = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port        = htons(8080)
    };
    bind(srv, (struct sockaddr *)&addr, sizeof(addr));
    listen(srv, 10);

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int cli = accept(srv, (struct sockaddr *)&cli_addr, &cli_len);

    printf("Bağlantı: %s\n", inet_ntoa(cli_addr.sin_addr));

    char buf[1024];
    ssize_t n = recv(cli, buf, sizeof(buf) - 1, 0);
    buf[n] = '\0';
    printf("Alındı: %s\n", buf);
    send(cli, "200 OK\r\n", 8, 0);

    close(cli);
    close(srv);
    return 0;
}
```

```c title="UDP Örneği"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/* Alıcı */
int sock = socket(AF_INET, SOCK_DGRAM, 0);
struct sockaddr_in addr = {
    .sin_family      = AF_INET,
    .sin_addr.s_addr = INADDR_ANY,
    .sin_port        = htons(5000)
};
bind(sock, (struct sockaddr *)&addr, sizeof(addr));

char buf[1024];
struct sockaddr_in sender;
socklen_t len = sizeof(sender);
recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&sender, &len);
```

## Bluetooth Classic ve BLE

### Bluetooth Protokol Yığını

```mermaid
graph TD
    APP[Uygulama Profili\nA2DP / HFP / SPP / GATT] --> HOST[Host Stack\nL2CAP / RFCOMM / ATT]
    HOST --> HCI[HCI\nHost-Controller Interface]
    HCI --> CTRL[Bluetooth Controller\nLMP / LL]
    CTRL --> RF[2.4 GHz RF\nFHSS / DSSS]
```

### Classic vs BLE Karşılaştırması

| Özellik           | Bluetooth Classic (BR/EDR) | Bluetooth Low Energy (BLE) |
| ----------------- | :------------------------: | :------------------------: |
| Kullanım          |       Ses/Veri akışı       |    Kısa periyodik veri     |
| Frekans kanalları |         79 × 1 MHz         |         40 × 2 MHz         |
| Veri hızı         |       1–3 Mbps (EDR)       |     125 Kbps – 2 Mbps      |
| Güç tüketimi      |           Yüksek           |       **Çok düşük**        |
| Bağlantı süresi   |          ~100 ms           |           < 3 ms           |
| Piconet           |     1 Master + 7 Slave     | Sınırsız (Mesh/Broadcast)  |
| Profil            |       A2DP, HFP, SPP       |  GATT (Generic Attribute)  |

### BLE GATT Mimarisi

```mermaid
graph TD
    CENTRAL[Central\nTelefon/PC] <-->|ATT Protocol| PERIPH[Peripheral\nSensör]
    PERIPH --> SVC1[Service: Battery 0x180F]
    PERIPH --> SVC2[Service: Heart Rate 0x180D]
    SVC1 --> CHAR1[Char: Battery Level\n0x2A19 | READ NOTIFY]
    SVC2 --> CHAR2[Char: HR Measurement\n0x2A37 | NOTIFY]
    CHAR2 --> DESC[CCC Descriptor\n0x2902 | Notify Enable]
```

```bash
# Linux'ta BLE araçları
bluetoothctl
  power on
  scan on
  connect AA:BB:CC:DD:EE:FF
  info AA:BB:CC:DD:EE:FF

# GATT okuma
gatttool -b AA:BB:CC:DD:EE:FF --char-read -a 0x0025

# Düşük seviye izleme
sudo btmon | grep -A5 "HCI Event"
```

```python title="BLE Python (bleak)"
import asyncio
from bleak import BleakClient

DEVICE = "AA:BB:CC:DD:EE:FF"
HR_CHAR = "00002a37-0000-1000-8000-00805f9b34fb"

async def main():
    async with BleakClient(DEVICE) as client:
        services = await client.get_services()
        for svc in services:
            print(f"Service: {svc.uuid}")

        def hr_callback(sender, data):
            print(f"HR: {data[1]} bpm")

        await client.start_notify(HR_CHAR, hr_callback)
        await asyncio.sleep(10)
        await client.stop_notify(HR_CHAR)

asyncio.run(main())
```

### Piconet ve Scatternet

| Yapı           | Açıklama                                                                 |
| -------------- | ------------------------------------------------------------------------ |
| **Piconet**    | 1 Master + maks. 7 aktif Slave                                           |
| **Scatternet** | Birden fazla Piconet'in örtüşmesi; bir cihaz iki Piconet'te rol alabilir |
| **FHSS**       | 79 kanalda saniyede 1600 hop - parazit direnci                           |

---

## Güvenlik Protokolleri

### TLS/SSL El Sıkışması

```mermaid
sequenceDiagram
    participant C as İstemci
    participant S as Sunucu

    C->>S: ClientHello\n(desteklenen cipher suites, TLS sürümü)
    S-->>C: ServerHello\n(seçilen cipher suite)
    S-->>C: Sertifika (X.509)
    S-->>C: ServerHelloDone
    C->>S: ClientKeyExchange\n(pre-master secret, RSA ile şifreli)
    C->>S: ChangeCipherSpec
    C->>S: Finished (HMAC)
    S-->>C: ChangeCipherSpec
    S-->>C: Finished (HMAC)
    Note over C,S: Şifreli kanal kuruldu
```

### RSA (Asimetrik Şifreleme)

RSA güvenliği, büyük sayıların asal çarpanlarına ayrılmasının hesaplama zorluğuna dayanır.

| Kavram                 | Açıklama                                         |
| ---------------------- | ------------------------------------------------ |
| **Public Key**         | Herkesle paylaşılır; yalnızca şifreler           |
| **Private Key**        | Gizli tutulur; şifreyi çözer ve imzalar          |
| **Anahtar uzunluğu**   | Minimum 2048-bit (≥4096-bit önerilir)            |
| **Matematiksel temel** | n = p × q (büyük asal sayılar); e·d ≡ 1 mod φ(n) |

```bash
# RSA anahtar çifti oluştur
openssl genrsa -out private.pem 4096
openssl rsa -in private.pem -pubout -out public.pem

# Şifreleme / Deşifreleme
openssl rsautl -encrypt -pubin -inkey public.pem -in data.txt -out data.enc
openssl rsautl -decrypt -inkey private.pem -in data.enc -out data.dec

# Dijital imza
openssl dgst -sha256 -sign private.pem -out sig.bin data.txt
openssl dgst -sha256 -verify public.pem -signature sig.bin data.txt
```

### SSH (Secure Shell)

SSH, OSI uygulama katmanında çalışan, **TCP 22** portunu kullanan şifreli uzaktan erişim protokolüdür.

|   Kimlik Doğrulama   |  Güvenlik  | Avantaj                           |
| :------------------: | :--------: | --------------------------------- |
|        Parola        |    Orta    | Kolay kurulum                     |
| RSA/Ed25519 Anahtar  | **Yüksek** | Şifresiz, brute-force'a dayanıklı |
| FIDO2 / Hardware Key | En yüksek  | Kimlik avına karşı dirençli       |

```bash
# Ed25519 anahtar çifti (RSA'dan daha küçük ve hızlı)
ssh-keygen -t ed25519 -C "serkan@host"

# Public key'i sunucuya kopyala
ssh-copy-id user@server
# veya
cat ~/.ssh/id_ed25519.pub | ssh user@server "cat >> ~/.ssh/authorized_keys"

# SSH bağlantısı
ssh -p 2222 user@server            # Özel port
ssh -i ~/.ssh/id_ed25519 user@server  # Belirli anahtar

# Tünel - yerel yönlendirme (L)
ssh -L 8080:localhost:80 user@server   # localhost:8080 → sunucu:80

# Tünel - uzak yönlendirme (R)
ssh -R 9090:localhost:3000 user@server # sunucu:9090 → yerel:3000

# SOCKS5 proxy
ssh -D 1080 user@server   # Tüm trafiği sunucu üzerinden geçir
```

```ini title="/etc/ssh/sshd_config - Güvenlik Ayarları"
Port 2222                      # Varsayılan 22'yi değiştir
PermitRootLogin no             # Root girişi engelle
PasswordAuthentication no      # Sadece anahtar
PubkeyAuthentication yes
MaxAuthTries 3
LoginGraceTime 30
X11Forwarding no
AllowUsers serkan admin        # Sadece belirtilen kullanıcılar
```

---

## MAVLink

MAVLink (Micro Air Vehicle Link), insansız hava araçları (UAV/drone) ve otonom sistemler için tasarlanmış, son derece hafif ikili (binary) haberleşme protokolüdür. 2009'da Lorenz Meier tarafından geliştirilmiştir. ArduPilot, PX4 ve PX4-based tüm uçuş denetleyicileri MAVLink'i standart iletişim protokolü olarak kullanır.

```mermaid
graph LR
    subgraph ARAÇ["Araç (Vehicle)"]
        FC[Uçuş Denetleyici\nPixhawk / CubePilot]
        ESC[ESC / Motor]
        GPS[GPS Modülü]
        FC --- ESC
        FC --- GPS
    end
    subgraph GCS["Yer Kontrol İstasyonu (GCS)"]
        QGC[QGroundControl]
        MP[Mission Planner]
        PY[Python / pymavlink]
        ROS[ROS2 / MAVROS]
    end

    FC <-->|MAVLink v2\nUART / UDP / TCP| TELM[Telemetri\nRadio 915 MHz]
    TELM <-->|MAVLink v2| QGC
    FC <-->|MAVLink v2\nUSB / UDP| MP
    FC <-->|MAVLink v2\nUDP 14550| PY
    FC <-->|MAVROS Bridge\nUDP / Serial| ROS

    style ARAÇ fill:#E3F2FD,stroke:#1565C0
    style GCS fill:#E8F5E9,stroke:#2E7D32
```

### MAVLink v1 vs v2

| Özellik            |   MAVLink v1  |        MAVLink v2       |
| ------------------ | :-----------: | :---------------------: |
| Magic byte         |     `0xFE`    |          `0xFD`         |
| Maks. payload      |    255 byte   |         255 byte        |
| Message ID         | 8 bit (0–255) |      24 bit (0–16M)     |
| İmzalama           |       ✗       |     ✓ (13 byte imza)    |
| Boş alan atlama    |       ✗       |         ✓ (trim)        |
| Component metadata |       ✗       |            ✓            |
| Kullanım           |     Legacy    | **Standart (önerilen)** |

### Paket Yapısı

=== "MAVLink v1"

    ```
    Byte:  0      1      2      3      4      5     6..N+5   N+6  N+7
           ┌──────┬──────┬──────┬──────┬──────┬──────┬───────┬──────┬──────┐
           │ 0xFE │ LEN  │ SEQ  │ SYS  │ COMP │ MSG  │ DATA  │ CRC  │ CRC  │
           │ STX  │(0-255│(0-255│  ID  │  ID  │  ID  │Payload│ LOW  │ HIGH │
           └──────┴──────┴──────┴──────┴──────┴──────┴───────┴──────┴──────┘
    ```

=== "MAVLink v2"

    ```
    Byte:  0      1      2      3      4      5     6  7  8   9..N+8   N+9 N+10  N+11..N+22
           ┌──────┬──────┬──────┬──────┬──────┬──────┬─────────┬──────────┬──────┬──────┬──────────────┐
           │ 0xFD │ LEN  │IFlag │CFlag │ SEQ  │ SYS  │  COMP   │  MSG ID  │ DATA │ CRC  │ SIGNATURE    │
           │  STX │      │      │      │      │  ID  │   ID    │ 24-bit   │      │2byte │ (opsiyonel)  │
           └──────┴──────┴──────┴──────┴──────┴──────┴─────────┴──────────┴──────┴──────┴──────────────┘
    ```

| Alan        | Açıklama                                         |
| ----------- | ------------------------------------------------ |
| **STX**     | Start of Frame sihirli byte                      |
| **LEN**     | Payload uzunluğu (byte)                          |
| **SEQ**     | Paket sıra numarası; kayıp tespiti için          |
| **SYS ID**  | Sistemi tanımlar (1–255; GCS genellikle 255)     |
| **COMP ID** | Bileşeni tanımlar (Autopilot=1, Camera=100, vb.) |
| **MSG ID**  | Mesaj tipi kimliği                               |
| **CRC**     | CRC-16/MCRF4XX + mesaj extra CRC                 |

## Protokol Seçim Rehberi

| Senaryo                            | Önerilen Protokol  |
| ---------------------------------- | :----------------: |
| MCU ↔ Sensör (kısa mesafe, hızlı)  |        SPI         |
| MCU ↔ Çoklu sensör (bus paylaşımı) |        I²C         |
| MCU ↔ PC debug / GPS / modem       |        UART        |
| Araç içi ECU ağı                   |      CAN Bus       |
| Güvenilir ağ iletişimi             |       TCP/IP       |
| Düşük gecikme, kayıp tolere        |        UDP         |
| Kısa mesafe kablosuz, ses          | Bluetooth Classic  |
| Pil ömrü kritik IoT                |        BLE         |
| Uzaktan terminal erişimi           |        SSH         |
| Yerel süreçler arası hızlı         | Unix Domain Socket |
| Kernel ↔ Userspace                 |  Netlink / ioctl   |
| UAV / Drone iletişimi              |     MAVLink v2     |
