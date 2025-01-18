
/*
##############################################################################
NOMBRE............:       Programa de Tiro Parabólico: Modo gráfico
VERSION...........:       1.0 - (Turbo C)
FECHA.............:       06/Julio/2002
PROGRAMADO POR....:       ArBR | arcebrito@gmail.com
##############################################################################
*/

#include <alloc.h>
#include <conio.h>
#include <ctype.h>
#include <graphics.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <c:\tc\arce\libgf.h>
#include <c:\tc\arce\mate.h>

#define MAX_LONG_BUTTON 7
#define MAX_BUTTON 10
#define SPACE_BUTTON_X 8 * 8
#define BUTTON_X 5
#define BUTTON_Y 450
#define T_PAUSE 15

#define COORD_X1 0
#define COORD_Y1 0
#define T_LONG_X 640
#define T_LONG_Y 420
#define MAX_Y 480
#define C_MSG_X 300
#define C_MSG_Y 100
#define TEXT_FORECOLOR 1

#define INITIAL_SPEED 118  // v
#define INITIAL_ANGLE 98   // b
#define INITIAL_HEIGHT 104 // h
#define FINAL_TIME 116     // t
#define PAUSE_TIME 112     // p
#define FINAL_DATA 100     // d
#define RUN 114            // r
#define CLEAR 99           // c
#define F1 59              // F1
#define EXIT 27            // ESC

#define CONST_GRAVITY_SI 9.81
#define CONST_GRAVITY_US 32.2
#define ORIGIN_X 20
#define ORIGIN_Y T_LONG_Y - 20

void show_button();
int show_wnd_help ();
int print_file (char *);
char press_button (char);
void show_wnd_about (int);
void init_button (int, int);
void copy_screen (int, int, int, int);
void draw_background (int, int, int, int);
double read_input_value (int, int, int, char *);
void show_wnd_data_table (double, double, double, double);
double render_graphic (double, double, double, int, int, int, int);

char tex_button [MAX_BUTTON][MAX_LONG_BUTTON] 
   = {"  Vo  ", "  Bo  ", "  Ho  ", " t(s) ", "  Dat ", "  Run ", "Pause ", "  CLS ", "  F1  ", " Exit "};

int assci_key [MAX_BUTTON] = {118, 98, 104, 116, 100, 114, 112, 99, 59, 27};

S_GBOTON boton [MAX_BUTTON];

S_GWIN_CENT C_MENSAJES = {EXTERIOR, C_MSG_X, C_MSG_Y, 6, GRIS, ROJO, BLANCO, "Entrada de datos"};

void *buf;
int maxx, maxy;
unsigned size;

