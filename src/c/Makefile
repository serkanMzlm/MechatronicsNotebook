# ==============================================================================
# Makefile — C Learning Project
#
# Görevler:
#   1. .config dosyasını okur (yoksa defconfig'den kopyalar)
#   2. config.h üretir (bool / tristate / int / hex / string ayrı işlenir)
#   3. .config'deki OPT_* / DEBUG / ASAN / UBSAN değerlerine göre CFLAGS kurar
#   4. MODULE_X=y olan modülleri ayrı binary olarak derler
#   5. MODULE_X=m olan modülleri ayrı nesne dosyası olarak derler
#
# Kullanım:
#   make              → .config yoksa defconfig yükler, derler
#   make menuconfig   → Kconfig menüsünü açar (.config yazar)
#   make build        → sadece derleme adımını çalıştırır
#   make clean        → build/ ve include/generated/ siler
#   make info         → aktif modülleri ve CFLAGS'i gösterir
# ==============================================================================

# ==============================================================================
# Dizin ve dosya değişkenleri
# ==============================================================================

PROJECT_NAME := c_learning_project

BUILD_DIR    := build
SRC_DIR      := src
INCLUDE_DIR  := include
CONFIG_DIR   := config

# .config dosyasının build dizinindeki konumu
DOTCONFIG    := $(BUILD_DIR)/.config

# Makefile tarafından üretilen C header dosyası
CONFIG_H     := $(INCLUDE_DIR)/generated/config.h

# Kconfig kaynak dosyası (menuconfig için gerekli)
KCONFIG_FILE := Kconfig

# ==============================================================================
# Compiler temel ayarları
# CFLAGS başlangıçta sadece include ve uyarı bayraklarını içerir.
# Optimizasyon ve debug bayrakları .config okunduktan sonra eklenir.
# ==============================================================================

CC     := gcc
CFLAGS := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/generated -Wall -Wextra -Wpedantic

# ==============================================================================
# .config okuma
#
# .config mevcut değilse bu blok boş değer üretir (hata vermez).
# Değerler config target çalıştıktan sonra anlamlı olur.
#
# shell komutu: her CONFIG_X=val satırını KEY=val çiftine dönüştürür,
# yorum satırları (#) ve boş satırlar atlanır.
# ==============================================================================

# wildcard diski okur ve .config varsa oku, yoksa boş bırak — hata verme
# ^$ -> boş satırları filtreler (^ baş, $ son) iki tane $$ kullanılır çünkü  
# Makefile içinde $ özel karakterdir.
CONFIG_VARS := $(if $(wildcard $(DOTCONFIG)), \
    $(shell grep -v '^\#' $(DOTCONFIG) | grep -v '^$$' | grep '='), \
)

# Belirli bir CONFIG sembolünün değerini döndüren yardımcı fonksiyon
# Kullanım: $(call cfg,MODULE_STDIO) → y / m / n / "" (bulunamazsa boş)
cfg = $(patsubst CONFIG_$1=%,%,$(filter CONFIG_$1=%,$(CONFIG_VARS)))

# ==============================================================================
# CFLAGS — .config değerlerine göre dinamik olarak kurulur
#
# Öncelik sırası:
#   OPT_NONE (DEBUG aktifse otomatik) > OPT_O1 > OPT_O2 > OPT_O3
#   DEBUG aktifse -g eklenir
#   ENABLE_ASAN aktifse -fsanitize=address eklenir
#   ENABLE_UBSAN aktifse -fsanitize=undefined eklenir
# ==============================================================================

# Optimizasyon bayrağı — choice bloğundan sadece biri y olabilir
ifeq ($(call cfg,OPT_NONE),y)
    CFLAGS += -O0
else ifeq ($(call cfg,OPT_O1),y)
    CFLAGS += -O1
else ifeq ($(call cfg,OPT_O3),y)
    CFLAGS += -O3
