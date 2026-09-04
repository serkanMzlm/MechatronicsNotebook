# Gömülü Sistem Kavramları

!!! note "Genel Bakış"
    Gömülü sistemler, belirli bir donanıma ve göreve özel, genellikle gerçek zamanlı kısıtlarla çalışan bilgisayar sistemleridir. Bu bölüm, ARM Cortex-M tabanlı mikrodenetleyicilerde bilinmesi gereken temel mimari ve donanım kavramlarını kapsar.

![mcu](../../assets/img/embedded/cpu_mpu_mcu.png)

---

## CPU - MPU - MCU

```mermaid
graph LR
    subgraph MCU["MCU (Microcontroller)"]
        direction TB
        CORE[CPU Core\nALU + CU + Register] --- MEM[Flash / SRAM]
        CORE --- PER[Peripherals\nGPIO / UART / SPI / ADC / Timer]
        CORE --- INT[Interrupt Controller\nNVIC]
    end

    subgraph MPU_["MPU (Microprocessor)"]
        direction TB
        CPU2[CPU Core] --- EXT_MEM[Harici RAM/ROM]
        CPU2 --- MMU[MMU]
        CPU2 --- EXT_PER[Harici Peripheral]
    end

    style MCU fill:#E3F2FD,stroke:#2196F3
    style MPU_ fill:#E8F5E9,stroke:#4CAF50
```

| Özellik           |    CPU    |    MPU    |          MCU          |
| ----------------- | :-------: | :-------: | :-------------------: |
| İşlemci           |     ✓     |     ✓     |           ✓           |
| Dahili Bellek     |     ✗     |     ✗     |    ✓ (Flash + SRAM)   |
| Dahili Peripheral |     ✗     |     ✗     |           ✓           |
| MMU               |     ✗     |     ✓     |   ✗ (genellikle MPU)  |
| İşletim Sistemi   |     ✗     | Linux vb. |   Bare-metal / RTOS   |
| Kullanım Alanı    | Hesaplama |  PC, SBC  | IoT, gömülü, otomotiv |

!!! note "CPU vs MCU"
    - **MCU:** Tek çipte CPU + Flash + SRAM + peripheral. Düşük maliyet, düşük güç. STM32, AVR, PIC.
    - **MPU:** Sadece işlemci çekirdeği; bellek ve periferaller harici. Raspberry Pi (BCM2837), i.MX8.

---

## Terimler

| Terim                 | Açıklama                                                                    |
| --------------------- | --------------------------------------------------------------------------- |
| **ALU**               | Aritmetik (toplama, çıkarma) ve mantıksal (AND, OR, XOR) işlemler           |
| **CU (Control Unit)** | Komutları sırayla okur, çözümler, kontrol sinyalleri üretir                 |
| **Register**          | CPU içindeki çok hızlı, küçük kapasiteli geçici depolama                    |
| **Peripheral**        | CPU çekirdeği dışında donanımsal iş yapan modüller (GPIO, UART, ADC…)       |
| **FPU**               | Kayan nokta işlemlerini yazılım yerine donanımda yapar                      |
| **T Bit**             | İşlemcinin hangi komut setini çalıştırdığını belirler (ARM/Thumb)           |
| **PLL**               | Düşük frekanslı giriş sinyalini çarparak yüksek frekans üretir              |
| **Callee-saved**      | Fonksiyon kullanılan register'ları işi bitince eski haline getirir (R4–R11) |