void main()
{
   int i, T_PAUSE = 20;
   int X1 = COORD_X1, Y1 = COORD_Y1, X2 = COORD_X1 + T_LONG_X, Y2 = COORD_Y1 + T_LONG_Y;
   int input_key, n_left, n_top, n_right, n_bottom, color_g = NEGRO;

   double Vo = 50.0, Bo = 45.0, Ho = 0.0, Tf = 3.0;

   time_t wait, time_ini;

   S_GWIN_XY A_GRAFICA = {INTERIOR, X1, Y1, X2, Y2, 0, GRIS, AZUL, ROJO_C, "Análisis del Movimiento Parabólico"},

   B_TAREAS = {INTERIOR, X1, Y2, X2, MAX_Y, 1, GRIS, AZUL, AMARILLO, "Opciones"};
   
   init_button(BUTTON_X, BUTTON_Y);

   initialize_graph_mode();

   draw_background(AZUL, BLANCO, ROJO_C, 3);
   show_wnd_about(BLANCO);

   time_ini = wait = time(NULL);
   while (!kbhit())
   {
      if ((difftime(wait, time_ini)) >= T_PAUSE)
         break;
      wait = time(NULL);
   }

   draw_background(BLANCO, BLANCO, AZUL_C, 9);
   GWinXY(A_GRAFICA);
   GWinXY(B_TAREAS);
   show_button();

   do
   {
      input_key = getche();
      
      press_button(input_key);

      switch (input_key)
      {
      case INITIAL_SPEED:
         copy_screen(170, 190, 470, 290);
         GWinCent(C_MENSAJES);
         Vo = read_input_value(180, 240, ROJO, "Introduce un valor para Vo: ");
         putimage(170, 190, buf, COPY_PUT);
         free(buf);
         break;

      case INITIAL_ANGLE:
         copy_screen(170, 190, 470, 290);
         GWinCent(C_MENSAJES);
         Bo = read_input_value(180, 240, ROJO, "Introduce un valor para Bo:");
         putimage(170, 190, buf, COPY_PUT);
         free(buf);
         break;

      case INITIAL_HEIGHT:
         copy_screen(170, 190, 470, 290);
         GWinCent(C_MENSAJES);
         Ho = read_input_value(180, 240, ROJO, "Introduce un valor para Ho: ");
         putimage(170, 190, buf, COPY_PUT);
         free(buf);
         break;

      case FINAL_TIME:
         copy_screen(170, 190, 470, 290);
         GWinCent(C_MENSAJES);
         Tf = read_input_value(180, 240, ROJO, "Introduce un valor para Tf: ");
         putimage(170, 190, buf, COPY_PUT);
         free(buf);
         break;

      case PAUSE_TIME:
         copy_screen(170, 190, 470, 290);
         GWinCent(C_MENSAJES);
         T_PAUSE = read_input_value(180, 240, ROJO, "Digite valor de pausa ms: ");
         putimage(170, 190, buf, COPY_PUT);
         free(buf);
         break;

      case FINAL_DATA:
         show_wnd_data_table (Vo, Bo, Ho, Tf);
         GWinXY(A_GRAFICA);
         break;

      case RUN:
         color_g++;
         if (color_g == BLANCO)
            color_g = AZUL;
         render_graphic(Vo, Bo, Ho, T_PAUSE, ORIGIN_X, ORIGIN_Y, color_g);
         break;

      case F1:
         restorecrtmode();
         show_wnd_help();
         initialize_graph_mode();
         draw_background(BLANCO, BLANCO, AZUL_C, 9);
         GWinXY(A_GRAFICA);
         GWinXY(B_TAREAS);
         show_button();
         break;

      case CLEAR:
         GWinXY(A_GRAFICA);
         color_g = AZUL;
         break;

      case EXIT:
         break;

      default:
         mysound(sCUATRO, DEF_LY);
         break;
      }

   } while (input_key != SCAPE);

   closegraph();
}

void copy_screen(int left, int top, int right, int bottom)
{
   size = imagesize(left, top, right, bottom);
   if ((buf = malloc(size)) == NULL)
   {
      closegraph();
      printf("Error: Memoria insuficiente     \n"
             "Presione una TECLA para terminar \n");
      getch();
      exit(1);
   }
   getimage(left, top, right, bottom, buf);
}

#define COORMAX_X 360
#define COORMAX_Y 360
#define MAX_PUNTOS 800

