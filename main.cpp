// =========== Program Scanner ==============
// Kelas 	: IF - 11 / TEKKOM - 11
// Kelompok : 10116467 - Esa M
//			  10116474 - Gerdi AS
//			  10116490 - M Fajar M
//			  10116503 - Adisri YA
//			  10116502 - Ken Wite
// ==========================================


//Library yang dibutuhkan
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <cstdio>

//Dengan menggunakan kata kunci typedef, kita dapat mendefinisikan alias struktur atau Deklarasi Structur tipe data.
typedef struct t1tag{
char nm[20];
} t1;

//Pendeklarasian type bahasa
t1 Typs[] = {"keyword", "operator", "identifier", "number", "string", "illegal character",
"assignment", "paranthesis", "end of statement", "relational operator", "array start", "array end", "seperator", "Unary operator",
"compiler directive", "converter", "range"};

// Pendeklarasian bahasa yang diterima oleh bahasa Pascal
t1 Rsrv[] = {"BEGIN","END","IF","THEN","ELSE","DO","FOR","TO","WHILE","REPEAT",
"UNTIL","PROCEDURE","FUNCTION","INTEGER","REAL","BYTE","CHAR",
"TEXT", "ARRAY","VAR","RECORD","SET","TYPE","AND","CASE","CONST",
"DIV", "DOWNTO","EXTERNAL","FILE","FORWARD","GOTO","IN","INLINE",
"INTERRUPT", "LABEL","MOD","NIL","NOT","OF","OR","PACKED","PROGRAM",
"WITH", "BOOLEAN","INPUT","OUTPUT", "WRITE", "WRITELN" };
long s = 49;

//Pendeklarasian operator dan simbol yang diterima oleh bahasa Pascal
char oprtr[] = { '.', ',', ';', '=', ':', '[', ']', '(', ')', '-', '+', '/', '*','>', '<', '$', '#', '@'};
long t = 18;

//Deklarasi Structur tipe data dengan library typedef
typedef struct ttyptg{
	long no;
	char ctkn[50];
	int typ;
	long lline;
} ttyp;

//Pengecekan Keyword sesuai dengan keyword yang dapat diterima pascal
int isReserved(char tmp[]){
	long i=0;
	char *str;

	str = _strupr( _strdup( tmp ) );//mengubah semua karakter huruf yang terdapat pada suatu variabel menjadi huruf besar semua

	while ( (i < s) && ( strcmp(str, Rsrv[i].nm) != 0) ){
		i++;
	}
	if ( strcmp(str, Rsrv[i].nm) != 0) // Cek jika tidak sama dengan Keyword
		return 1;

		return 0;
}