!!! danger "T Bit - HardFault Sebebi"
    Cortex-M mimarisinde T Bit **her zaman 1** olmalıdır (Thumb modu). Yanlış adresleme veya hatalı fonksiyon işaretçisi T Bit'i 0'a düşürür → `HardFault`. LSB = 1 → Thumb, LSB = 0 → ARM (Cortex-M'de desteklenmez).

---

## Bellek Mimarisi

```mermaid
graph TD
    subgraph "ARM Cortex-M Adres Uzayı (4 GB)"
        direction TB
        F["0x0000_0000 – 0x1FFF_FFFF\nCode (Flash)"]
        S["0x2000_0000 – 0x3FFF_FFFF\nSRAM"]
        P["0x4000_0000 – 0x5FFF_FFFF\nPeripheral Register"]
        R["0xE000_0000 – 0xE00F_FFFF\nSystem (NVIC, SysTick, ITM)"]
    end
    F --> S --> P --> R
    style F  fill:#BBDEFB
    style S  fill:#C8E6C9
    style P  fill:#FFE0B2
    style R  fill:#EDE7F6
```

### Stack Belleği

| Pointer                         | Kullanım Alanı                                  | Mod           |
| ------------------------------- | ----------------------------------------------- | ------------- |
| **MSP** (Main Stack Pointer)    | Kesme (ISR) ve sistem başlatma                  | Privileged    |
| **PSP** (Process Stack Pointer) | RTOS thread'leri; her task kendi PSP'sine sahip | Thread (user) |

```c
/* PSP kullanımı (CMSIS) */
__set_PSP(0x20002000);
__set_CONTROL(__get_CONTROL() | 2);  /* SPSEL = 1 → PSP seç */
```

### Heap Belleği

!!! warning "Gömülü Sistemlerde Heap Kullanımı"
    `malloc`/`free` çağrıları deterministik değildir ve bellek parçalanmasına (fragmentation) yol açabilir. Kritik güvenlik sistemlerinde ve sıkı gerçek zamanlı uygulamalarda heap kullanımından kaçınılır; statik tampon ve memory pool tercih edilir.

---

## AAPCS (ARM Procedure Call Standard)

```mermaid
graph LR
    A["Çağıran (Caller)"] -->|"R0–R3\nParametreler"| B["Çağrılan (Callee)"]
    B -->|"R0 (–R1)\nDönüş Değeri"| A
    A -->|"Stack\n5. param ve sonrası"| B
```

| Register Grubu | Roller                        | Kim Korur?                     |
| -------------- | ----------------------------- | ------------------------------ |
| R0 – R3        | Parametre + dönüş değeri      | Caller-saved (caller korumalı) |
| R4 – R11       | Genel amaçlı uzun ömürlü      | Callee-saved (callee korumalı) |
| R12 (IP)       | Geçici / derleyici ara değeri | Caller-saved                   |
| SP (R13)       | Stack pointer                 | Otomatik                       |
| LR (R14)       | Geri dönüş adresi             | Caller-saved                   |
| PC (R15)       | Program counter               | Otomatik                       |

---

## NVIC (Nested Vectored Interrupt Controller)

ARM Cortex-M kesme yönetim donanımıdır.

| Özellik            | Açıklama                                                      |
| ------------------ | ------------------------------------------------------------- |
| **Vectored**       | Her kesme için vektör tablosunda önceden tanımlı ISR adresi   |
| **Nested**         | Yüksek öncelikli kesme, düşük öncelikli ISR'ı kesebilir       |
| **Öncelik**        | 0 (en yüksek) → 255 (en düşük); sayı küçüldükçe öncelik artar |
| **Enable/Disable** | `NVIC_EnableIRQ(IRQn)` / `NVIC_DisableIRQ(IRQn)`              |

!!! note "Sabit Öncelikli İstisnalar"
    Reset (-3), NMI (-2), HardFault (-1) sabittir ve değiştirilemez; diğer kesmelerin önceliği yazılımla ayarlanabilir.

---

## RCC - HSI - HSE - PLL

```mermaid
graph LR
    HSI[HSI\n8/16 MHz\nDahili RC] --> MUX{Clock Mux}
    HSE[HSE\n4–25 MHz\nHarici Kristal] --> PLL --> MUX
    HSE --> MUX
    MUX --> SYSCLK[SYSCLK]
    SYSCLK --> |AHB Prescaler| AHB[AHB Bus]
    AHB --> |APB1 Prescaler| APB1[APB1 Bus\n≤42 MHz]
    AHB --> |APB2 Prescaler| APB2[APB2 Bus\n≤84 MHz]

    style HSI fill:#FFF9C4
    style HSE fill:#DCEDC8
    style PLL fill:#BBDEFB
    style SYSCLK fill:#F8BBD0
```

| Kaynak  | Tip                    |  Doğruluk  | Kullanım                     |
| ------- | ---------------------- | :--------: | ---------------------------- |
| **HSI** | Dahili RC              |    ±1 %    | Hızlı başlatma, genel amaçlı |
| **HSE** | Harici kristal         | < 100 ppm  | USB, CAN, hassas UART        |
| **LSI** | Dahili RC, 32 kHz      |   Düşük    | IWDG, RTC (düşük hassasiyet) |
| **LSE** | Harici 32.768 kHz      | Çok yüksek | RTC (yüksek hassasiyet)      |
| **PLL** | HSI/HSE tabanlı çarpan | Kaynağına bağlı (HSE ile beslenirse daha kararlı) | Yüksek sistem frekansı       |

---

## AHB - APB Bus Mimarisi

```mermaid
graph TD
    CPU[CPU\nCortex-M4] -->|I-Bus| FLASH[Flash Memory]
    CPU -->|D-Bus| SRAM[SRAM]
    CPU -->|S-Bus| AHB[AHB Matrix]
    DMA[DMA] --> AHB
    AHB -->|Bridge| APB1[APB1\nUART, SPI, I2C, TIM2-7]
    AHB -->|Bridge| APB2[APB2\nADC, TIM1, SPI1, USART1]
    AHB --> GPIO[GPIO]
    AHB --> USB[USB OTG]
```

| Özellik        |           AHB            |            APB             |
| -------------- | :----------------------: | :------------------------: |
| Hız            | Yüksek (burst, pipeline) | Düşük (basit, pipelinesız) |
| Gecikme        |          Düşük           |           Yüksek           |
| Karmaşıklık    |          Yüksek          |           Düşük            |
| Güç            |          Yüksek          |           Düşük            |
| Tipik bağlılar |   CPU, DMA, GPIO, USB    |   UART, SPI, I2C, Timer    |

---

## Bit-Banding

Belirli bir bellek bölgesindeki her bit için ayrı alias adresi üretir; RMW (Read-Modify-Write) döngüsünü atomik tek yazıya indirir.

| Bölge      |        Gerçek Adres       | Alias Başlangıcı |
| ---------- | :-----------------------: | :--------------: |
| SRAM       | 0x2000_0000 – 0x200F_FFFF |   0x2200_0000    |
| Peripheral | 0x4000_0000 – 0x400F_FFFF |   0x4200_0000    |

```c
/* Alias adresi hesabı */
#define BB_ADDR(base, byte_off, bit) \
    ((base##_BB) + (byte_off)*32 + (bit)*4)

/* GPIOA ODR bit 5 = Alias adresi üzerinden atomic set */
*(volatile uint32_t*)BB_ADDR(PERIPH, 0x40020014 - 0x40000000, 5) = 1;
```

!!! note "Cortex-M7 ve Sonrası"
    Bit-Banding Cortex-M0/M0+/M3/M4/M4F'te desteklenir. Cortex-M7 ve M23/M33'te kaldırılmıştır; yerini atomik `LDREX/STREX` ve C11 `_Atomic` almaktadır.

---

## Erişim Seviyeleri ve Modlar

```mermaid
stateDiagram-v2
    [*] --> Thread_Privileged : Reset
    Thread_Privileged --> Thread_Unprivileged : CONTROL.nPRIV = 1
    Thread_Unprivileged --> Thread_Privileged : SVC (Supervisor Call)
    Thread_Privileged --> Handler_Mode : Interrupt / Exception
    Thread_Unprivileged --> Handler_Mode : Interrupt / Exception
    Handler_Mode --> Thread_Privileged : EXC_RETURN
```

| Mod                       |        Erişim        | Açıklama                               |
| ------------------------- | :------------------: | -------------------------------------- |
| **Handler Mode**          | Privileged (zorunlu) | ISR ve exception handler'lar           |
| **Thread - Privileged**   |      Tam erişim      | Kernel kodu, RTOS kodu                 |
| **Thread - Unprivileged** |       Kısıtlı        | Kullanıcı uygulama kodu (MPU korumalı) |

---

## ISR (Interrupt Service Routine)

```mermaid
sequenceDiagram
    participant MAIN as Ana Program
    participant HW as Donanım
    participant NVIC as NVIC
    participant ISR as ISR

    MAIN->>MAIN: Normal çalışma
    HW->>NVIC: Kesme sinyali
    NVIC->>MAIN: Durdur, R0-R3 + LR stack'e kaydedilir
    NVIC->>ISR: PC → ISR adresine atla
    ISR->>ISR: Bayrağı temizle, işle
    ISR->>MAIN: EXC_RETURN → Kaldığı yerden devam
```

!!! danger "ISR Tasarım Kuralları"
    - ISR mümkün olduğunca **kısa** tutulmalı; ağır işler ana döngüye veya RTOS task'ına bırakılmalı.
    - ISR içinde `while()` veya bloklu bekleme **kesinlikle yasak**.
    - Kesme bayrağı temizlenmezse ISR sürekli tetiklenir (interrupt storm).
    - ISR ile paylaşılan veriler `volatile` ile işaretlenmelidir.

---

## SVC (Supervisor Call) ve Debug Arayüzleri

### SVC

Kullanıcı kodundan kernel/RTOS servislerine güvenli geçiş yöntemi:

```c
__ASM("svc 5");   /* SVC #5 ile kernel çağrısı */
```

### SWD vs JTAG

| Özellik      |        JTAG        |        SWD        |
| ------------ | :----------------: | :---------------: |
| Pin sayısı   |        4–5         |  2 (SWDIO, SWCLK) |
| Zincirleme   |  ✓ (daisy-chain)   |         ✗         |
| ARM Cortex-M |         ✓          | ✓ (tercih edilen) |
| Trace çıkışı |      ✓ (ETM)       |      ✓ (SWO)      |
| Kullanım     | FPGA, üretim testi |     MCU debug     |

!!! note "OpenOCD"
    OpenOCD (Open On-Chip Debugger), JTAG/SWD üzerinden mikrodenetleyici programlama ve debug yapılmasını sağlayan açık kaynaklı debug sunucusudur. GDB ile birlikte `arm-none-eabi-gdb` + OpenOCD + ST-Link/J-Link üçlüsü tipik geliştirme ortamını oluşturur.

---

## ITM (Instrumentation Trace Macrocell)

ARM Cortex-M3 ve üzeri çekirdeklerde bulunan, **UART kullanmadan** SWO pini üzerinden gerçek zamanlı log ve trace gönderme altyapısı.

```mermaid
graph LR
    SW[printf / ITM_SendChar] --> ITM[ITM Stimulus Port 0]
    ITM --> SWO[SWO Pin]
    SWO --> ST_LINK[ST-Link / Debug Probe]
    ST_LINK --> IDE[SWV ITM Data Console\nSTM32CubeIDE]
```

!!! note "Cortex-M0/M0+/M1 ITM Yoktur"
    Bu çekirdekler alan ve maliyet odaklı tasarlandığından CoreSight debug altyapısı (ITM, DWT, ETM) içermez. M3 ve sonrasında kullanılabilir.

---

## GPIO - Nibble - Bit İşlemleri

| Terim      | Açıklama                               |
| ---------- | -------------------------------------- |
| **Nibble** | 4 bit; 0x0–0xF arası hex değer         |
| **MSB**    | Most Significant Bit - en soldaki bit  |
| **LSB**    | Least Significant Bit - en sağdaki bit |

| Operatör | İşlem                             |
| -------- | --------------------------------- |
| `&`      | Bitwise AND - bit maskeleme için  |
| `\|`     | Bitwise OR - bit set için         |
| `^`      | Bitwise XOR - bit toggle için     |
| `~`      | Bitwise NOT - tüm bitleri çevirir |
| `<<`     | Bit sola kaydır                   |
| `>>`     | Bit sağa kaydır                   |

```c
/* Set, Clear, Toggle kalıpları */
REG |=  (1 << N);    /* Bit N'yi SET et         */
REG &= ~(1 << N);    /* Bit N'yi CLEAR et       */
REG ^=  (1 << N);    /* Bit N'yi TOGGLE et      */
bit = (REG >> N) & 1; /* Bit N'yi oku           */
```

---

## Cross Compiler

Geliştirme ortamında (x86/macOS/Windows) çalışıp hedef mimari (ARM Cortex-M) için kod üreten derleyici.

```
arm-none-eabi-gcc
│   │    │      └─ gcc (GNU Compiler Collection)
│   │    └─ eabi (Embedded Application Binary Interface)
│   └─ none (İşletim sistemi yok - bare-metal)
└─ arm (ARM mimarisi)
```

| Çıktı Formatı | Uzantı | Açıklama                                           |
| ------------- | :----: | -------------------------------------------------- |
| ELF           | `.elf` | Debug bilgisi + sembol tablosu; GDB ile kullanılır |
| Intel HEX     | `.hex` | ASCII kodlu; bootloader ile flashing               |
| Binary        | `.bin` | Saf makine kodu; doğrudan flash adresine yazılır   |

```mermaid
graph LR
    A[main.c] -->|Preprocessor| B[main.i]
    B -->|Parser + Codegen| C[main.s]
    C -->|Assembler| D[main.o]
    LIB[stdlib.a\nCustom Lib] --> E
    D -->|Linker LD Script| E[firmware.elf]
    E -->|objcopy| F[firmware.hex]
    E -->|objcopy| G[firmware.bin]
    style E fill:#BBDEFB
    style F fill:#C8E6C9
    style G fill:#FFE0B2
```

```bash
arm-none-eabi-gcc   -mcpu=cortex-m4 -mthumb -c main.c -o main.o
arm-none-eabi-ld    main.o -T linker.ld -o firmware.elf
arm-none-eabi-objcopy -O ihex   firmware.elf firmware.hex
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
arm-none-eabi-size  firmware.elf   # Flash ve RAM kullanımı
```