else
    # OPT_O2 varsayılan — hiçbiri seçilmemişse de O2 kullan
    CFLAGS += -O2
endif

# Debug sembolleri
ifeq ($(call cfg,DEBUG),y)
    CFLAGS += -g
endif

# AddressSanitizer — sadece DEBUG ile birlikte Kconfig'de aktif edilebilir
ifeq ($(call cfg,ENABLE_ASAN),y)
    CFLAGS += -fsanitize=address
endif

# UndefinedBehaviorSanitizer
ifeq ($(call cfg,ENABLE_UBSAN),y)
    CFLAGS += -fsanitize=undefined
endif

# ==============================================================================
# Modül → binary eşlemesi
#
# Her modülün kaynak dosyası ve çıktı binary adı burada tanımlanır.
# İleride dosya isimleri değiştiğinde sadece bu bölüm güncellenir.
#
# Yapı:
#   MODULE_<AD>_SRC  → kaynak .c dosyası
#   MODULE_<AD>_BIN  → çıktı binary adı (build/ altında)
# ==============================================================================

MODULE_STDIO_SRC   := $(SRC_DIR)/stdio_fundamentals.c
MODULE_STDIO_BIN   := $(BUILD_DIR)/stdio_test

MODULE_ALGO_SRC    := $(SRC_DIR)/algorithm_lab.c
MODULE_ALGO_BIN    := $(BUILD_DIR)/algorithm_test

MODULE_MEM_SRC     := $(SRC_DIR)/dynamic_memory.c
MODULE_MEM_BIN     := $(BUILD_DIR)/memory_test

MODULE_FILE_SRC    := $(SRC_DIR)/file_operations.c
MODULE_FILE_BIN    := $(BUILD_DIR)/file_test

MODULE_NET_SRC     := $(SRC_DIR)/system_network_ip_reader.c
MODULE_NET_BIN     := $(BUILD_DIR)/network_test

# ==============================================================================
# Aktif modül listesi
#
# .config'de MODULE_X=y olan modüller TARGETS listesine eklenir.
# MODULE_X=m olan modüller MODULE_OBJECTS listesine eklenir (ayrı .o dosyası).
# MODULE_X=n veya tanımsız olanlar derlenmez.
# ==============================================================================

TARGETS        :=
MODULE_OBJECTS :=

# MODULE_STDIO
ifeq ($(call cfg,MODULE_STDIO),y)
    TARGETS += $(MODULE_STDIO_BIN)
else ifeq ($(call cfg,MODULE_STDIO),m)
    MODULE_OBJECTS += $(BUILD_DIR)/stdio_fundamentals.o
endif

# MODULE_ALGORITHM
ifeq ($(call cfg,MODULE_ALGORITHM),y)
    TARGETS += $(MODULE_ALGO_BIN)
else ifeq ($(call cfg,MODULE_ALGORITHM),m)
    MODULE_OBJECTS += $(BUILD_DIR)/algorithm_lab.o
endif

# MODULE_DYNAMIC_MEMORY
ifeq ($(call cfg,MODULE_DYNAMIC_MEMORY),y)
    TARGETS += $(MODULE_MEM_BIN)
else ifeq ($(call cfg,MODULE_DYNAMIC_MEMORY),m)
    MODULE_OBJECTS += $(BUILD_DIR)/dynamic_memory.o
endif

# MODULE_FILE_OPS
ifeq ($(call cfg,MODULE_FILE_OPS),y)
    TARGETS += $(MODULE_FILE_BIN)
else ifeq ($(call cfg,MODULE_FILE_OPS),m)
    MODULE_OBJECTS += $(BUILD_DIR)/file_operations.o
endif

# MODULE_NETWORK
ifeq ($(call cfg,MODULE_NETWORK),y)
    TARGETS += $(MODULE_NET_BIN)
else ifeq ($(call cfg,MODULE_NETWORK),m)
    MODULE_OBJECTS += $(BUILD_DIR)/system_network_ip_reader.o
