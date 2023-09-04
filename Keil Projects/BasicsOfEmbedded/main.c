


/*************************************************** MERMORY_MAPPING *****************************************************************/


/* -------------------------------- MERMORY_MAPPING_EXAMPLE_1 -----------------------------*/

#define PERIPH_ADDRESS		((uint32_t)0x40000400)				/* Peripheral base adress 0x40000400 olarak düsünürsek (DataSheet deki Memory-Map den bakilabilir) */


#define __IO volatile																										
#define __I  volatile const 														  
	
/*
Volatile keywordünü kullanarak compiler a, register lardaki degerlerin degisebilecegini ve optimize edilmemesi gerektigini söylüyoruz.
Bu sayede Runtime'da register larla her islem yaptigimizda, ilgili registere giderek o anki degeri okur 
*/ 
	


/* Söz konusu peripheralin 32bit lik 10 adet registeri oldugunu varsayarsak */
typedef struct 
{

    __IO  uint32_t register1;		
    __IO  uint32_t register2;
    __IO  uint32_t register3; //offset 0x08
    __I   uint32_t register4;
    __IO  uint32_t register5;
    __IO  uint32_t register6;
    __I   uint32_t register7;
    __IO  uint32_t register8;
		__I  	uint32_t register9;
    __IO  uint32_tregister10;
    
}Periph;                                


#define PERIPH		((Periph *) PERIPH_ADDRESS)


int main()
{
	
/* Bu 10 adet registere pointer aritmetigi ile ulasmayi inceleyelim, register3 için Offset degeri 0x08 dir */
(*(PERIPH_ADDRESS + 0x08)) |= 0x80u;         	//register3 isimli registerin ilgili bitlerine veri atamasi yapiliyor

/* Bu 10 adet registere peripheralin adresi ve typedef ile bir struct olusturarak, pointer access member operator yardimiyla ulasabiliriz*/
PERIPH->register3 |= 0x80u;            				//ikinci bir yöntemle register3 isimli registerin ilgili bitlerine veri atamasi yapiliyor
																							// 0x40000400 base adress den baslamak üzere 4byte offsetler ile 10 eleman (register) bulundugu için pointer yardimiyla direk ulasim yapilabiliyor

	
/* ----------------------------- MERMORY_MAPPING_EXAMPLE_1_END ----------------------------*/	
	
	
	
	
/* -------------------------------- MERMORY_MAPPING_EXAMPLE_2 -----------------------------*/
	
	
/* GPIOA->ODR seklindeki tüm ifadelerin temel mantigi bu, GPIO_TypeDef adindaki bir typedef struct yardimiyla GPIOA 'nin base adresi üzerinden registerlerine ulasiliyor*/	

	
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)	
#define GPIOA               	((GPIO_TypeDef *) GPIOA_BASE)	
	
GPIOA burada, GPIO_TypeDef türünden bir pointerdir ve access member operator ile tüm registerlerine ulasilabilir, örnegin;
	
	
GPIOA->ODR |= (1<<3)
GPIOA->MODER |= (1<<3)
GPIOA->PUPDR |= (1<<3)

/* ----------------------------- MERMORY_MAPPING_EXAMPLE_2_END ----------------------------*/


/*************************************************** MERMORY_MAPPING_END *****************************************************************/










/******************************************************* VOLATILE ************************************************************************/


/* -------------------------------- VOLATILE_EXAMPLE_1 -----------------------------*/


int loop;

while (loop < 1000) {
	++loop;
}

/* Yukaridaki kodda, compiler optimizasyonu High seviyede ise, compiler bu döngünün, programin hiçbir noktasina bir faydasi olmadigini düsüncegi için
		optimizasyon yapar yani söz konusu döngüyü tamamen kaldirir, böylece sizin gecikme yapmak amaciyla koydugunuz kod yok olur, haliyle gecikme de saglanmaz
*/



/* Ancak kodu su sekilde düzenlersek */