double render_graphic(double V, double B, double H, int time, int Xo, int Yo, int graphic_color)
{
   int max_x = getmaxx() - 19, Y2 = 40, cord_max_y, CONTINUE,
       nn, n, pp = nCERO, p, y_cord, x_cord, corxy[MAX_PUNTOS];

   float f_scale;

   double g = CONST_GRAVITY_SI, Vox, Voy, SEN_B, COS_B, t, SEN_2B, TIEMPO_MAS,
          Vx[MAX_PUNTOS], Vy[MAX_PUNTOS], Time[MAX_PUNTOS], Sx[MAX_PUNTOS],
          Sy[MAX_PUNTOS], ALCANCE_X, TIEMPO_DE_VUELO, ALTURA_MAXIMA;

   char *cx[80], *cy[80], Tex[80];

   //-- draw coordiates
   setcolor(ROJO);
   setlinestyle(SOLID_LINE, 1, 20);

   line(Xo, Yo, max_x, Yo); // Eje X
   line(Xo, Y2, Xo, Yo);    // Eje Y

   outtextxy(max_x + 10, Yo - 4, "x");
   outtextxy(Xo - 3, 30, "y");
   outtextxy(Xo, Yo + 3, "o");
   outtextxy(max_x, Yo - 3, "*");
   outtextxy(Xo - 3, 38, "*");

   settextstyle(0, 1, 0);
   outtextxy(Xo - 3, 45, "360");
   settextstyle(0, 0, 0);
   outtextxy(Xo + 580, Yo + 10, "580");

|  //-- compute components
   SEN_B = operacion(B, uSEN, GRAD);
   SEN_2B = operacion(2.0 * B, uSEN, GRAD);
   COS_B = operacion(B, uCOS, GRAD);
   Vox = V * COS_B;
   Voy = V * SEN_B;

   //-- comput max-conditions
   ALCANCE_X = V * V * SEN_2B / (2.0 * g) * (1 + pow(1 + 2 * g * H / (Voy * Voy), 0.5));
   TIEMPO_DE_VUELO = ALCANCE_X / Vox;
   ALTURA_MAXIMA = H + Voy * Voy / (2.0 * g);

   if (ALCANCE_X > ALTURA_MAXIMA) {
      f_scale = ceil(ALCANCE_X / 360.0);
   }
   else {
      f_scale = ceil(ALTURA_MAXIMA / 360.0);
   }

   if (ALCANCE_X <= 100 && ALTURA_MAXIMA <= 100) f_scale = 0.9;
   if (ALCANCE_X <= 50 && ALTURA_MAXIMA <= 50) f_scale = 0.8;
   if (ALCANCE_X <= 25 && ALTURA_MAXIMA <= 25) f_scale = 0.04;
   if (ALCANCE_X <= 10 && ALTURA_MAXIMA <= 10) f_scale = 0.02;

   TIEMPO_MAS = ceill((TIEMPO_DE_VUELO + 40) / 80) / 10;
   if (TIEMPO_DE_VUELO <= 80) TIEMPO_MAS = 0.1;

   //-- calculate s,v for each time (t)
   for (t = 0.0, p = 0; t <= TIEMPO_DE_VUELO; t = t + TIEMPO_MAS, p++)
   {
      if (t + TIEMPO_MAS > TIEMPO_DE_VUELO) t = TIEMPO_DE_VUELO;
      Time[p] = t;
      Sx[p] = Vox * t;
      Sy[p] = H + Voy * t - g * t * t / 2.0;
      Vx[p] = Vox;
      Vy[p] = Voy - g * t;
   }

   setcolor(BLANCO);
   outtextxy(30, 30, "================================");
   sprintf(Tex, "Escala 1:%.2f", f_scale);
   setcolor(ROJO_C);
   outtextxy(30, 30, Tex);

   for (nn = 0, n = 150; nn <= 2; nn++, n = n + 20)
   {
      setcolor(BLANCO);
      outtextxy(490, n, "===============================");
   }

   setcolor(ROJO);
   sprintf(Tex, "Vo [m/s] = %.3lf ", V);
   outtextxy(490, 150, Tex);
   sprintf(Tex, "Bo [ ° ] = %.3lf ", B);
   outtextxy(490, 170, Tex);
   sprintf(Tex, "Ho [ m ] = %.3lf ", H);
   outtextxy(490, 190, Tex);

   //-- render parabolic motion
   CONTINUE = TRUE;
   cord_max_y = (-1) * ALTURA_MAXIMA / f_scale + ORIGIN_Y;
   while (pp < p && !kbhit())
   {
      x_cord = (1) * Sx[pp] / f_scale + ORIGIN_X;
      y_cord = (-1) * Sy[pp] / f_scale + ORIGIN_Y;
      putpixel(x_cord, y_cord, graphic_color);
      delay(time * TIEMPO_MAS * 10);

      for (nn = 0, n = 50; nn <= 4; nn++, n = n + 20)
      {
         setcolor(BLANCO);
         outtextxy(490, n, "================================");
      }

      setcolor(AZUL_C);
      sprintf(Tex, "t  [ s ] = %.3lf ", Time[pp]);
      outtextxy(490, 50, Tex);
      sprintf(Tex, "x  [ m ] = %.3lf ", Sx[pp]);
      outtextxy(490, 70, Tex);
      sprintf(Tex, "y  [ m ] = %.3lf ", Sy[pp]);
      outtextxy(490, 90, Tex);
      sprintf(Tex, "vx [m/s] = %.3lf ", Vx[pp]);
      outtextxy(490, 110, Tex);
      sprintf(Tex, "vy [m/s] = %.3lf ", Vy[pp]);
      outtextxy(490, 130, Tex);

      if (y_cord == cord_max_y && pp > nCERO && CONTINUE != FALSE)
      {
         setcolor(graphic_color);
         outtextxy(x_cord - 2, y_cord - 5, "+");
         CONTINUE = FALSE;
      }

      pp++;
   }
   setcolor(graphic_color);
   outtextxy(x_cord - 5, y_cord - 5, "*");
   mysound(MI, 30);

   return (TRUE);
}

#define INITIAL_X 150
#define INITIAL_Y 140
#define INCREMENT_X 300
#define INCREMENT_Y 50

