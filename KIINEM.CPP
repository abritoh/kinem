
/*------------------------------------------------------------------------
NOMBRE............:       Programa de Tiro Parabólico: Modo gráfico
VERSION...........:       1.0
FECHA.............:       06/Julio/2002
PROGRAMADO POR....:       ARBR
			  
------------------------------------------------------------------------*/

# include <alloc.h>       *
# include <conio.h>       *
# include <ctype.h>       *
# include <graphics.h>    *
# include <io.h>          *
# include <stdio.h>       *
# include <stdlib.h>      *
# include <string.h>      *
# include <time.h>        *


# include <c:\tc\arce\libgf.h> // copiar estas librerias a \\tc\include\
# include <c:\tc\arce\mate.h>  // e incluirlas como:  # include <mate.h>

# define MAXLONGBOTOM   7
# define MAXBOTON      10
# define SPACEBOTON_X 8*8
# define BOTON_X        5
# define BOTON_Y      450
# define T_PAUSA       15

# define CORX1          0
# define CORY1          0
# define T_LONG_X     640
# define T_LONG_Y     420
# define MAX_Y        480
# define C_MSGE_X     300
# define C_MSGE_Y     100
# define COLOR_TEXTO    1

# define VELOCIDAD_0  118 // v
# define ANGULO_0      98 // b
# define ALTURA_0     104 // h
# define TIEMPO_F     116 // t
# define TIME_PAUSA   112 // p
# define DATOS_F      100 // d
# define RUN          114 // r
# define CLEAR         99 // c
# define F1            59 // F1
# define EXIT          27 // Esc

# define GRAVEDAD_SI      9.81
# define GRAVEDAD_IN      32.2
# define Origen_X           20
# define Origen_Y  T_LONG_Y-20


void   ShowBoton();

void   copy_screen(int, int, int, int);
void   Presenta(int);
void   Fondo(int, int, int, int);
void   IniBoton(int ,int);
void   datos(double, double, double ,double);
char   pressboton(char);

double readNum(int,int,int,char *);
double Graficar(double, double, double, int, int, int, int);

int    help();
int    Imprimir(char *);

/* Declaraci¢n de Botones */
char Texboton[MAXBOTON][MAXLONGBOTOM]
     ={"  Vo  ","  áo  ","  Ho  "," t(s) ","  Dat ",
       "  Run ","pause ","  Cls ","  F1  "," Exit "};

int assciTec[MAXBOTON]
     ={ 118,98,104,116,100,114,112,99,59,27};

   S_GBOTON boton[MAXBOTON];
   /*-----------------------------------------------------------------*/
	      S_GWIN_CENT
   C_MENSAJES =
   {EXTERIOR,C_MSGE_X,C_MSGE_Y,6,GRIS,ROJO,BLANCO,"Entrada de datos"};
   /*-----------------------------------------------------------------*/

int maxx, maxy;

void  *buf; /* Resevamos un area para salvar pantalla*/
unsigned size;


