


/*************************************************** MERMORY_MAPPING *****************************************************************/


/* -------------------------------- MERMORY_MAPPING_EXAMPLE_1 -----------------------------*/

#define PERIPH_ADDRESS		((uint32_t)0x40000400)				/* Peripheral base adress 0x40000400 olarak d�s�n�rsek (DataSheet deki Memory-Map den bakilabilir) */


#define __IO volatile																										
#define __I  volatile const 														  
	
/*
Volatile keyword�n� kullanarak compiler a, register lardaki degerlerin degisebilecegini ve optimize edilmemesi gerektigini s�yl�yoruz.
Bu sayede Runtime'da register larla her islem yaptigimizda, ilgili registere giderek o anki degeri okur 
*/ 
	


/* S�z konusu peripheralin 32bit lik 10 adet registeri oldugunu varsayarsak */
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
	
/* Bu 10 adet registere pointer aritmetigi ile ulasmayi inceleyelim, register3 i�in Offset degeri 0x08 dir */
(*(PERIPH_ADDRESS + 0x08)) |= 0x80u;         	//register3 isimli registerin ilgili bitlerine veri atamasi yapiliyor

/* Bu 10 adet registere peripheralin adresi ve typedef ile bir struct olusturarak, pointer access member operator yardimiyla ulasabiliriz*/
PERIPH->register3 |= 0x80u;            				//ikinci bir y�ntemle register3 isimli registerin ilgili bitlerine veri atamasi yapiliyor
																							// 0x40000400 base adress den baslamak �zere 4byte offsetler ile 10 eleman (register) bulundugu i�in pointer yardimiyla direk ulasim yapilabiliyor

	
/* ----------------------------- MERMORY_MAPPING_EXAMPLE_1_END ----------------------------*/	
	
	
	
	
/* -------------------------------- MERMORY_MAPPING_EXAMPLE_2 -----------------------------*/
	
	
/* GPIOA->ODR seklindeki t�m ifadelerin temel mantigi bu, GPIO_TypeDef adindaki bir typedef struct yardimiyla GPIOA 'nin base adresi �zerinden registerlerine ulasiliyor*/	

	
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)	
#define GPIOA               	((GPIO_TypeDef *) GPIOA_BASE)	
	
GPIOA burada, GPIO_TypeDef t�r�nden bir pointerdir ve access member operator ile t�m registerlerine ulasilabilir, �rnegin;
	
	
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

/* Yukaridaki kodda, compiler optimizasyonu High seviyede ise, compiler bu d�ng�n�n, programin hi�bir noktasina bir faydasi olmadigini d�s�ncegi i�in
		optimizasyon yapar yani s�z konusu d�ng�y� tamamen kaldirir, b�ylece sizin gecikme yapmak amaciyla koydugunuz kod yok olur, haliyle gecikme de saglanmaz
*/



/* Ancak kodu su sekilde d�zenlersek */

volatile int loop;

while (loop < 1000) {
	++loop;
}

/* Bu durumda loop degiskenine optimizasyon yapilmayacak ve b�ylece d�ng�, islevini g�rererek gecikme islemi saglanmis olacaktir */

/* ----------------------------- VOLATILE_EXAMPLE_1_END ----------------------------*/	




/* -------------------------------- VOLATILE_EXAMPLE_2 -----------------------------*/

#define GPIOA_PIN    (*((uint32_t *)0x40000400))

GPIOA_PIN = 0x80u;

while ( GPIOA_PIN ) 
{
		// TODO
}



/*  Compiler optimizasyonu High seviyede ise, s�z konusu koda farkli optimizasyonlar yapabilmektedir. Bunlardan birka�i;



		Ihtimal 1 - Yukaridaki kodda, compiler while d�ng�s�ndeki kontrol sartini her defasinda tekrar kontrol edip
		kaynak harcamamak adina bir �st satirdaki "GPIOA_PIN = 0x80u" atamasini alarak daha sonra g�rd�g� t�m GPIOA_PIN ifadeleri yerine 0x80 yazar ve b�ylece
		d�ng�deki kontrol sarti ( (0x80u) ) s�rekli true d�necegi i�in bunu "while(true)" olarak tekrar yazmis olacak ve b�ylece s�z konusu ifade;

while ( true ) 
{
		// TODO
}

	  seklinde optimize edilerek sonsuz d�ng� elde edilecektir. Esitlik her t�rl� saglandigi i�in bosyere tekrar tekrar kontrol yapmamak adina yapilan bu 
		optimizasyon, aksine programin akisini tamamen bozmakla sonu�lanacaktir. ��nk� programda sonsuz d�ng� istenmemektedir. 



		Ihtimal 2 - Compiler, GPIOA_PIN 'a atanan 0x80 degerini, kontrol� kolayca saglamak ve CPU yu daha az yormak amaciyla CPU Register'larindan birine(�rnegin R1)
		atayarak, bir sonraki kosul kontrol�nde direk olarak buradaki deger (R1 deki deger) ile kontrol yapmak isteyebilir. B�ylece GPIOA_PIN adresindeki degeri her 
	  defasinda adresten �ekmeden, direk olarak R1 registerindan alarak sistemi �ok hizli hale getirmektedir. Ancak, kaynak tasarrufu i�in yapilan bu optimizasyon 
		sonucu R1 'de s�rekli olarak 0x80 degeri tutuldugu i�in, d�ng�deki kontrol sarti ( (0x80u) ) s�rekli true d�necegi i�in bu "while(true)" anlamina gelmekte ve
		sonsuza kadar d�ng�n�n i�inde kalinmaktadir.

*/




