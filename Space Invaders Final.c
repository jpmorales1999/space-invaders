#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESPACIO 32

FILE *Archivo;

//Direccion de donde se va a crear el archivo
char direccion[] = "D:\\Juan\\jugadores.txt";

//Struct para llenar Archivo
struct{
    char nombre[30];
    int score;
}jugadores;

//Variables
int puntos=0;
//Vectores de 80 posiciones para crear tanto meteoros como disparos en cordenas de x
int meteoroX2[80], meteoroY2[80];
int disparoX[80], disparoY[80];
int posNaveX[81], posNaveY[81];
int salud=3;
int vidas=3;

//Prototipo de las funciones
void ver();
void abrirArchivo();
int buscarCentro();
void perderVida();
void verNave();
void imagenNave();
void presentacionJuego();
void puntuacion();
void colisionmeteoroydisparo();
void disparo(int,int);
void gotoxy(int x, int y);
void cuadro();
void ocultar_cursor();
void borrarNave();
int mover();
void vida();
void choque();
void llenarVectores();
void meteorosydisparosver();
void registrarmeteorosydisparos(int);
void moverNaveIzquierda();
void moverNaveDerecha();
void moverNaveArriba(int);
void moverNaveAbajo(int);
int posicionYPuntaNave();

//Funcion principal
int main(){
    int disparo;
    imagenNave();
    system("pause");
    system("cls");
    gotoxy(1,10);
    presentacionJuego();
    gotoxy(1,28); system("pause");
    system("cls");
    system("color F");
    llenarVectores();
	ocultar_cursor();
	cuadro();
    vida();

	bool game_over = false;
	 while(!game_over){

        disparo = mover();
        verNave();
        registrarmeteorosydisparos(disparo);
        meteorosydisparosver();
        choque();
        colisionmeteoroydisparo();
        if(puntos >= 0 && puntos <100){
        Sleep(80);
        }else if(puntos >= 100 && puntos < 200){
        Sleep(60);
        }else{
            Sleep(30);
        }
        if(vidas == 0){
            system("cls");
            printf("Perdiste el juego\n");
            system("pause");
            if(puntos > 0){
                    abrirArchivo();
                    ver();
            }
            game_over = true;
        }
    }
	return 0;
}

//Cordenadas de la pantalla
void gotoxy(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}

//Margen cuadro Space Invaders
void cuadro(){
    for (int i=5; i<80; i++){
        gotoxy(i,3); printf("%c",205);
        gotoxy(i,32); printf("%c",205);
    }
    for(int i=4; i<32; i++){
        gotoxy(4,i); printf("%c",186);
        gotoxy(80,i); printf("%c",186);
    }
    gotoxy(4,3); printf("%c",201);
    gotoxy(4,32); printf("%c",200);
    gotoxy(80,3); printf("%c",187);
    gotoxy(80,32); printf("%c",188);
}

//Ocultar Cursor
void ocultar_cursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &cci);
}

//Borrar
void borrarNave(){
    for(int i=5; i<80; i++){
        if(posNaveX[i]>=1){
            for(int j=posNaveY[i];j<posNaveY[i]+posNaveX[i];j++){
                gotoxy(i,j); printf(" ");
        }
    }
}
}

//Mover nave dependiendo tecla oprimida
int mover(){
    int disparo = 0;
    if(kbhit()){
            char tecla = getch();
            if(tecla == IZQUIERDA) moverNaveIzquierda();
            if(tecla == DERECHA) moverNaveDerecha();
            if(tecla == ARRIBA) moverNaveArriba(posicionYPuntaNave());
            if(tecla == ABAJO) moverNaveAbajo(posicionYPuntaNave());
            if(tecla == ESPACIO){
                  disparo = 1;
            }
            verNave();
        }
        return disparo;
}

//Vidas
void vida(){
    gotoxy(46,2); printf("Vidas:  %d", vidas);
    gotoxy(62,2); printf("Salud: ");
    gotoxy(70,2); printf("        ");
    for(int i=0; i<salud; i++){
        gotoxy(70+i,2); printf("%c",241);
    }
}

