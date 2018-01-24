#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
	
	
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int iloscMiast = 0;
    int iloscOdleglosci = 0;
    int dlugoscNazwyMiast = 0;
    
    char **tblmiasta;
    int *tblOdleglosci;
    
    char answer;
    
    int najmniejszaIloscKilometrow = 0;
    int * optymalnaDroga;
    int optymalnePierwsze;

void policzMiasta(){
	// Liczenie MIAST 
	// otwarcie pliku z miastami
	FILE * miast;
	
	miast = fopen("miasta.txt", "r");
	if (miast == NULL){
		printf("Blad wczytywania pliku miasta.txt");
		exit(EXIT_FAILURE);
	}
	// liczenie ilosci miast oraz maksymalnej dlugosci nazwy
	while ((read = getline(&line, &len, miast)) != -1) {
		if(read > dlugoscNazwyMiast){
			dlugoscNazwyMiast = read;
		}
		iloscMiast++;
	}
	// zamykamy plik z miastami
	fclose(miast);
}
	
void wczytajMiasta(){
	FILE * miast;
	// wczytywanie miast
	// tworzenie tablicy dwuwymiarowej na nazwy miejscowosci
	int i;
	int counter = 0;
	tblmiasta =(char**)malloc(sizeof(char*) * iloscMiast);
	
	for (i = 0; i < iloscMiast; i++){
		tblmiasta[i] = (char*)malloc(sizeof(char*) * dlugoscNazwyMiast);
	}
	
	miast = fopen("miasta.txt", "r");
    if (miast == NULL){
		printf("Blad wczytywania pliku miasta.txt");
        exit(EXIT_FAILURE);
	}
	
	
	while ((read = getline(&line, &len, miast)) != -1) {
		
		char  lineTmp[100];
		int len = strlen(line); 
		strncpy(lineTmp,line,len-1);
		strcpy(tblmiasta[counter],lineTmp);
		 memset(lineTmp, 0, 100);
		counter++;
    }
    fclose(miast);
    
    printf("* Wczytano %d miast.\n",iloscMiast);
    
}

void drukujMiasta(){
	int i;
	printf("Wczytane miasta: (%d) \n\n",iloscMiast);
	printf("ID | NAZWA MIEJSCOWOSCI\n");
	printf("---|------------------------\n");
    for (i = 0; i<iloscMiast;i++){
		if(i < 10){ printf(" "); }
		printf("%d | ",i);
		printf("%s\n",tblmiasta[i]);
	}
}
	
void wczytajOdleglosci(){
	// ODLEGLOSCI 
	FILE * odl;
	int i, j;
	
	odl = fopen("odleglosci.txt", "r");
    if (odl == NULL){
        printf("Blad wczytywania pliku odleglosci.txt");
	}
	
	// liczenie ilosci odleglosci
    while ((read = getline(&line, &len, odl)) != -1) {
		iloscOdleglosci++;
    }
    fclose(odl);
    
    tblOdleglosci = (int *)malloc(iloscMiast * iloscMiast * sizeof(int));
    
    
    int count = 0;
    for (i = 0; i <  iloscMiast; i++){
      for (j = 0; j < iloscMiast; j++){
         *(tblOdleglosci + i*iloscMiast + j) = ++count;
	  }
    }
 
	odl = fopen("odleglosci.txt", "r");
    if (odl == NULL){
        printf("Blad wczytywania pliku odleglosci.txt");
	}
	
	char *minus = "  -  ";
	char *dwukropek = " : ";
	char *nawiasStop = "),";
	
	char miastoZrodlowe[3];
	char miastoDocelowe[3];
	char odlegloscMiedzyMiastami[3];
	
	int miastoZrodloweInt;
	int miastoDoceloweInt;
	int odlegloscMiedzyMiastamiInt;
	
	int dlugoscWpisuZ;
	int dlugoscWpisuD;
	int dlugoscWpisuO;
	
    while ((read = getline(&line, &len, odl)) != -1) {
		//printf("%s", line);
				
		dlugoscWpisuZ = strstr(line,minus) - line; // odejmujemy od wskaznika poczatku wystapienia minusa wskaznik poczatku calego stringu, i otrzymujemy wynik jaki przesuniecie
		dlugoscWpisuZ = dlugoscWpisuZ - 2; // odejmujemy ilosc znakow od poczatku wiersza
		//printf("%ld\n",  dlugoscWpisu);
		strncpy(miastoZrodlowe, line+2, dlugoscWpisuZ); // kopiujemy interesujaca nas wartosc
		miastoZrodloweInt = atoi(miastoZrodlowe)-1 ; // konwertujemy ja na int
		//printf("Z: %d ",miastoZrodloweInt); 
		
		
		dlugoscWpisuD = strstr(line,dwukropek) - line;
		dlugoscWpisuD = dlugoscWpisuD - (7 + dlugoscWpisuZ);
		//printf("dlugosc d: %ld\n",  dlugoscWpisuD );
		strncpy(miastoDocelowe, line+7+dlugoscWpisuZ, dlugoscWpisuD);
		miastoDoceloweInt = atoi(miastoDocelowe)-1 ;
		//printf("do %d ",miastoDoceloweInt);
		
		dlugoscWpisuO = strstr(line,nawiasStop) - line;
		dlugoscWpisuO = dlugoscWpisuO - (10 + dlugoscWpisuZ + dlugoscWpisuD); // 11 bo tyle stalych znakow 
		
		//printf("Wycinam zaczynajac od-%s-",line+11+dlugoscWpisuZ+dlugoscWpisuD );
		//printf("wycinam znakow: %d\n",  dlugoscWpisuO );
		
		strncpy(odlegloscMiedzyMiastami, line+10+dlugoscWpisuZ+dlugoscWpisuD, dlugoscWpisuO);
		//printf("%d",odlegloscMiedzyMiastami);
		odlegloscMiedzyMiastamiInt = atoi(odlegloscMiedzyMiastami);
		//printf("jest km: %d\n\n",odlegloscMiedzyMiastamiInt);
		
		//*tblOdleglosci[miastoZrodloweInt][miastoDoceloweInt] = odlegloscMiedzyMiastamiInt;
		*(tblOdleglosci + miastoZrodloweInt*iloscMiast + miastoDoceloweInt) = odlegloscMiedzyMiastamiInt;
		
    }
    printf("* Wczytano %d odleglosci.\n",iloscOdleglosci);
}