volatile int loop;

while (loop < 1000) {
	++loop;
}

/* Bu durumda loop degiskenine optimizasyon yapilmayacak ve böylece döngü, islevini görererek gecikme islemi saglanmis olacaktir */

/* ----------------------------- VOLATILE_EXAMPLE_1_END ----------------------------*/	




/* -------------------------------- VOLATILE_EXAMPLE_2 -----------------------------*/

#define GPIOA_PIN    (*((uint32_t *)0x40000400))

GPIOA_PIN = 0x80u;

while ( GPIOA_PIN ) 
{
		// TODO
}



/*  Compiler optimizasyonu High seviyede ise, söz konusu koda farkli optimizasyonlar yapabilmektedir. Bunlardan birkaçi;



		Ihtimal 1 - Yukaridaki kodda, compiler while döngüsündeki kontrol sartini her defasinda tekrar kontrol edip
		kaynak harcamamak adina bir üst satirdaki "GPIOA_PIN = 0x80u" atamasini alarak daha sonra gördügü tüm GPIOA_PIN ifadeleri yerine 0x80 yazar ve böylece
		döngüdeki kontrol sarti ( (0x80u) ) sürekli true dönecegi için bunu "while(true)" olarak tekrar yazmis olacak ve böylece söz konusu ifade;

while ( true ) 
{
		// TODO
}

	  seklinde optimize edilerek sonsuz döngü elde edilecektir. Esitlik her türlü saglandigi için bosyere tekrar tekrar kontrol yapmamak adina yapilan bu 
		optimizasyon, aksine programin akisini tamamen bozmakla sonuçlanacaktir. Çünkü programda sonsuz döngü istenmemektedir. 



		Ihtimal 2 - Compiler, GPIOA_PIN 'a atanan 0x80 degerini, kontrolü kolayca saglamak ve CPU yu daha az yormak amaciyla CPU Register'larindan birine(örnegin R1)
		atayarak, bir sonraki kosul kontrolünde direk olarak buradaki deger (R1 deki deger) ile kontrol yapmak isteyebilir. Böylece GPIOA_PIN adresindeki degeri her 
	  defasinda adresten çekmeden, direk olarak R1 registerindan alarak sistemi çok hizli hale getirmektedir. Ancak, kaynak tasarrufu için yapilan bu optimizasyon 
		sonucu R1 'de sürekli olarak 0x80 degeri tutuldugu için, döngüdeki kontrol sarti ( (0x80u) ) sürekli true dönecegi için bu "while(true)" anlamina gelmekte ve
		sonsuza kadar döngünün içinde kalinmaktadir.

*/




/* Ancak kodu su sekilde düzenlersek */

#define GPIOA_PIN    (*((volatile uint32_t *)0x40000400))
	
/* Bu durumda Runtime'da program, GPIOA_PIN ifadesini gördügü heryerde GPIOA_PIN makrosundaki adrese giderek yeniden degeri alip while döngüsü içindeki sarti
	 kontrol edecektir. Bunu yapmamizin sebebi GPIOA_PIN makrosu ile ifade edilen ve GPIOA pinlerinin konumlarini okudugumuz registera, kendi kodumuz harici bir
	 kaynaktan müdahele edilebilecegidir. Örnegin bizim programimizda yer almayan ve harici bir tus ile bu pinlerin konumlarinin degistirilebildigi bir sistemde, 
	 Runtime zamaninda dis müdahele ile pinlerin konumlarinda degisiklik yapilirsa ve 0x00 konumu elde edilirse (tüm pinler RESET konumuna alinirsa), haliyle 
	 "while(0x00)" durumu olusarak False bir ifade elde edilecek ve bu sayede while döngüsü sonlandirilabilecektir. Haliyle, volatile kullanilmasaydi while döngüsü
	 hiçbir zaman sonlandirilamayacakti.

*/