endif

# ==============================================================================
# Hedefler
# ==============================================================================

# Varsayılan hedef: sırayla çalıştır
.PHONY: all
all: prepare config config_header build

# ==============================================================================
# prepare — gerekli dizinleri oluşturur
# ==============================================================================

.PHONY: prepare
prepare:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(INCLUDE_DIR)/generated

# ==============================================================================
# config — .config dosyasını hazırlar
#
# Öncelik sırası:
#   1. Proje kökünde .config varsa build/ altına taşır (kullanıcı el ile düzenledi)
#   2. build/.config zaten varsa dokunmaz (önceki build korunur)
#   3. Hiçbiri yoksa defconfig'den kopyalar
# ==============================================================================

.PHONY: config
config: prepare
	@if [ -f .config ]; then \
		mv .config $(DOTCONFIG); \
		echo "[INFO] Existing .config moved to $(DOTCONFIG)"; \
	elif [ -f $(DOTCONFIG) ]; then \
		echo "[INFO] Using existing $(DOTCONFIG)"; \
	else \
		echo "[INFO] No .config found — loading defconfig"; \
		cp $(CONFIG_DIR)/defconfig $(DOTCONFIG); \
		echo "[INFO] $(DOTCONFIG) loaded from defconfig"; \
	fi

# ==============================================================================
# menuconfig — Kconfig menü arayüzünü açar
#
# Gereklilik: pip install kconfiglib
# Çalışma: mevcut .config varsa üzerine yazar, yoksa defconfig'den başlar
# ==============================================================================

.PHONY: menuconfig
menuconfig: prepare
	@echo "[INFO] Opening menuconfig (requires kconfiglib)..."
	@if [ -f $(DOTCONFIG) ]; then \
		KCONFIG_CONFIG=$(DOTCONFIG) menuconfig $(KCONFIG_FILE); \
	else \
		cp $(CONFIG_DIR)/defconfig $(DOTCONFIG); \
		KCONFIG_CONFIG=$(DOTCONFIG) menuconfig $(KCONFIG_FILE); \
	fi

# ==============================================================================
# config_header — .config dosyasını okuyarak config.h üretir
#
# Desteklenen değer tipleri:
#   CONFIG_X=y       → #define CONFIG_X 1
#   CONFIG_X=m       → #define CONFIG_X 2   (module: y=1, m=2, n=tanımsız)
#   CONFIG_X=n       → tanımsız bırakılır (#undef etkisi)
#   CONFIG_X=42      → #define CONFIG_X 42
#   CONFIG_X=0xFF    → #define CONFIG_X 0xFF
#   CONFIG_X="text"  → #define CONFIG_X "text"  (tırnaklar korunur)
#
# # ile başlayan satırlar (yorum ve "is not set") atlanır.
# Boş satırlar atlanır.
# ==============================================================================

.PHONY: config_header
config_header: config
	@echo "[INFO] Generating $(CONFIG_H)..."
	@echo "/* Generated by Makefile — DO NOT EDIT */" > $(CONFIG_H)
	@echo "/* Source: $(DOTCONFIG) */"               >> $(CONFIG_H)
	@echo "#pragma once"                             >> $(CONFIG_H)
	@echo ""                                         >> $(CONFIG_H)
	@if [ ! -f $(DOTCONFIG) ]; then \
		echo "[ERROR] $(DOTCONFIG) not found!"; \
		exit 1; \
	fi
	@while IFS= read -r line || [ -n "$$line" ]; do \
		case "$$line" in \
			""|"#"*) \
				continue ;; \
			*=*) \
				key=$${line%%=*}; \
				val=$${line#*=}; \
				case "$$val" in \
					y) \
						echo "#define $$key 1" >> $(CONFIG_H) ;; \
					m) \
						echo "#define $$key 2" >> $(CONFIG_H) ;; \
					n) \
						echo "/* $$key is not set */" >> $(CONFIG_H) ;; \
					\"*\") \
						echo "#define $$key $$val" >> $(CONFIG_H) ;; \
					0x*|0X*) \
						echo "#define $$key $$val" >> $(CONFIG_H) ;; \
					*) \
						echo "#define $$key $$val" >> $(CONFIG_H) ;; \
				esac ;; \
		esac; \
	done < $(DOTCONFIG)
	@echo "[INFO] $(CONFIG_H) generated successfully."