//Movimiento meteoros y disparos
void meteorosydisparosver(){
    for(int i=5; i<80; i++){
     if(meteoroX2[i]==1){
        gotoxy(i,meteoroY2[i]); printf(" ");
        meteoroY2[i]++;
        gotoxy(i,meteoroY2[i]); printf("%c", 248);
     }
     if(disparoX[i]==1){
        gotoxy(i,disparoY[i]); printf(" ");
        disparoY[i]--;
        gotoxy(i,disparoY[i]); printf("+");
     }
    if(meteoroY2[i]>=31){
        gotoxy(i,meteoroY2[i]); printf(" ");
        meteoroX2[i]=0;
    }
    if(disparoY[i]<=4){
        gotoxy(i,disparoY[i]); printf(" ");
        disparoX[i]=0;
    }
  }
}

//Choque meteoro-nave resta salud si se presenta impacto
void choque(){
    for(int i=5; i<80; i++){
        if(posNaveX[i] > 0 && meteoroX2[i]==1){
            if(meteoroY2[i]>=posNaveY[i]){
                 salud--;
                 vida();
                 perderVida();
                 meteoroX2[i] = 0;
            }
          }
       }
    }

//Inicializamos vectores en 0
void llenarVectores(){
    //int nx = 40;
    for (int i=5; i<80; i++){
        meteoroX2[i]=0;
        meteoroY2[i]=0;
        disparoX[i]=0;
        disparoY[i]=0;
        posNaveX[i]=0;
    }
    posNaveX[80]=0;
    int alto = 0;
    int subir = 1;
    for(int i=37; i<=43; i++){
      if(subir==1){
         alto++;
      }else{
         alto--;
      }
      posNaveX[i]=alto;
      if(alto==4){
        subir = 0;
      }
    }
        posNaveY[37]=28;
        posNaveY[43]=28;
        posNaveY[38]=27;
        posNaveY[42]=27;
        posNaveY[39]=26;
        posNaveY[41]=26;
        posNaveY[40]=25;
}

//Creacion nuevo meteoro - nuevo disparo
void registrarmeteorosydisparos(int disp){
    //Rand para meteoro x
    int mx;
    if(disparoX[buscarCentro()]==0 && disp==1){
        disparoX[buscarCentro()] = 1;
        disparoY[buscarCentro()] = posicionYPuntaNave()-1;
    }
    mx = 6 + rand()%((79+1) - 6);
    if(meteoroX2[mx]==0){
        meteoroX2[mx] = 1;
        meteoroY2[mx] = 5;
    }
}

//Choque meteoro - disparo
void colisionmeteoroydisparo(){
    for(int i=5; i<80; i++){
    if(disparoX[i]==1 && meteoroX2[i]==1){
            if(meteoroY2[i] >= disparoY[i]){
                gotoxy(i, disparoY[i]); printf("X");
                Sleep(30);
                gotoxy(i, disparoY[i]); printf(" ");
                puntuacion();
                disparoX[i]=0;
                meteoroX2[i]=0;
            }
       }
   }
}

//Puntuacion total alcanzada por el jugador
void puntuacion(){
    puntos += 10;
    gotoxy(10,2); printf("Puntuacion:  %d", puntos);
}

//Ascii Space Invaders
void presentacionJuego(){
    system("color 3");
printf("  #####\n");
printf(" #     # #####    ##    ####  ######\n");
printf(" #       #    #  #  #  #    # #\n");
printf("  #####  #    # #    # #      #####\n");
printf("       # #####  ###### #      #\n");
printf(" #     # #      #    # #    # #\n");
printf("  #####  #      #    #  ####  ######\n");

printf(" ###\n");
printf("  #  #    # #    #   ##   #####  ###### #####   ####\n");
printf("  #  ##   # #    #  #  #  #    # #      #    # #\n");
printf("  #  # #  # #    # #    # #    # #####  #    #  ####\n");
printf("  #  #  # # #    # ###### #    # #      #####       #\n");
printf("  #  #   ##  #  #  #    # #    # #      #   #  #    #\n");
printf(" ### #    #   ##   #    # #####  ###### #    #  ####\n");
}