//Program Utama
int main(){
	long i, j, ind, zz, N, NN, ln;
	char ch, nxt, instr[1024], tmp[100], FName[256];
	ttyp Token[1024];
	FILE *f;

	printf("Masukkan Nama dari File Source Pascal (.pas) yang akan di scan: \n");
	scanf("%s", FName);

	f = fopen(FName,"r");


		//Mengecek End of file dari file pascal yang akan di scan
		ln = 0;
		NN = 0;
		while (!feof(f)){
			N = 0;
			ch = 0;
			while ( ('\n' != (ch = getc(f))) && (!feof(f)) ) {
				instr[N] = ch;
				N++;
				}
		instr[N] = 0;
		N++;

	//  Pembuatan dan pembagian token dimulai
	ind = 0;
	while (ind < (N-1)){
		ch = instr[ind];
			// mengecek karakter spasi atau space
			while (ch == 32) {
			ind++;
			ch = instr[ind];
			}


	// jika dalam baris hanya ada spasi saja maka akan diabaikan
	if ( ind < N ) {
		//Pengecekan Operator yang ada
		j = 0;
		while( (j < t) && (ch != oprtr[j]) )
		j++;

	// mengecek apakah karakter tersebut termasuk operator
	if ( ch == oprtr[j] ) {
		zz = 0;
		tmp[0] = ch;
		if ( ind < N )
			nxt = instr[ind + 1];

		// Mengecek operator relasional atau operator pembanding seperti ini “<>, <=, >=, :=” .
		if ((nxt == '>') || (nxt == '=')) {
			zz++;
			tmp[1] = nxt;
			ind +=2;

			if ( nxt == '>' )
			Token[NN].typ = 9; // operator relasional
		}
		else if ( (ch == '.') && (nxt == '.')){
			zz++;
			tmp[1] = nxt;
			ind +=2;
			Token[NN].typ = 16; // range
		}
		else
		ind++;

			switch (ch){
				case ':': if (nxt == '=')
					Token[NN].typ = 6;
					else
					Token[NN].typ = 1;
				break;
				case '+':
				case '-':
				case '*':
				case '/': Token[NN].typ = 1; break; // operator
				case '.': if ( nxt != '.')
					Token[NN].typ = 12; break; // operator
				case ';': Token[NN].typ = 8; break; // end of statement
				case ',': Token[NN].typ = 12; break; // seperator
				case '=':
				case '>':
				case '<': Token[NN].typ = 9; break; // relational oparator
				case '[': Token[NN].typ = 10; break; // array start
				case ']': Token[NN].typ = 11; break; // array end
				case '(':
				case ')': Token[NN].typ = 7; break; // paranthesis
				case '$': Token[NN].typ = 14; break; // compiler directive
				case '#': Token[NN].typ = 15; break; // ASCII converter
				case '@': Token[NN].typ = 13; break; // unary operator
			}
		zz++;
		tmp[zz] = 0;
		Token[NN].no = NN;
		strcpy(Token[NN].ctkn, tmp);
		Token[NN].lline = ln;
		NN++;
	}
	// 39 = ” ‘ ” : mulai string
	else if (ch == 39) {
		zz = 0;
		ind++;
		nxt = instr[ind];
		tmp[0] = nxt;

		// akhir dari string
		while ( nxt != 39) {
			zz++;
			ind++;
			tmp[zz] = nxt = instr[ind];
		}

		tmp[zz] = 0;
		Token[NN].no = NN;
		strcpy(Token[NN].ctkn, tmp);
		Token[NN].typ = 4;
		Token[NN].lline = ln;
		NN++;
		ind++;
	}

	// Jika karakternya adalah A..Z atau a..z atau ‘_’ masuk kedalam identifier atau reserved word
	else if ( ((ch >= 65) && (ch <= 90)) || ((ch >= 97) &&
	(ch <= 122)) || (ch == 95) ){
			zz = 0;
			tmp[0] = ch;
			nxt = instr[ind + 1];
			zz++;

			// A..Z , a..z , ‘_’, 0..9
			while ( ((nxt >= 65) && (nxt <= 90)) || ((nxt >= 97) &&
			(nxt <= 122)) || (nxt == 95) || ((nxt >= 48) && (nxt <= 57)) ){
					ind++;
					ch = instr[ind];
					tmp[zz] = ch;
					zz++;
					nxt = instr[ind + 1];
			}
			tmp[zz] = 0;

			if ( isReserved(tmp) == 0)
				Token[NN].typ = 0; // keyword = reserved
			else
				Token[NN].typ = 2; // identifier
				Token[NN].no = NN;
				strcpy(Token[NN].ctkn, tmp);
				Token[NN].lline = ln;
				NN++;
				ind++;
	}

	 // 0..9 : ‘number’
	else if ( (ch >= 48) && (ch <= 57) ) {
			zz = 0;
			tmp[0] = ch;
			nxt = instr[ind + 1];
			zz++;

			// Mengambil semua 0..9 sebagai numbers.
			while ( ((nxt >= 48) && (nxt <= 57)) ) {
				ind++;
				ch = instr[ind];
				tmp[zz] = ch;
				zz++;
				nxt = instr[ind + 1];
			}

			tmp[zz] = 0;
			Token[NN].typ = 3; // constant
			Token[NN].no = NN;
			strcpy(Token[NN].ctkn, tmp);
			Token[NN].lline = ln;
			NN++;
			ind++;
		}


	//Cek Jika illegal charater
	else {
		Token[NN].no = NN;
		Token[NN].ctkn[0] = ch;
		Token[NN].ctkn[1] = 0;
		Token[NN].typ = 5;
		Token[NN].lline = ln;
		NN++;
		ind++;
	} // if
	} // if
	} // while
	ln++; // Menambah nomor baris
	} // while


	fclose(f);

	//Mencetak hasil scan
	printf( "\n  NO       LINE              TOKEN                              TIP\n");
	printf( "__________________________________________________________________________\n");
	for(i=0;i<NN;i++){
		printf(" %4ld   %4ld    %30s       %20s\n",
		Token[i].no, Token[i].lline, Token[i].ctkn, Typs[Token[i].typ].nm);
	}
	//Mencetak hasil scan ke notepad
    fopen("Token.txt", "w");
	fprintf(f, "\n  NO       LINE              TOKEN                              TIP\n");
	fprintf(f, "__________________________________________________________________________\n");
	for(i=0;i<NN;i++)
	{
	fprintf(f, " %4ld   %4ld    %30s       %20s\n",
	Token[i].no, Token[i].lline, Token[i].ctkn, Typs[Token[i].typ].nm);
	}
	getch();

  return 0;
}