# ==============================================================================
# build — aktif modülleri derler
#
# y modülleri → ayrı executable (her birinin kendi main()'i var)
# m modülleri → ayrı .o nesne dosyası (main() yok, kütüphane gibi)
# n modülleri → derlenmez
# ==============================================================================

.PHONY: build
build: config_header $(TARGETS) $(MODULE_OBJECTS)
	@if [ -z "$(TARGETS)$(MODULE_OBJECTS)" ]; then \
		echo "[WARN] No modules enabled. Check $(DOTCONFIG)."; \
	else \
		echo "[INFO] Build complete."; \
		echo "[INFO] Binaries : $(TARGETS)"; \
		echo "[INFO] Objects  : $(MODULE_OBJECTS)"; \
	fi

# ==============================================================================
# Binary derleme kuralları — her modül için ayrı pattern
#
# Her modül bağımsız derlenir çünkü her birinin kendi main()'i var.
# Ortak CFLAGS kullanılır (optimizasyon + debug + sanitizer bayrakları dahil).
# ==============================================================================

$(MODULE_STDIO_BIN): $(MODULE_STDIO_SRC) $(CONFIG_H)
	@echo "[CC] $< → $@"
	@$(CC) $(CFLAGS) $< -o $@

$(MODULE_ALGO_BIN): $(MODULE_ALGO_SRC) $(CONFIG_H)
	@echo "[CC] $< → $@"
	@$(CC) $(CFLAGS) $< -o $@

$(MODULE_MEM_BIN): $(MODULE_MEM_SRC) $(CONFIG_H)
	@echo "[CC] $< → $@"
	@$(CC) $(CFLAGS) $< -o $@

$(MODULE_FILE_BIN): $(MODULE_FILE_SRC) $(CONFIG_H)
	@echo "[CC] $< → $@"
	@$(CC) $(CFLAGS) $< -o $@

$(MODULE_NET_BIN): $(MODULE_NET_SRC) $(CONFIG_H)
	@echo "[CC] $< → $@"
	@$(CC) $(CFLAGS) $< -o $@

# ==============================================================================
# Nesne dosyası derleme kuralları — tristate=m modüller için
#
# main() içermeyen, başka bir binary'e link edilebilecek nesne dosyaları.
# Şu an standalone kullanılmıyor; ileride kütüphane mimarisi için altyapı.
# ==============================================================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(CONFIG_H)
	@echo "[CC] $< → $@ (object)"
	@$(CC) $(CFLAGS) -c $< -o $@

# ==============================================================================
# info — aktif konfigürasyonu ekrana basar
# Derleme yapmadan mevcut durumu kontrol etmek için kullanılır
# ==============================================================================

.PHONY: info
info:
	@echo "========================================"
	@echo "  Project : $(PROJECT_NAME)"
	@echo "  Config  : $(DOTCONFIG)"
	@echo "  CC      : $(CC)"
	@echo "  CFLAGS  : $(CFLAGS)"
	@echo "----------------------------------------"
	@echo "  Active binaries  : $(if $(TARGETS),$(TARGETS),[none])"
	@echo "  Active objects   : $(if $(MODULE_OBJECTS),$(MODULE_OBJECTS),[none])"
	@echo "========================================"

# ==============================================================================
# clean — üretilen tüm dosyaları siler
# ==============================================================================

.PHONY: clean
clean:
	@echo "[INFO] Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(INCLUDE_DIR)/generated
	@echo "[INFO] Clean complete."