/*-------------------------------------------------------------------------*/
void main()
{
   int   i, t_pausa = 20;
   int   X1 = CORX1, Y1 = CORY1, X2 = CORX1 + T_LONG_X, Y2 = CORY1 + T_LONG_Y;
   int   TECLA,nLeft, nTop, nRight, nBottom,COLOR_G = NEGRO;

   double Vo = 50.0, Bo = 45.0, Ho = 0.0, Tf=3.0;

   time_t wait, time_Ini;

   /* INICIO DE VENTANAS (I) E INICIO DE BOTONES (II) */
   /*-----------------------------------------------------------------*/
	       S_GWIN_XY
   A_GRAFICA   =
   {INTERIOR,X1,Y1,X2,Y2,0,GRIS,AZUL,ROJO_C,"An lisis del Movimiento Parab¢lico"},

   B_TAREAS    =
   {INTERIOR,X1,Y2,X2,MAX_Y,1,GRIS,AZUL,AMARILLO,"Opciones"};
   /*-----------------------------------------------------------------*/
   IniBoton(BOTON_X,BOTON_Y);
   /*-----------------------------------------------------------------*/

   /* Inicia el modo grafico */
      IniGf();

   /* Presenta Pantalla de Inicio */
	Fondo(AZUL,BLANCO,ROJO_C,3); Presenta(BLANCO);

	time_Ini = wait = time(NULL);
	while(!kbhit())
	{
	 if( (difftime(wait,time_Ini)) >= T_PAUSA) break; wait = time(NULL);
	}

   /* Presenta area de trabajo */
      Fondo(BLANCO,BLANCO,AZUL_C,9);
      GWinXY(A_GRAFICA); GWinXY(B_TAREAS);
      ShowBoton();

   do{
       TECLA = getche();

    /* PULSA EL BOTON SI ES VµLIDO */
       pressboton(TECLA);

      switch(TECLA)
      {
	case VELOCIDAD_0:
	     copy_screen(170,190,470,290);  GWinCent(C_MENSAJES);
	     Vo = readNum(180,240,ROJO,"Introduce un valor para Vo: ");
	     putimage(170,190,buf,COPY_PUT);  free(buf);
	break;

	case ANGULO_0:
	     copy_screen(170,190,470,290);   GWinCent(C_MENSAJES);
	     Bo = readNum(180,240,ROJO,"Introduce un valor para áo:");
	     putimage(170,190,buf,COPY_PUT);  free(buf);
	break;

	case ALTURA_0:
	     copy_screen(170,190,470,290);   GWinCent(C_MENSAJES);
	     Ho = readNum(180,240,ROJO,"Introduce un valor para Ho: ");
	     putimage(170,190,buf,COPY_PUT); free(buf);
	break;

	case TIEMPO_F:
	     copy_screen(170,190,470,290); GWinCent(C_MENSAJES);
	     Tf = readNum(180,240,ROJO,"Introduce un valor para Tf: ");
	     putimage(170,190,buf,COPY_PUT);  free(buf);
	break;

	case TIME_PAUSA:
	     copy_screen(170,190,470,290); GWinCent(C_MENSAJES);
	     t_pausa = readNum(180,240,ROJO,"Digite valor de pausa ms: ");
	     putimage(170,190,buf,COPY_PUT);  free(buf);
	break;

	case DATOS_F:
	     datos(Vo,Bo,Ho,Tf); GWinXY(A_GRAFICA);
	break;

	case RUN:
	      COLOR_G ++; if(COLOR_G==BLANCO) COLOR_G = AZUL;
	      Graficar(Vo,Bo,Ho,t_pausa,Origen_X,Origen_Y,COLOR_G);
	break;

	case F1:
	     restorecrtmode();  help(); IniGf();
	     Fondo(BLANCO,BLANCO,AZUL_C,9); GWinXY(A_GRAFICA);
	     GWinXY(B_TAREAS); ShowBoton();
	break;

	case CLEAR:
	     GWinXY(A_GRAFICA); COLOR_G = AZUL;
	break;

	case EXIT:
	break;

	default:
	    mysound(sCUATRO,DEF_LY);
	break;
      }

   }while(TECLA!=SCAPE);

   closegraph(); // limpia el  rea y finaliza el modo gr fico
}


/*-------------------------------------------------------------------------*/

void copy_screen(int left, int top, int right, int bottom)
{
   size = imagesize(left, top, right, bottom);
   if ((buf = malloc(size)) == NULL)
   {
      closegraph(); printf("Error: Memoria insuficiente     \n"
			   "Presione una Tecla para terminar \n");
      getch(); exit(1);
   }
   getimage(left, top, right, bottom,buf);
}

# define COORMAX_X  360
# define COORMAX_Y  360
# define MAX_PUNTOS 800