#define T_MORE 43
#define T_LESS 45
#define T_INTERVAL 105
#define MIN_TIME 0

void show_wnd_data_table (double V, double B, double H, double t)
{

   char input_key, Tex[80], msg_F[6][80];
   char *TITULOS_X[] = {"Al inicio", "Despues de t", "En el Impacto"};
   char *TITULOS_Y[] = {"Posición", "Velocidad", "En H es Max"};
   char *TITULOS_dY[] = {"Sx", "Sy", "S", "Vx", "Vy", "Bo", "V", "t", "H"};

   double g = CONST_GRAVITY_SI, T[3], Vox, Voy, SEN_2B, SEN_B, COS_B, TAN_B,
          Sx[3], Sy[3], Sf[3], Vx[3], Vy[3], Vf[3], B_[3], TanB_[3],
          I_time = 1.0, ALCANCE_X, TIEMPO_DE_VUELO, ALTURA_MAXIMA,
          TIEMPO_DE_H_MAX, ALCANCE_EN_H_MAX;

   int x, y, i, j;

   S_GWIN_CENT C_DATOS =
       {EXTERIOR, 600, 350, 1, AZUL_C, ROJO_C, BLANCO, " Análisis de datos "};

   do
   {
      GWinCent(C_DATOS);

      //-- draw table
      x = INITIAL_X, y = INITIAL_Y;
      for (i = 30, j = 1; i <= 30 * 8; i = i + 25, j++)
      {
         if (j == 0 || j == 3 || j == 7 || j == 9 || j == 12)
         {
            setcolor(ROJO_C);
            setlinestyle(SOLID_LINE, 10, 45);
         }
         rectangle(x, y, x + 450, y + i);
         setcolor(BLANCO);
      }

      //-- columns
      for (i = INITIAL_X + 50; i <= 600; i = i + 140)
      {
         line(i, INITIAL_Y, i, INITIAL_Y + 230);
      }
      setcolor(AMARILLO);

      //-- main-column titles
      for (i = 0, j = INITIAL_X + 50; i <= 2; i++, j = j + 140)
      {
         outtextxy(j, INITIAL_Y - 35, TITULOS_X[i]);
      }

      //-- rows titles
      for (i = 0, j = INITIAL_Y + 20; i <= 2; i++, j = j + 80)
      {
         outtextxy(INITIAL_X - 100, j, TITULOS_Y[i]);
      }

      //-- 1st-column titles
      for (i = 0, j = INITIAL_Y + 15; i <= 8; i++, j = j + 25)
      {
         outtextxy(INITIAL_X + 20, j, TITULOS_dY[i]);
      }

      //-- compute components
      SEN_B = operacion(B, uSEN, GRAD);
      SEN_2B = operacion(2.0 * B, uSEN, GRAD);
      COS_B = operacion(B, uCOS, GRAD);

      Vox = V * COS_B;
      Voy = V * SEN_B;

      //-- MAX-values
      ALCANCE_X = V * V * SEN_2B / (2.0 * g) * (1 + pow(1 + 2 * g * H / (Voy * Voy), 0.5));
      TIEMPO_DE_VUELO = ALCANCE_X / Vox;
      ALTURA_MAXIMA = H + Voy * Voy / (2.0 * g);

      T[0] = 0.0;
      T[1] = t, T[2] = TIEMPO_DE_VUELO;

      TIEMPO_DE_H_MAX = Voy / g;
      ALCANCE_EN_H_MAX = Vox * TIEMPO_DE_H_MAX;

      sprintf(msg_F[0], "%.4lf", TIEMPO_DE_H_MAX);
      sprintf(msg_F[1], "%s", "Altura maxima");
      sprintf(msg_F[2], "%.4lf", ALTURA_MAXIMA);
      sprintf(msg_F[3], "%s", " ");
      sprintf(msg_F[4], "%s", "x en H max");
      sprintf(msg_F[5], "%.4lf", ALCANCE_EN_H_MAX);

      //-- compute results
      for (i = 0; i <= 2; i++)
      {
         Sx[i] = Vox * T[i];
         Sy[i] = H + Voy * T[i] - g * T[i] * T[i] / 2.0;
         Sf[i] = pow((Sx[i] * Sx[i] + Sy[i] * Sy[i]), 0.50);
         Vx[i] = Vox;
         Vy[i] = Voy - g * T[i];

         TanB_[i] = Vy[i] / Vx[i];
         B_[i] = operacion(TanB_[i], uArTAN, GRAD);

         Vf[i] = pow((Vx[i] * Vx[i] + Vy[i] * Vy[i]), 0.50);
      }

      //-- display results
      for (i = 0, j = INITIAL_X + 60; i <= 2; i++, j = j + 140)
      {
         if (i == 1)
            setcolor(BLANCO);
         else
            setcolor(VERDE_C);
         sprintf(Tex, "t = %.4lf [s]", T[i]);
         outtextxy(j - 10, INITIAL_Y - 15, Tex);

         sprintf(Tex, "%.4lf", Sx[i]);
         outtextxy(j, INITIAL_Y + 10, Tex);
         sprintf(Tex, "%.4lf", Sy[i]);
         outtextxy(j, INITIAL_Y + 40, Tex);
         sprintf(Tex, "%.4lf", Sf[i]);
         outtextxy(j, INITIAL_Y + 65, Tex);

         sprintf(Tex, "%.4lf", Vx[i]);
         outtextxy(j, INITIAL_Y + 90, Tex);
         sprintf(Tex, "%.4lf", Vy[i]);
         outtextxy(j, INITIAL_Y + 115, Tex);
         sprintf(Tex, "%.3lf", B_[i]);
         outtextxy(j, INITIAL_Y + 140, Tex);
         sprintf(Tex, "%.4lf", Vf[i]);
         outtextxy(j, INITIAL_Y + 165, Tex);

         outtextxy(j, INITIAL_Y + 190, msg_F[i]);
         outtextxy(j, INITIAL_Y + 215, msg_F[i + 3]);
      }

      input_key = getch();
      switch (input_key)
      {
      case T_MORE:
         t = t + I_time;
         if (t >= TIEMPO_DE_VUELO)
            t = MIN_TIME;
         break;
      case T_LESS:
         t = t - I_time;
         if (t < MIN_TIME)
            t = TIEMPO_DE_VUELO;
         break;
      case T_INTERVAL:
         GWinCent(C_MENSAJES);
         I_time = read_input_value(180, 240, ROJO, "Digita valor del Intervalo: ");
         break;
      }
   } while (input_key != SCAPE);
}