void drukujOdleglosci(){
    int i,j;
	printf("     ");
	for (i = 0; i <  iloscMiast; i++){
		printf(" %02d ", i);
	  }
	printf("\n");
	
	for (i = 0; i <=  iloscMiast; i++){
		printf("----");
		
	}
	printf("\n");
 
    for (i = 0; i <  iloscMiast; i++){
	  printf("%02d | ",i);
      for (j = 0; j < iloscMiast; j++){
         printf("%03d ", *(tblOdleglosci + i*iloscMiast + j));
	  }
	  printf("\n");
	}
}
	
void czyscPamiec(){
	int i;
    for (i = 0; i < iloscMiast; i++){
		free(tblmiasta[i]) ;
	}
	free(tblmiasta);
	free(tblOdleglosci);
}


void drukujMenu(){
printf("Menu:\n");
printf("m - wypisz miejscowosci | o - wypisz odleglosci | k - oblicz Trase | Q - wyjście z programu! \n");
}


void drukujPowitanie(){
printf("\n**************************************\n");	
printf("* Program KURIER wersja 1.0          *");
printf("\n**************************************\n");	
}

void wczytajOdpowiedz(){
printf("?> ");
scanf(" %c", &answer);
}

void print(int *num, int n, int pierwsze){
	int i;
	int sumaKilometrow = 0;
	int aktualneKilometry = 0;
	
	//printf("Aktualnie rozpatrywana trasa:\n");
    
    //printf("%s - ", tblmiasta[pierwsze]);
    aktualneKilometry = *(tblOdleglosci + pierwsze*iloscMiast + num[0]);
    sumaKilometrow += aktualneKilometry;
    //printf("(%d) -> ",aktualneKilometry);
    for ( i = 0 ; i < n ; i++){
        //printf("%s - ", tblmiasta[num[i]]);
        if((i+1) < n){
			aktualneKilometry = *(tblOdleglosci + num[i]*iloscMiast + num[i+1]);
			sumaKilometrow += aktualneKilometry;
			//printf("(%d) -> ",aktualneKilometry);
		} else {
			aktualneKilometry = *(tblOdleglosci + num[i]*iloscMiast + pierwsze);
			sumaKilometrow += aktualneKilometry;
			//printf("(%d) -> ",aktualneKilometry);
		}
	}
	//printf("%s\n", tblmiasta[pierwsze]);
	
	if(najmniejszaIloscKilometrow == 0){
		najmniejszaIloscKilometrow = sumaKilometrow;
		memcpy(optymalnaDroga,num, n * sizeof(int));
		optymalnePierwsze = pierwsze;
	} else if(najmniejszaIloscKilometrow > sumaKilometrow){
		najmniejszaIloscKilometrow = sumaKilometrow;
		memcpy(optymalnaDroga,num, n * sizeof(int));
		optymalnePierwsze = pierwsze;
		
	}
	
	//printf("Suma kilometrow: %d\n",sumaKilometrow);
	
    //printf("\n");
}