double Graficar(double V, double B, double H, int time,int Xo,int Yo,int GRAFICA_COLOR)
{
    int    MAX_X = getmaxx() - 19, Y2=40, cord_max_y, CONTINUE,
	   nn,n,pp = nCERO, p, y_cord, x_cord, corxy[MAX_PUNTOS];

    float  Escala;

    double  g= GRAVEDAD_SI, Vox, Voy, SEN_B, COS_B, t, SEN_2B,TIEMPO_MAS,
	    Vx[MAX_PUNTOS], Vy[MAX_PUNTOS], Time[MAX_PUNTOS],Sx[MAX_PUNTOS],
	    Sy[MAX_PUNTOS], ALCANCE_X ,TIEMPO_DE_VUELO,ALTURA_MAXIMA;

    char   *cx[80],*cy[80],Tex[80];


    /* SE DIBUJA EL SISTEMA COORDENADO ----------------------*/
       setcolor(ROJO);
       setlinestyle(SOLID_LINE, 1, 20);

       line(Xo, Yo, MAX_X, Yo);     // Eje X
       line(Xo, Y2, Xo, Yo);        // Eje Y

       outtextxy(MAX_X+10, Yo-4, "x"); // T¡tulo x
       outtextxy(Xo-3    , 30  , "y"); // T¡tulo y
       outtextxy(Xo      , Yo+3, "o");
       outtextxy(MAX_X   , Yo-3, "");
       outtextxy(Xo-3    , 38  , "");

       settextstyle(0,1,0);
       outtextxy(Xo-3  , 45   , "360");
       settextstyle(0,0,0);
       outtextxy(Xo+580, Yo+10, "580");

    /* ------------------------------------------------------ */

    /* CALCULOS COMPONENTES */

	SEN_B  = operacion(B    , uSEN ,GRAD);
	SEN_2B = operacion(2.0*B, uSEN ,GRAD);
	COS_B  = operacion(B    , uCOS ,GRAD);
	Vox    = V * COS_B;
	Voy    = V * SEN_B;

   /* CALCULO DE CONDICIONES MµXIMAS */

      ALCANCE_X       = V*V*SEN_2B / (2.0*g)*(1+pow(1+2*g*H/(Voy*Voy),0.5) );
      TIEMPO_DE_VUELO = ALCANCE_X / Vox;
      ALTURA_MAXIMA   = H + Voy * Voy / ( 2.0 * g );

      if(ALCANCE_X > ALTURA_MAXIMA){ Escala = ceil(ALCANCE_X / 360.0); }
      else{ Escala = ceil(ALTURA_MAXIMA / 360.0); }

      if(ALCANCE_X <= 100 && ALTURA_MAXIMA <=100) Escala = 0.9;
      if(ALCANCE_X <= 50 && ALTURA_MAXIMA  <=50)  Escala = 0.8;
      if(ALCANCE_X <= 25 && ALTURA_MAXIMA  <=25)  Escala = 0.04;
      if(ALCANCE_X <= 10 && ALTURA_MAXIMA  <=10)  Escala = 0.02;

      TIEMPO_MAS =  ceill( (TIEMPO_DE_VUELO + 40) /80) / 10;
      if (TIEMPO_DE_VUELO <= 80 ) TIEMPO_MAS = 0.1;

   /* CALCULO DE s,v, PARA CADA VALOR DE t */
   for(t = 0.0, p = 0; t <= TIEMPO_DE_VUELO; t = t + TIEMPO_MAS, p++)
   {
       if(t + TIEMPO_MAS > TIEMPO_DE_VUELO) t = TIEMPO_DE_VUELO;
       Time[p] = t;
       Sx[p]   = Vox * t;
       Sy[p]   = H + Voy * t - g * t * t / 2.0;
       Vx[p]   = Vox;
       Vy[p]   = Voy - g * t;
   }

   setcolor(BLANCO);  outtextxy(30,30,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
   sprintf(Tex,"Escala 1:%.2f",Escala);setcolor(ROJO_C);
   outtextxy(30,30,Tex);

   for(nn=0,n = 150; nn<=2; nn++, n = n + 20)
   { setcolor(BLANCO);  outtextxy(490,n,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ"); }

   setcolor(ROJO);
   sprintf(Tex,"Vo [m/s] = %.3lf ",V);   outtextxy(490,150,Tex);
   sprintf(Tex,"áo [ ø ] = %.3lf ",B);   outtextxy(490,170,Tex);
   sprintf(Tex,"Ho [ m ] = %.3lf ",H);   outtextxy(490,190,Tex);

   /* GRAFICACION DEL TIRO PARABOLICO */

   cord_max_y = (-1) * ALTURA_MAXIMA / Escala + Origen_Y; CONTINUE = TRUE;
   while(pp < p && !kbhit())
   {
       x_cord = ( 1) * Sx[pp] / Escala + Origen_X;
       y_cord = (-1) * Sy[pp] / Escala + Origen_Y;
       putpixel(x_cord, y_cord ,GRAFICA_COLOR); delay(time*TIEMPO_MAS*10);

	 for(nn=0,n = 50; nn<=4; nn++, n = n + 20)
	 { setcolor(BLANCO);  outtextxy(490,n,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ"); }

	 setcolor(AZUL_C);
	 sprintf(Tex,"t  [ s ] = %.3lf ",Time[pp]); outtextxy(490,50,Tex);
	 sprintf(Tex,"x  [ m ] = %.3lf ",Sx[pp]);   outtextxy(490,70,Tex);
	 sprintf(Tex,"y  [ m ] = %.3lf ",Sy[pp]);   outtextxy(490,90,Tex);
	 sprintf(Tex,"vx [m/s] = %.3lf ",Vx[pp]);   outtextxy(490,110,Tex);
	 sprintf(Tex,"vy [m/s] = %.3lf ",Vy[pp]);   outtextxy(490,130,Tex);

	 if(y_cord == cord_max_y && pp > nCERO && CONTINUE!=FALSE)
	 {
	   setcolor(GRAFICA_COLOR); outtextxy(x_cord-2, y_cord-5,"+");
	   CONTINUE = FALSE;
	 }

      pp++;
   }
   setcolor(GRAFICA_COLOR); outtextxy(x_cord-5, y_cord-5,"*");
   mysound(MI,30);

 return (TRUE);
}

# define Ini_x       150
# define Ini_y       140
# define mas_x       300
# define mas_y        50

# define T_MAS        43
# define T_MENOS      45
# define T_INTERVALO 105
# define MIN_TIME      0

void  datos(double V, double B, double H, double t)
{

   char TEC,Tex[80],msg_F[6][80];
   char *TITULOS_X[]={"Al inicio","Despues de t","En el Impacto"};
   char *TITULOS_Y[]={"Posici¢n","Velocidad","En H es Max"};
   char *TITULOS_dY[]={"Sx","Sy","S","Vx","Vy","áø","V","t",""};

   double g = GRAVEDAD_SI,T[3], Vox, Voy, SEN_2B, SEN_B,COS_B,TAN_B,
	  Sx[3],Sy[3],Sf[3],Vx[3],Vy[3],Vf[3],B_[3],TanB_[3],
	  I_time = 1.0, ALCANCE_X ,TIEMPO_DE_VUELO,ALTURA_MAXIMA,
	  TIEMPO_DE_H_MAX,ALCANCE_EN_H_MAX;

   int x,y,i,j;

   S_GWIN_CENT  C_DATOS =
   { EXTERIOR,600,350,1,AZUL_C,ROJO_C,BLANCO," An lisis de datos " };

   do{
       GWinCent(C_DATOS); /* CREA LA VENTANA */

       /* DIBUJA LA TABLA */

       x = Ini_x, y = Ini_y;
       for(i=30,j=1; i<=30*8; i=i+25,j++)
       {
	 if(j==0 || j==3 || j==7 || j==9 || j==12)
	 { setcolor(ROJO_C); setlinestyle(SOLID_LINE, 10, 45); }
	rectangle(x, y, x + 450, y + i ); setcolor(BLANCO);
       }

       /* DIBUJA LAS DIVISIONES EN COLUMNAS */
       for(i=Ini_x + 50; i<=600; i = i + 140)
       {
	 line(i, Ini_y, i,Ini_y+230 );
       }
       setcolor(AMARILLO);

       /* COLOCA LOS TÖTULOS DE COLUMNA PRINCIPAL */
       for(i=0,j = Ini_x+50; i<=2; i++,j = j + 140)
       {
	  outtextxy(j,Ini_y-35,TITULOS_X[i]);
       }

       /* COLOCA LOS TÖTULOS DE LAS FILAS */
       for(i=0,j = Ini_y+20; i<=2; i++,j = j + 80)
       {
	 outtextxy(Ini_x-100,j,TITULOS_Y[i]);
       }

       /* COLOCA LOS TÖTULOS EN PRIMERA COLUMNA */
       for(i=0,j = Ini_y+15; i<=8; i++,j = j + 25)
       {
	 outtextxy(Ini_x+20,j,TITULOS_dY[i]);
       }

	/* CALCULOS COMPONENTES */
	SEN_B  = operacion(B , uSEN ,GRAD);
	SEN_2B = operacion(2.0*B, uSEN ,GRAD);
	COS_B  = operacion(B , uCOS ,GRAD);

	Vox    = V * COS_B;
	Voy = V * SEN_B;

	/* CALCULO DE CONDICIONES MµXIMAS */
	ALCANCE_X       = V*V*SEN_2B / (2.0*g)*(1+pow(1+2*g*H/(Voy*Voy),0.5) );
	TIEMPO_DE_VUELO = ALCANCE_X / Vox;
	ALTURA_MAXIMA   = H + Voy * Voy / ( 2.0 * g );

	T[0]   = 0.0; T[1] = t, T[2] = TIEMPO_DE_VUELO;

	TIEMPO_DE_H_MAX = Voy / g;
	ALCANCE_EN_H_MAX= Vox * TIEMPO_DE_H_MAX;

	sprintf(msg_F[0],"%.4lf",TIEMPO_DE_H_MAX);
	sprintf(msg_F[1],"%s","Altura maxima");
	sprintf(msg_F[2],"%.4lf",ALTURA_MAXIMA);
	sprintf(msg_F[3],"%s"," ");
	sprintf(msg_F[4],"%s","x en H max");
	sprintf(msg_F[5],"%.4lf",ALCANCE_EN_H_MAX);

      /* CALCULO DE RESULTADOS */
      for(i=0; i<=2; i++)
      {
	 Sx[i] = Vox * T[i];
	 Sy[i] = H + Voy * T[i] - g * T[i] * T[i] / 2.0;
	 Sf[i] = pow( ( Sx[i] * Sx[i] + Sy[i] * Sy[i] ),0.50);
	 Vx[i] = Vox;
	 Vy[i] = Voy - g * T[i];

	 TanB_[i] = Vy[i] / Vx[i];
	 B_[i] = operacion(TanB_[i] , uArTAN ,GRAD);

	 Vf[i] = pow( ( Vx[i] * Vx[i] + Vy[i] * Vy[i] ),0.50);
      }

      /* SALIDA DE RESULTADOS */
      for(i=0,j = Ini_x+60; i<=2; i++, j = j + 140)
      {
	 if(i==1) setcolor(BLANCO); else setcolor(VERDE_C);
	 sprintf(Tex,"t = %.4lf [s]",T[i]); outtextxy(j-10,Ini_y-15,Tex);

	 sprintf(Tex,"%.4lf",Sx[i]);    outtextxy(j,Ini_y+10,Tex);
	 sprintf(Tex,"%.4lf",Sy[i]);    outtextxy(j,Ini_y+40,Tex);
	 sprintf(Tex,"%.4lf",Sf[i]);    outtextxy(j,Ini_y+65,Tex);

	 sprintf(Tex,"%.4lf",Vx[i]);    outtextxy(j,Ini_y+90,Tex);
	 sprintf(Tex,"%.4lf",Vy[i]);    outtextxy(j,Ini_y+115,Tex);
	 sprintf(Tex,"%.3lfø",B_[i]);   outtextxy(j,Ini_y+140,Tex);
	 sprintf(Tex,"%.4lf",Vf[i]);    outtextxy(j,Ini_y+165,Tex);

	 outtextxy(j,Ini_y+190,msg_F[i]);
	 outtextxy(j,Ini_y+215,msg_F[i+3]);
    }

  TEC = getch();
  switch(TEC)
  {
     case T_MAS:
	  t = t + I_time;
	  if(t >= TIEMPO_DE_VUELO) t = MIN_TIME;
     break;
     case T_MENOS:
	  t = t - I_time;
	  if(t<MIN_TIME) t = TIEMPO_DE_VUELO;
     break;
     case T_INTERVALO:
	   GWinCent(C_MENSAJES);
	   I_time = readNum(180,240,ROJO,"Digita valor del Intervalo: ");
     break;

  }
  }while(TEC != SCAPE);
}


# define MAXDIGIT 5

double readNum(int x,int y,int COLOR_NUM,char TEXTO[])
{
  char *fincadena,cDIG[MAXDIGIT+1], sTECLA[1];
  int i,j,AUXTEC,TECLA,PUNTO=FALSE;
  double valor;

  i = nCERO, TECLA = cVACIO;
  setcolor(COLOR_TEXTO); outtextxy(x,y,TEXTO);

  x = x + strlen(TEXTO) * 8;
  do{

     do{
	setcolor(BLANCO); outtextxy(x+i*8,y,"ÛÛ");
	setcolor(COLOR_NUM);	TECLA  = getch();
	AUXTEC = TECLA;  sprintf(sTECLA,"%c",AUXTEC);
	outtextxy(x+i*8,y,sTECLA);

	if(TECLA == cNEGATIVO && i==nCERO ) break;
	if(TECLA == cPUNTO && PUNTO==FALSE ){ PUNTO =TRUE ; break;}
	if(TECLA == BORRA_C && i>=nUNO)
	  { i--; if(cDIG[i-1] == cPUNTO); PUNTO=FALSE;}

     }while(!isdigit(TECLA) && TECLA!= uENTER);

     cDIG[i]=TECLA;
     if(i<=MAXDIGIT) i++;
  }while( TECLA!=uENTER);

  valor = strtod(cDIG, &fincadena);
  setcolor(BLANCO);

 return(valor);
}

void ShowBoton()
{
   int i;
   for(i=0; i<=(MAXBOTON-1);i++)
   {
	if(strlen(Texboton[i])==0) break; GBoton(boton[i]);
   }
}


void IniBoton(int x,int y)
{
   int i, colboton = GRIS, colTexboton = ROJO;

   for(i=0; i<=(MAXBOTON-1);i++)
   {
	if(strlen(Texboton[i])==0) break;
	boton[i].nWide      = 20;
	boton[i].nModo      = EXTERIOR;
	boton[i].nColorBack = colboton;
	boton[i].nColorText = colTexboton;
	boton[i].cText      = Texboton[i];
	boton[i].nX1 = x;
	boton[i].nY1 = y;

	x = x + SPACEBOTON_X;
   }
}


# define TIME         100
char pressboton(char TEC)
{
  int i=0;

  do{
    if(TEC==assciTec[i])
    {
      boton[i].nModo= INTERIOR; GBoton(boton[i]); delay(TIME);
      boton[i].nModo= EXTERIOR; GBoton(boton[i]);
      return(TRUE);
    }
    i++;
  }while(i<=MAXBOTON);

 return(FALSE);
}


void Fondo(int colorFondo,int colorTexto,int colorRelleno, int Relleno)
{
   setbkcolor(colorFondo); setcolor(colorTexto);
   setfillstyle(Relleno,colorRelleno);
   bar(0,0,getmaxx(),getmaxy());
}

void Presenta(int nColorF)
{

  setbkcolor(nColorF);   bar(0,0,getmaxx(),getmaxy());
  settextstyle(7,0,3);   setcolor(ROJO);
  outtextxy(300,20,"     K I N E M A T I C ");
  outtextxy(300,70,"       C A L C U L O S ");
  outtextxy(300,120,"C I N E M A T I C O S ");

  setcolor(AZUL);
  settextstyle(0,0,10);  outtextxy(10,20, "~~~~");
  settextstyle(0,0,10);	 outtextxy(10,70," ÛÛ ");

  setcolor(BLANCO);
  settextstyle(1,0,6);   outtextxy(95 ,72,"UAEM");
  setcolor(VERDE);
  settextstyle(1,0,2);   outtextxy(145,125,"FCQeI");

  setcolor(AZUL_C);
  settextstyle(0,0,2);
  outtextxy(100,320,"   Brito Hern ndez Arcenio  ");
  outtextxy(100,350,"Ing. Victor Dominguez Flores");
}


# define F5                63
# define MAXLINE           23
# define TEC_AV            81
# define TEC_HOME          71
# define MAXBYTE         5237
# define MSGERF  "No es posible abrir el archivo.\n"

int help()
{
  FILE *p;

  long byte=nCERO,lin=nCERO;
  char msg[2517],ch,TEC,msg2[100];
  char NAME_ARCHIVO[] = "kinem.hlp";

   if ((p = fopen(NAME_ARCHIVO,"rt")) == NULL)
   { fprintf(stderr,MSGERF); getch(); return(0); }

   textcolor(BLANCO);  textbackground(AZUL); clrscr();
   do{
      fseek(p, byte, SEEK_SET); lin = 0;
      do
      {
	 ch = fgetc(p);  printf("%c",ch);
	 if(ch=='\n') lin++;  byte = ftell(p);
      }while (lin<=MAXLINE && byte<=MAXBYTE-1);

      do{
	 TEC = getch();
	 if(TEC==F5){ Imprimir(NAME_ARCHIVO); TEC = TEC_HOME; }
	 if(TEC==TEC_HOME)  { byte = nCERO; clrscr(); }
      }while(TEC!=TEC_AV && TEC!=TEC_HOME && TEC!=SCAPE);

      if(byte>=MAXBYTE || byte < nCERO){ byte = nCERO; clrscr(); }

   }while(TEC != SCAPE);

   fclose(p);
   return(1);
}

# define MSGER_IMP  "Error, Imposible Imprimir\n       "
# define MSGER_IMP2 "Error al accesar a la Impresora\n "
# define BEGIN       740
/*-----------------------------------*/
int Imprimir (char ArImp[])
{
   FILE *ip,*PRINT;
   char ch;
   int i = BEGIN;
   if ((PRINT = fopen("PRN", "wt")) == NULL)
   {  fprintf(stderr,MSGER_IMP2);getch(); return(-1); }

   if ((ip = fopen(ArImp,"rt")) == NULL)
   { fprintf(stderr,MSGER_IMP); getch();  return(-1); }

   fseek(ip, BEGIN, SEEK_SET);
   do
   {
      ch = fgetc(ip);   fputc(ch,PRINT);
      i++; fseek(ip, i, SEEK_SET);
   }while (i<=MAXBYTE);

  fclose(PRINT);
  fclose(ip);

return(1);
}
/*-----------------------------------*/