#define MAXDIGIT 5

double read_input_value(int x, int y, int COLOR_NUM, char TEXTO[])
{
   char *fincadena, cDIG[MAXDIGIT + 1], sinput_key[1];
   int i, j, AUXKEY, input_key, PUNTO = FALSE;
   double valor;

   i = nCERO, input_key = cVACIO;
   setcolor(TEXT_FORECOLOR);
   outtextxy(x, y, TEXTO);

   x = x + strlen(TEXTO) * 8;
   do
   {
      do
      {
         setcolor(BLANCO);
         outtextxy(x + i * 8, y, "**");
         setcolor(COLOR_NUM);
         input_key = getch();
         AUXKEY = input_key;
         sprintf(sinput_key, "%c", AUXKEY);
         outtextxy(x + i * 8, y, sinput_key);

         if (input_key == cNEGATIVO && i == nCERO)
            break;
         if (input_key == cPUNTO && PUNTO == FALSE)
         {
            PUNTO = TRUE;
            break;
         }
         if (input_key == BORRA_C && i >= nUNO)
         {
            i--;
            if (cDIG[i - 1] == cPUNTO)
               ;
            PUNTO = FALSE;
         }

      } while (!isdigit(input_key) && input_key != uENTER);

      cDIG[i] = input_key;
      if (i <= MAXDIGIT)
         i++;
   } while (input_key != uENTER);

   valor = strtod(cDIG, &fincadena);
   setcolor(BLANCO);

   return (valor);
}

void show_button()
{
   int i;
   for (i = 0; i <= (MAX_BUTTON - 1); i++)
   {
      if (strlen(tex_button[i]) == 0)
         break;
      GBoton(boton[i]);
   }
}

void init_button(int x, int y)
{
   int i, colboton = GRIS, coltex_button = ROJO;

   for (i = 0; i <= (MAX_BUTTON - 1); i++)
   {
      if (strlen(tex_button[i]) == 0)
         break;
      boton[i].nWide = 20;
      boton[i].nModo = EXTERIOR;
      boton[i].nColorBack = colboton;
      boton[i].nColorText = coltex_button;
      boton[i].cText = tex_button[i];
      boton[i].nX1 = x;
      boton[i].nY1 = y;

      x = x + SPACE_BUTTON_X;
   }
}

