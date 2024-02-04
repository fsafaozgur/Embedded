
# Gömülü Sistemler Yazılım Temelleri ve Mikrodenetleyici Örnek Kodları

Mikrodenetleyici peripheral kullanımlarına örnek olarak verilen kodların tasarımı; 

-İşlemciden bağımsız çalışabilen, bir Main fonksiyonun yer aldığı dosya (main.c)

-İşlemciye özgü dosyaların yer aldığı BSP(Board Support Package) dosyaları (örneğin bsp.h ve bsp.c)

-Tasarladığımız bir sistem varsa (örneğin RTOS) bu sisteme ilişkin dosyalar (örneğin rtos.h ve rtos.c)

vb.

şeklinde modüler olan bir tasarım yapılmıştır ve böylece işlemci değiştirilmesi gerektiği zaman, yalnızca "bsp.h" içerisinde yer alan tanımlamalar(define), adresler(peripheral register adress vb.) ile "bsp.c" içerisinde yer alan ayarların(configuration), init işlemlerinin ve kullanılan fonksiyonların kod bloklarının yeni işlemciye göre tekrar yazılmasının yeterli olduğu bir tasarım yapılmıştır.