void obliczTrase(){
	najmniejszaIloscKilometrow = 0;
	
	
	int przezTyleMiast;
	int * tblMiastaDoOdwiedzenia;
	int pierwszeMiasto;
	int i,j, temp;
	
	printf("Podaj liczbe miast przez ktore ma przejechac kurier\n?> ");
    scanf("%d", &przezTyleMiast);
    printf("Jedziesz przez %d miast\n", przezTyleMiast);
    
    optymalnaDroga = (int*)malloc(sizeof(int*) * przezTyleMiast-1);
    
    tblMiastaDoOdwiedzenia = (int *)malloc((przezTyleMiast-1) * sizeof(int));
    
    printf(" *************************************************************************** \n");
	printf(" !!! Pierwsze miasto ktore podasz jest miastem poczatkowym oraz koncowym !!! \n");
	printf(" *************************************************************************** \n");
    printf("Podaj ID miasta startowego\n");
	printf("?> ");
	scanf("%d", &pierwszeMiasto );
	
    for(i = 0; i<przezTyleMiast-1; i++){
		printf("Podaj ID miasta %d\n", (i+1));
		printf("?> ");
		scanf("%d", &tblMiastaDoOdwiedzenia[i] );
    }
    
    printf("Jedziesz przez: \n");
    printf("START - %s\n",tblmiasta[pierwszeMiasto]);
    for(i = 0; i<przezTyleMiast-1; i++){
		printf("%d) ",(i+1));
		printf("%s\n",tblmiasta[tblMiastaDoOdwiedzenia[i]]);
    }
    printf("KONIEC - %s\n",tblmiasta[pierwszeMiasto]);
    
    printf("\nSzukam najlepszej trasy \n");
    
    for (j = 1; j <= przezTyleMiast-1; j++) {
        for (i = 0; i < przezTyleMiast-2; i++) {
            temp = tblMiastaDoOdwiedzenia[i];
            tblMiastaDoOdwiedzenia[i] = tblMiastaDoOdwiedzenia[i+1];
            tblMiastaDoOdwiedzenia[i+1] = temp;
            print(tblMiastaDoOdwiedzenia, przezTyleMiast-1,pierwszeMiasto);
		}
	}
    
    printf("\nSugerowana trasa to: \n");
    
    int sumaKilometrow = 0;
	int aktualneKilometry = 0;
	
    printf("%s --", tblmiasta[optymalnePierwsze]);
    aktualneKilometry = *(tblOdleglosci + optymalnePierwsze*iloscMiast + optymalnaDroga[0]);
    sumaKilometrow += aktualneKilometry;
    printf("(%d)--> ",aktualneKilometry);
    for ( i = 0 ; i < przezTyleMiast-1 ; i++){
        printf("%s --", tblmiasta[optymalnaDroga[i]]);
        if((i+1) < przezTyleMiast-1){
			aktualneKilometry = *(tblOdleglosci + optymalnaDroga[i]*iloscMiast + optymalnaDroga[i+1]);
			sumaKilometrow += aktualneKilometry;
			printf("(%d)--> ",aktualneKilometry);
		} else {
			aktualneKilometry = *(tblOdleglosci + optymalnaDroga[i]*iloscMiast + optymalnePierwsze);
			sumaKilometrow += aktualneKilometry;
			printf("(%d)--> ",aktualneKilometry);
		}
	}
	printf("%s\n", tblmiasta[optymalnePierwsze]);
    printf("Suma kilometrow na trasie: %d\n",najmniejszaIloscKilometrow);
    
    printf("\n Wracam do glownego menu \n");
    free(tblMiastaDoOdwiedzenia);
    free(optymalnaDroga);

}

int main() {
	drukujPowitanie();
	policzMiasta();
	wczytajMiasta();
	wczytajOdleglosci();
	drukujMenu();
	wczytajOdpowiedz();
	
    while( answer != 'Q' ) {
		
	 switch (answer) {
       case 'm': drukujMiasta();
         break;
       case 'o': drukujOdleglosci();
         break;
	   case 'k': obliczTrase();
	     break;
	 }
	 drukujMenu();
	 wczytajOdpowiedz();
   }
   
   return 0;
}