/* NOT: Normalde GPIOA_ODR 'a veri yazilarak GPIOA_IDR 'dan veri okunur, burada GPIOA_PIN den hem okuma yapilip hem yazilmasi örnek açisindan verilmistir.
	 Olmasi gereken kod;


	GPIOA_ODR |= 0x80u;
	while ( GPIOA_IDR & 0x80 ) 
	{
			// TODO
	}

	seklindedir.

*/


/* ----------------------------- VOLATILE_EXAMPLE_2_END ----------------------------*/


/***************************************************** VOLATILE_END **********************************************************************/






/***************************************************** SIZE_TYPE_CAST ********************************************************************/




/* -------------------------------- SIZE_TYPE_CAST_EXAMPLE_1 -----------------------------*/

/* 16 bit bir islemci ile çalistigimiz durumlarda */ 

uint16_t a, b;
uint32_t c;


int main ()
	
{

a= 45000;
b= 35000;
	
c=a+b;
	
/* Seklinde bir toplama yaptigimiz zaman ifadenin 80000 çikmasini bekleriz ancak 14464 olarak çikacaktir. 
	 Bunun sebebi söz konusu sistemin 16 bit olarak toplama yapmasidir, haliyle deger 16 bitin üstünde oldugundan tasar(overflow)
	 16 bitlik bir sayi 65536 farkli deger alabilir, o halde tasma miktari 80000-65536=14464 degeri elde edilmis olur.
	
	 Esitligin soluncaki c degiskeni 32 bit olarak verildiyse de, söz konusu hesaplama islemi esitligin sagindaki degerlere göre
	 yapildigi için sonuç olarak iki adet 16 bitlik sayi toplanarak 16 bitlik unsigned bir toplam ifadesi bulunmaktadir. 
*/
	
	
	
/* 32 bit bir islemci ile çalistigimiz durumlarda ise*/ 
	
c=a+b;
	
/* Yukaridaki islem otomatik olarak; */

	c=(uint32_t)a+(uint32_t)b
	
/*	sekline dönüsecegi için sonuç 80000 olacaktir.  */
	



/* O halde bizler islemcinin kaç bit ile çalistigindan bagimsiz kod yazmak istiyorsak (portable code) kodu asagidaki sekilde düzenlemeliyiz; */

c=(uint32_t)a+b;

/* Söz  konusu kod ile büyük olan (32bitlik sayi) sayiya otomatik cast yapilacagi için söz konusu islem 32bit'lik iki sayinin toplami seklinde olur */
}



/* ----------------------------- SIZE_TYPE_CAST_EXAMPLE_1_END ----------------------------*/






/* -------------------------------- SIZE_TYPE_CAST_EXAMPLE_2 -----------------------------*/


uint16_t a, b;
uint32_t c;


int main ()
	
{

a= 45000;
b= 35000;
	
c=(uint32_t)a+b;

	
if ( c > -1)
{
	//Sonuc-1
}else
{
	//Sonuc-2
}

/* Söz konusu islem daima "Sonuc-2" sonucunu verecektir. Çünkü söz konusu islemci "-1" ifadesini unsigned ifadeye çevirecektir ve 0xFFFFFFFF 
	 ifadesine çevrilir ve bu sayi çok büyüktür, haliyle (80000 > 0xFFFFFFFF) sarti daima yanlis olacaktir.
*/




/* Bunu düzeltmek için, -1 ifadesinin islemci tarafindan unsigned yapilmamasi gerekmektedir. O halde kodu düzeltirsek; */

if ( (int32_t)c > -1)
{
	//Sonuc-1
}else
{
	//Sonuc-2
}


/* Bu defa iki signed sayinin karsilasmasi olacagi için, haliyle (80000 > -1 ) ifadesi dogru olacak ve "Sonuc-1" sonucu elde edilecektir. */ 
 

/* ----------------------------- SIZE_TYPE_CAST_EXAMPLE_2_END ----------------------------*/


/**************************************************** SIZE_TYPE_CAST_END ******************************************************************/


return 0;

}
