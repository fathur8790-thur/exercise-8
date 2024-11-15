# Exercise 8: Eliminating Resource Contention Using Semaphores

## Pendahuluan
Pada Exercise 8, kita mempelajari cara menghilangkan resource contention secara selektif dengan memanfaatkan semaphore. Pendekatan ini lebih efisien dibandingkan hanya menonaktifkan interrupt, karena hanya membatasi akses pada resource tertentu tanpa mengganggu keseluruhan sistem multitasking.

Semaphore digunakan untuk melindungi critical code section, memastikan bahwa hanya satu task yang dapat mengakses resource bersama pada satu waktu.

---

## Langkah-langkah Exercise

### **1. Membuat Semaphore**
Langkah pertama adalah membuat semaphore menggunakan FreeRTOS dan STM32CubeMX. Berikut detailnya:
1. **Definisikan Referensi Semaphore:**
   Tambahkan variabel referensi untuk semaphore pada bagian `Private variables`
   
2. **Definisikan Semaphore:**
   Tambahkan definisi semaphore

3. **Inisialisasi Semaphore:**
   Buat semaphore di awal program
   

### **2. Mengganti Interrupt Disabling dengan Semaphore**
Modifikasi kode dari **Exercise 7.3** dengan mengganti mekanisme `taskENTER_CRITICAL()` dan `taskEXIT_CRITICAL()` dengan semaphore:
- **Mengambil Semaphore** (Wait):
  ```c
  osSemaphoreWait(CriticalResourceSemaphoreHandle, WaitTimeMilliseconds);
  ```
- **Melepas Semaphore** (Release):
  ```c
  osSemaphoreRelease(CriticalResourceSemaphoreHandle);
  ```

### **3. Menjalankan Exercise**
#### Exercise 8.1
- Tetapkan nilai `WaitTimeMilliseconds` menjadi nilai yang sesuai, misalnya 1.
- Jalankan program dan perhatikan perilaku LED:
  - LED biru **tidak menyala**, menandakan tidak ada resource contention.
  - Tugas dengan prioritas tertinggi (misalnya, tugas LED orange) berjalan seperti yang diharapkan.

#### Exercise 8.2
- Tetapkan nilai `WaitTimeMilliseconds` menjadi 0.
- Jalankan program dan perhatikan perilaku LED:
  - LED biru menyala, menunjukkan bahwa contention terjadi karena semaphore tidak cukup menunggu.

---

## Kesimpulan
### **Temuan pada Exercise 8.1**
- Semaphore membatasi pengaruh mutual exclusion hanya pada resource tertentu.
- Tidak ada contention yang terjadi, sehingga performa sistem lebih stabil.

### **Temuan pada Exercise 8.2**
- Ketika waktu tunggu semaphore disetel ke nol, resource contention dapat terjadi.
- Hal ini menunjukkan pentingnya penyesuaian nilai `WaitTimeMilliseconds`.

### **Manfaat Penggunaan Semaphore**
- Membatasi pengaruh penguncian hanya pada resource bersama tertentu.
- Lebih efisien dibandingkan hanya mematikan interrupt, karena tidak mengganggu multitasking.

---

Dengan demikian, Exercise 8 memberikan pemahaman mendalam tentang bagaimana semaphores dapat digunakan untuk mengelola akses resource bersama dengan cara yang lebih efektif dan efisien dalam sistem multitasking.