//Ascii imagen nave
void imagenNave(){
    system("color 8");
printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW@##@WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW@#=--=#@WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW@#=......=#@WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW@#=-........-=#WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW##:............:##WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW#=................=#WWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWW#*..................*#@WWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWW@#*....................+#@WWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWW#*......................*#WWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWW#=........-*####*-........=#WWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWW@#-.......*#*----*#*.......-#@WWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWW#*.......-#=------=#-.......*#WWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW@#-........*#:----:#*........-#@WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW#=..........:######:..........=#WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW#*............................*#WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW#+............................+#WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW#+............................+#WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW#*............................*#WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW#=............................=#WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWW@#-..........................-#@WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWW#*..........................*#WWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWW@#-........................-##WWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWW#=...........**...........=#WWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWW###+..........==..........*###@WWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWW@##:-*#+.........==.........+#*-:##@WWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWW@##:----*#+........==........+#*----:##@WWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWW##:-------+#*.......==.......*#+-------+##WWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWW#=----------:##-.....==.....-##:----------=#WWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWW#*---------:*##################*:---------*#WWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWW@#+----:*###WWWWWWWWWWWWWWWWWWWW@##*:----+##WWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWW@#=*###WWWWWWWWWWWWWWWWWWWWWWWWWWWW@##*=#@WWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW@#=++++++=#@WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW@#-........:##WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW#*..........*#WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW##.........-##WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW##:......+##WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW##:..+##WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW####WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");

printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n");
}

//Pintar nave en pantalla segun movimientos
void verNave(){
    for(int i=5; i<80; i++){
        if(posNaveX[i]>=1){
            for(int j=posNaveY[i];j<posNaveY[i]+posNaveX[i];j++){
                gotoxy(i,j); printf("%c", 219);
        }
    }
}
}

//Mover izquierda nave
void moverNaveIzquierda(){
    if(posNaveX[6]==0){
            borrarNave();
    for(int i=5; i<80; i++){
        posNaveX[i]=posNaveX[i+1];
        posNaveY[i]=posNaveY[i+1];
    }
  }
}

//Mover derecha nave
void moverNaveDerecha(){
    if(posNaveX[79]==0){
            borrarNave();
    for(int i=79; i>5; i--){
        posNaveX[i]=posNaveX[i-1];
        posNaveY[i]=posNaveY[i-1];
    }
  }
}

//Mueve arriba la nave
void moverNaveArriba(int pos){
    if(pos>7){
            borrarNave();
        for(int i=5; i<80; i++){
                if(posNaveY[i]>0){
            posNaveY[i]--;
            }
        }
    }
}

//Mueve abajo la nave
void moverNaveAbajo(int pos){
    if(pos+4<35){
            borrarNave();
        for(int i=5; i<80; i++){
                if(posNaveY[i]>0){
            posNaveY[i]++;
                }
        }
    }
}

//Busca la punta de la nave
int posicionYPuntaNave(){
    int mayor;

    for(int i=5; i<80; i++){
        if(i == 5){
           mayor=posNaveY[i];
        }
        if(posNaveY[i]> mayor){
            mayor = posNaveY[i];
        }
    }
    return mayor;
}

//Busca el centro de la nave
int buscarCentro(){
    for(int i=5; i<80; i++){
        if(posNaveX[i]==4){
            return i;
        }
    }
    return 0;
}

//Funcion perder vidas
void perderVida(){
    if(salud==0){
        vidas--;
        salud=3;
        vida();
    }
}

//Creamos o abrimos archivo txt
void abrirArchivo(){
    Archivo = fopen(direccion,"a+");
    if(Archivo==NULL){
        printf("No se pudo abrir el archivo");
    }else{
        printf("Ingrese Nombre del jugador: "); gets(jugadores.nombre);
        fprintf(Archivo,"\n\t\tLISTA DE JUGADORES\n");
        fprintf(Archivo,"\nNombre Jugador: ");
        fprintf(Archivo,"%s\n",jugadores.nombre);
        fprintf(Archivo,"\nPuntuacion: ");
        fprintf(Archivo,"%d\n",puntos);
    }
    fclose(Archivo);
}

void ver(){
    //Visualizar Contactos del Archivo
    int c;
    Archivo = fopen(direccion,"r");
	if(Archivo == NULL){
		printf("Error al tratar de leer el Archivo");
		exit (1);
	}
	while((c=fgetc(Archivo))!=EOF){
		if(c=='\n'){
			printf("\n");
		}
		else{
			putchar(c);
		}
	}
}