/* Ancak kodu su sekilde d�zenlersek */

#define GPIOA_PIN    (*((volatile uint32_t *)0x40000400))
	
/* Bu durumda Runtime'da program, GPIOA_PIN ifadesini g�rd�g� heryerde GPIOA_PIN makrosundaki adrese giderek yeniden degeri alip while d�ng�s� i�indeki sarti
	 kontrol edecektir. Bunu yapmamizin sebebi GPIOA_PIN makrosu ile ifade edilen ve GPIOA pinlerinin konumlarini okudugumuz registera, kendi kodumuz harici bir
	 kaynaktan m�dahele edilebilecegidir. �rnegin bizim programimizda yer almayan ve harici bir tus ile bu pinlerin konumlarinin degistirilebildigi bir sistemde, 
	 Runtime zamaninda dis m�dahele ile pinlerin konumlarinda degisiklik yapilirsa ve 0x00 konumu elde edilirse (t�m pinler RESET konumuna alinirsa), haliyle 
	 "while(0x00)" durumu olusarak False bir ifade elde edilecek ve bu sayede while d�ng�s� sonlandirilabilecektir. Haliyle, volatile kullanilmasaydi while d�ng�s�
	 hi�bir zaman sonlandirilamayacakti.

*/




/* NOT: Normalde GPIOA_ODR 'a veri yazilarak GPIOA_IDR 'dan veri okunur, burada GPIOA_PIN den hem okuma yapilip hem yazilmasi �rnek a�isindan verilmistir.
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

/* 16 bit bir islemci ile �alistigimiz durumlarda */ 

uint16_t a, b;
uint32_t c;


int main ()
	
{

a= 45000;
b= 35000;
	
c=a+b;
	
/* Seklinde bir toplama yaptigimiz zaman ifadenin 80000 �ikmasini bekleriz ancak 14464 olarak �ikacaktir. 
	 Bunun sebebi s�z konusu sistemin 16 bit olarak toplama yapmasidir, haliyle deger 16 bitin �st�nde oldugundan tasar(overflow)
	 16 bitlik bir sayi 65536 farkli deger alabilir, o halde tasma miktari 80000-65536=14464 degeri elde edilmis olur.
	
	 Esitligin soluncaki c degiskeni 32 bit olarak verildiyse de, s�z konusu hesaplama islemi esitligin sagindaki degerlere g�re
	 yapildigi i�in sonu� olarak iki adet 16 bitlik sayi toplanarak 16 bitlik unsigned bir toplam ifadesi bulunmaktadir. 
*/
	
	
	
/* 32 bit bir islemci ile �alistigimiz durumlarda ise*/ 
	
c=a+b;
	
/* Yukaridaki islem otomatik olarak; */

	c=(uint32_t)a+(uint32_t)b
	
/*	sekline d�n�secegi i�in sonu� 80000 olacaktir.  */
	



/* O halde bizler islemcinin ka� bit ile �alistigindan bagimsiz kod yazmak istiyorsak (portable code) kodu asagidaki sekilde d�zenlemeliyiz; */

c=(uint32_t)a+b;

/* S�z  konusu kod ile b�y�k olan (32bitlik sayi) sayiya otomatik cast yapilacagi i�in s�z konusu islem 32bit'lik iki sayinin toplami seklinde olur */
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

/* S�z konusu islem daima "Sonuc-2" sonucunu verecektir. ��nk� s�z konusu islemci "-1" ifadesini unsigned ifadeye �evirecektir ve 0xFFFFFFFF 
	 ifadesine �evrilir ve bu sayi �ok b�y�kt�r, haliyle (80000 > 0xFFFFFFFF) sarti daima yanlis olacaktir.
*/




/* Bunu d�zeltmek i�in, -1 ifadesinin islemci tarafindan unsigned yapilmamasi gerekmektedir. O halde kodu d�zeltirsek; */

if ( (int32_t)c > -1)
{
	//Sonuc-1
}else
{
	//Sonuc-2
}


/* Bu defa iki signed sayinin karsilasmasi olacagi i�in, haliyle (80000 > -1 ) ifadesi dogru olacak ve "Sonuc-1" sonucu elde edilecektir. */ 
 

/* ----------------------------- SIZE_TYPE_CAST_EXAMPLE_2_END ----------------------------*/


/**************************************************** SIZE_TYPE_CAST_END ******************************************************************/


return 0;

}
