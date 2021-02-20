/* 
@file
BLM2512 2019-2020 BAHAR ODEV-2
Bu programda Single Linked List yapýsý kullanýlarak bir Huffman Tree tasarýmý yapýlmýþtýr.

@author
Ýsim: Ahmet Said SAÐLAM
Öðrenci No: 17011501
Tarih: 06.04.2020
E-Mail: l1117501@std.yildiz.edu.tr
Compiler: TDM-GCC 4.9.2 64 bit-Release
IDE: DEV-C++ (version 5.11)
Ýþletim Sistemi: Windows 10 Pro 64 bit
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#define LENGTH 50		//kullancidan elle alinabilecek maksimum string uzunlugu

//ayný zamanda hem single linked list hem de tree olabilecek yapi tanimi
typedef struct node {
	struct node* next;		//linkli listedeki sonraki eleman
	struct node* right;		//agacin sag yapragi
	struct node* left;		//agacin sol yapragi
	int count;				//karakterin kac kez tekrar ettigini tutan degisken
	char character;			//karakteri tutan degisken
} node;

//linked list basa eleman ekleme fonksiyonu
node* basaEkle(node* head, char val) {
	node* newnode;
	newnode = (node*) malloc(sizeof(node));
	newnode->character = val;
	newnode->count = 1;
	newnode->next = head;
	newnode->left = NULL;
	newnode->right = NULL;
	head = newnode;
	return head;
}

//linkli listedeki count deðerlerine göre araya elemen ekleme fonksiyonu
//agac olustruruken kullanilacagi icin sag ve sol node'lar ayarlanir
node* arayaEkle(node* head, node* temp, int toplam) {		//temp düðümü linkli listeye kendisinden sonra araya eleman eklenecek düðümdür.
	node* newnode;
	newnode = (node*) malloc(sizeof(node));
	newnode->count = toplam;
	newnode->character = 254;
	newnode->next = temp->next;
	temp->next = newnode;
	newnode->left = head;
	newnode->right = head->next;
	return head;
}

//linkli listedeki count deðerlerine göre sona eleman ekleme fonksiyonu
//agac olustruruken kullanilacagi icin sag ve sol node'lar ayarlanir
node* sonaEkle(node* head, node* curr, int toplam) {		//curr = linkli listenin mevcut haldeki son düðümü
	node* newnode;
	newnode = (node*) malloc(sizeof(node));
	newnode->count = toplam;
	newnode->character = 254;
	newnode->next = NULL;
	curr->next = newnode;
	newnode->left = head;
	newnode->right = head->next;
	return head;
}

//linkli listenin bastan ilk iki elemanini silen fonksiyon (silme isleminde node'lar free edilmez cunku daha sonradan agacin dallari olacaklardir.)
node* bastanSilme(node* head) {
	node* curr;
	curr = head;			//ilk durumdaki listenin bastan ilk elemanýný silme asamasi
	head = head->next;
	curr = head;			//ilk durumdaki listenin bastan ikinci elemanýný silme asamasi
	head = head->next;
	return head;
}

//linkli listeye disaridan gelen karakter mevcutta var mi diye kontrol eden fonksiyon
node* arama(node* head, char val) {
	int bulgu = 1;
	node* curr = head;
	node* temp;
	while(bulgu != 0 && curr->next !=NULL) {
		if(curr->character == val){
			bulgu = 0;
		}
		temp = curr;
		curr = curr->next;
	}
	if(bulgu == 0) {
		return temp;		//bas node veya ara nodelardan biri esit ise disariya adresi dondurulur
	}
	else {
		if(curr->character == val){
			return curr;	//son eleman esit ise disariya son elemanin adresi dondurulur
		}
		return NULL;		//mevcut degilse NULL dondurulur
	}
}

void siraliEkle(node**, node*);

//insertion sort yontemiyle linkli listeyi kucukten buyuge siralayan fonksiyon
void insertionSort(node** head) {
	node* sirali = NULL;
	node* current = *head;
	while(current != NULL) {
		node* next = current->next;
		siraliEkle(&sirali, current);
		current = next;
	}
	*head = sirali;			//head isaretcisi yeni linkli listenin head node'unu gosterecek sekilde ayarlanir.
}

//insertionSort fonksiyonu icinde cagirilarak linkli listeyi yeniden olusturarak siralar
void siraliEkle(node** head, node* newnode) {
	node* current;
	if(*head == NULL || (*head)->count >= newnode->count) {	//ilk durumda head  null oldugunda veya eklencek node'un count degeri head node'unkinden daha kucukse basa eklenir.
		newnode->next = *head;
		*head = newnode; 
	}
	else {													
		current = *head;
		while(current->next != NULL && current->next->count < newnode->count) {	//yeni eklenecek node'un count degeri, listedeki ilk olarak hangi node'dan daha buyukse bulmak icin while dongusu
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}

//linkli listeyi ekrana yazdiran fonksiyon
void printList(node* head) {
	node* temp = head;
	while (temp->next != NULL) {
		printf("%c,%d -> ",temp->character,temp->count);
		temp = temp->next;
	}
	printf("%c,%d",temp->character,temp->count);

}

//huffman tree'nin yuksekligini hesaplayan fonksiyon
int height(node* head) {
	if(head == NULL) {
		return 0;
	}
	else{
		int leftDepth = height(head->left);
		int rightDepth = height(head->right);
		if(leftDepth > rightDepth) {
			return (leftDepth+1);
		}
		else {
			return (rightDepth+1);
		}
	}
}

//agacin ilgili seviyesini ekrana yazdiran fonksiyon
void printLevel(node* head, int level) {
	if(head == NULL) {
		printf(" ");
		return;
	}
	if(level == 1) {
		printf("%d'%c' ",head->count,head->character);
	}
	else if(level > 1) {
		printLevel(head->left, level-1);
		printLevel(head->right, level-1);

	}
}

//agaci butun seviyeleriyle birlikte kokten yapraklara kadar ekrana yazdiran fonksiyon
void printTree(node* head) {
	int yukseklik = height(head);
	int i;
	for (i=1; i<=yukseklik; i++) {
		printLevel(head,i);
		printf("\n");
	}
}

//gets() fonksiyonu kullanýlmadan önce eger scanf kullanýldýysa gets() duzgun calismayacagi icin input buffer'i temizlemmek icin gerekli fonksiyon
int clear_input_buffer(void) {
    int ch;
    while (((ch = getchar()) != EOF) && (ch != '\n'));
    return ch;
}

//post-traverse kullanarak agaci memory'den temizleyen fonksiyon
void deleteTree(node* head) {
	if(head == NULL){
		return;
	}
	deleteTree(head->left);
	deleteTree(head->right);
	free(head);
}

int main() {
	FILE *fp;				//file pointer
	int i;					//for dongusu icin indis tanimi
	int cls;				//program sonlanirken agaci temizlemek veya oldugu gibi birakmak durumunu kontrol eden degisken
	int toplam;				//linkli listenin dugumlerinden agac olustururken, iki dugumun count degerlerinin toplamini tutup yeni node'a count degeri olarak atanmasini saglayan degisken
	int tur;				//girdinin dosyadan mi yoksa kullanicidan elle mi alinacagini kontrol eden degisken
	char string[LENGTH];	//girdinin kullanicidan elle alinmasi durumunda bu veriyi saklayan karakter dizisi
	char val;				//linkli listenin her bir dugumunde bulunan karakteri disaridan veren degisken
	char filename[50];		//girdinin dosyadan okunmasi durumunda dosya ismini tutacak olan karakter dizisi
	node* head = NULL;		//linkli listenin ilk dugumu ilklendirilir
	node* curr;				//
	node* prev;				//linkli listede ve agac olustururken kullaniacak olan dugum isaret eden gecici degiskenler
	node* temp;				//
	printf("Metni dosyadan almak icin 1'e, elle girmek icin 2'ye basiniz.\n");
	scanf("%d",&tur);
	if(tur == 1) {							//metin dosyadan alinir
		printf("Lutfen acmak istediginiz dosyanin ismini uzantisiyla birlikte giriniz.\n");
		scanf("%s",filename);
		if((fp = fopen(filename,"r")) == NULL) {
			printf("Dosya acilamadi!\n");
			return 0;
		}
		else {
			val = fgetc(fp);
			head = basaEkle(head,val);
			while(!feof(fp)) {				//dosyadaki string karakter karakter okunur ve linkli liste olusturulur
				val = fgetc(fp);
				if(val != EOF && val != 10) {
					temp = arama(head,val);
					if(temp == NULL) {
						head = basaEkle(head,val);
					}
					else {
						temp->count++;
					}
				}
			}
			fclose(fp);
		}
	}
	else if(tur == 2) {							//metin kullanicidan elle alinir
		printf("String giriniz.\n");
		clear_input_buffer();					//input buffer temizlenir
		gets(string);
		val = string[0];
		head = basaEkle(head,val);
		for(i=1; i<strlen(string); i++){			//elle girilen string karakter karakter okunur ve linkli liste olusturulur
			val = string[i];
			temp = arama(head,val);
			if(temp == NULL){
				head = basaEkle(head,val);
			}
			else {
				temp->count++;
			}
		}	
	}
	else {
		printf("Hatali Giris Yaptiniz!\nProgram Sonlandiriliyor...\n");  //veri alma seklini belirlemede hatali kullanici girisi
		return 0;														//program sonlanir
	}
	printf("\nLinkli Liste:\n\n");				//
	printList(head);							//
	printf("\n\n");								// Linkli liste önce oldugu gibi sonra da sirali sekilde ekrana yazdirilir.
	insertionSort(&head);						//
	printf("Sirali Linkli Liste:\n\n");			//
	printList(head);							//
	
	//linkli listeden agaci olusturan blok...
	while(head->next != NULL) {
		curr = head;
		toplam = head->count + head->next->count;
		while(toplam >= curr->count && curr->next != NULL) {
			prev = curr;
			curr = curr->next;
		}
		if(curr->next == NULL) {
			head = sonaEkle(head,curr,toplam);
			head = bastanSilme(head);
		}
		else {
			head = arayaEkle(head,prev,toplam);
			head = bastanSilme(head);
		}
	}
	//...linkli listeden agaci olusturan blok
	
	printf("\n-----------------------------------------\n\nHuffman Tree : \n\n");
	printTree(head);
	printf("\nHuffman Tree'yi temizlemek icin 1'e, oldugu gibi birakmak icin 2'ye basiniz.\n");
	scanf("%d",&cls);
	if(cls == 1) {
		printf("Huffman Tree temizleniyor...\n");
		deleteTree(head);
		printf("Huffman Tree temizlendi!");
	}
	else {
		printf("\nHuffman Tree:\n");
		printTree(head);
	}
	return 0;
}






