# MkDocs Kullanımı

```bash
mkdocs serve      # Local olaraka görüntülemek için kullanılır.
mkdocs gh-deploy  # GitHub Pages üzerinde yayınlamak için kullanılır.
```

### Kod Blokları

- **json** dosya türü 
- **title** dosya adı
- **linenums** satır numaralarını gösterir.
- **hl_lines** belirli satır aralığına vurgu yapar.

```json title="tasks.json" linenums="1" hl_lines="9-16"
json title="tasks.json" linenums="1" hl_lines="9-16"
```

### Bilgilendirme Kutuları (Admonitions)

```
!!! abstract ""

!!! tip "Info" 
    ...

!!! note "Info" 
    ...

!!! example "Usage Example" 
    ...

!!! danger "Warning"
    ...    
```

### Sekmeli İçerik (Content Tabs)

```
=== "A" 
    ```
    ...
    ```

=== "B" 
    ```
    ...
    ```
```

### Mermaid Diyagramları

```txt 
mermaid
graph LR 
    A[Flash] --> |I-Bus| B{CPU}; 
    B --> |D-Bus| C[RAM]; 
    B --> |S-Bus| D[DMA / Debug / Peripherals];
```

### Tablolar

```
| A1  | A2  |
| --- | --- |
| B1  | B2  |
```

### Kod Açıklamaları (Annotations)

Kod blokları yorum satırının içine numara yazılır `(1)` daha sonra bu numara alt kısımda **1. Açıklama** şeklinde belirlenir.

```c++
int main() {  // (1) 
    return 0; // (2)
} 
``` 

1. Program girişi 
2. Normal çıkış