#define TIME 100
char press_button(char input_key)
{
   int i = 0;
   do
   {
      if (input_key == assci_key[i])
      {
         boton[i].nModo = INTERIOR;
         GBoton(boton[i]);
         delay(TIME);
         boton[i].nModo = EXTERIOR;
         GBoton(boton[i]);
         return (TRUE);
      }
      i++;
   } while (i <= MAX_BUTTON);

   return (FALSE);
}

void draw_background(int background_color, int text_color, int fill_color, int fill_char)
{
   setbkcolor(background_color);
   setcolor(text_color);
   setfillstyle(fill_char, fill_color);
   bar(0, 0, getmaxx(), getmaxy());
}

void show_wnd_about(int bg_color)
{

   setbkcolor(bg_color);
   bar(0, 0, getmaxx(), getmaxy());
   settextstyle(7, 0, 3);
   setcolor(ROJO);
   outtextxy(300, 20, "     K I N E M A T I C ");
   outtextxy(300, 70, "       C A L C U L O S ");
   outtextxy(300, 120, "C I N E M A T I C O S ");

   setcolor(AZUL);
   settextstyle(0, 0, 10);
   outtextxy(10, 20, "~~~~");
   settextstyle(0, 0, 10);
   outtextxy(10, 70, " ** ");

   setcolor(BLANCO);
   settextstyle(1, 0, 6);
   outtextxy(95, 72, "UAEM");
   setcolor(VERDE);
   settextstyle(1, 0, 2);
   outtextxy(145, 125, "FCQeI");

   setcolor(AZUL_C);
   settextstyle(0, 0, 2);
   outtextxy(100, 320, "  ClusterBR | ArBR  "); //--updated (2025)
   outtextxy(100, 350, "         FCQeI      ");
}

#define F5 63
#define MAX_LINE 23
#define KEY_AV 81
#define KEY_HOME 71
#define MAX_BYTE 5237
#define MSG_ERR_OPENING_FILE "No es posible abrir el archivo.\n"

int show_wnd_help()
{
   FILE *p;

   long byte = nCERO, lin = nCERO;
   char msg[2517], ch, input_key, msg2[100];
   char file_name[] = "kinem.hlp";

   if ((p = fopen(file_name, "rt")) == NULL)
   {
      fprintf(stderr, MSG_ERR_OPENING_FILE);
      getch();
      return (0);
   }

   set_text_color(BLANCO);
   set_text_background(AZUL);
   clrscr();

   do
   {
      fseek(p, byte, SEEK_SET);
      lin = 0;
      do
      {
         ch = fgetc(p);
         printf("%c", ch);
         if (ch == '\n')
            lin++;
         byte = ftell(p);
      } while (lin <= MAX_LINE && byte <= MAX_BYTE - 1);

      do
      {
         input_key = getch();
         if (input_key == F5)
         {
            print_file(file_name);
            input_key = KEY_HOME;
         }
         if (input_key == KEY_HOME)
         {
            byte = nCERO;
            clrscr();
         }
      } while (input_key != KEY_AV && input_key != KEY_HOME && input_key != SCAPE);

      if (byte >= MAX_BYTE || byte < nCERO)
      {
         byte = nCERO;
         clrscr();
      }

   } while (input_key != SCAPE);

   fclose(p);
   return (1);
}

#define BEGIN 740
#define MSG_ERR_PRINT_FILE "Error, Imposible print_file\n       "
#define MSG_ERR_ACCESING_PRINTER "Error al accesar a la Impresora\n "

int print_file(char file_name[])
{
   FILE *ip, *PRINT;
   char ch;
   int i = BEGIN;
   if ((PRINT = fopen("PRN", "wt")) == NULL)
   {
      fprintf(stderr, MSG_ERR_ACCESING_PRINTER);
      getch();
      return (-1);
   }

   if ((ip = fopen(file_name, "rt")) == NULL)
   {
      fprintf(stderr, MSG_ERR_PRINT_FILE);
      getch();
      return (-1);
   }

   fseek(ip, BEGIN, SEEK_SET);
   do
   {
      ch = fgetc(ip);
      fputc(ch, PRINT);
      i++;
      fseek(ip, i, SEEK_SET);
   } while (i <= MAX_BYTE);

   fclose(PRINT);
   fclose(ip);

   return (1);
}
