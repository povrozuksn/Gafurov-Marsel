#include "TXLib.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "dirent.h"


struct Button
 {
  int x;
  int y;
  const char* text;
  string category;
 };

void drawButton(Button btn)
 {

   txSetColor (TX_BLACK);
   txSetFillColor (TX_GRAY);
   Win32::RoundRect (txDC(), btn.x+2, btn.y+2, btn.x+100+2, btn.y+30+2, 20, 20);
   txSetFillColor (TX_WHITE);
   Win32::RoundRect (txDC(), btn.x, btn.y, btn.x+100, btn.y+30, 20, 20);
   txSelectFont("Times", 20);
   txDrawText(btn.x, btn.y, btn.x+100, btn.y+30, btn.text);
 }

struct Picture
 {

   int x;
   int y;
   string adress;
   HDC image;
   string category;
   int w;
   int h;
   bool visible;

 };

int readFromDir(string adress, Picture menuPicture[], int COUNT_PICS)
{
   DIR *dir;
   struct dirent *ent;
   int lastY = 20;
   if ((dir = opendir (adress.c_str())) != NULL)
   {
     while ((ent = readdir (dir)) != NULL)
     {
       if ((string)ent->d_name != "." &&
           (string)ent->d_name != "..`")
       {
           menuPicture[COUNT_PICS].y = lastY;
           menuPicture[COUNT_PICS].adress = adress + (string)ent->d_name;
           COUNT_PICS ++;
           lastY+=100;
       }
      }
       closedir (dir);
     }

     return COUNT_PICS;
}

void drawPicture(Picture pct)
 {
  if (pct.visible)
  {
  Win32::TransparentBlt(txDC(), pct.x, pct.y, 100, 100, pct.image, 0, 0, pct.w, pct.h, TX_BLACK);
  }
}

void drawPicture2(Picture pct)
 {
  if (pct.visible)
  {
  Win32::TransparentBlt(txDC(), pct.x, pct.y, pct.w, pct.w, pct.image, 0, 0, pct.w, pct.h, TX_BLACK);
  }
 }

 bool Click(Button btn)
 {
    if(txMouseButtons() == 1 &&
       txMouseX() >= btn.x &&
       txMouseX() <= btn.x+100 &&
       txMouseY() >= btn.y &&
       txMouseY() <= btn.y+30)
       {
       return 1;
       }
       else
       {
       return 0;
       }
 }

int get_h(string adress)
{
    unsigned char info[54];
    FILE* f = fopen(adress.c_str(), "rb");
    fread(info, sizeof(unsigned char), 54, f);
    int h = *(int*)&info[22];

    return h;
}
    /// найти ширину по адрессу
int get_w(string adress)
{
    unsigned char info[54];
    FILE* f = fopen(adress.c_str (), "rb");
    fread(info, sizeof(unsigned char), 54, f);
    int w = *(int*)&info[18];

    return w;
}



 int main()
 {
    txCreateWindow(1200, 600);

  int COUNT_PICS = 0;
  const int COUNT_BTNS = 9;
  const int START_PAGE = 0;

//Массив картинок меню
  Picture menuPicture[1000];

 //Массив центральный картинок
   Picture centralPicture[31];

  COUNT_PICS = readFromDir("Pictures/Walls/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Beds/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Carpets/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Sofas/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Chairs/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Tables/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Cupboards/", menuPicture, COUNT_PICS);
  COUNT_PICS = readFromDir("Pictures/Glasses/", menuPicture, COUNT_PICS);



 //массив кнопок
 Button btn[11];
 btn[0] = {50, 30,  "Стены",   "Walls"};
 btn[1] = {160, 30, "Кровати", "Beds"};
 btn[2] = {270, 30, "Ковры",   "Carpets"};
 btn[3] = {380, 30, "Диваны",  "Sofas"};
 btn[4] = {490, 30, "Стулья",  "Chairs"};
 btn[5] = {600, 30, "Столы",   "Tables"};
 btn[6] = {710, 30, "Шкафы",   "Cupboards"};
 btn[7] = {820, 30, "Зеркала", "Glasses"};
 btn[8] = {1080, 30, "Музыка", ""};
//* btn[9] = {1080, 100,"Выход", ""};
// btn[8] = {930, 30, "Лампы",   "Лампы"};



    int choose =-1;
    int numberpic = 0;
    int music_on = 0;
 /*   int lastpic = 0;
    int countpics = lastpic;
    int pageNumber = 0;   */
    bool mouse_release = false;

    for(int npic=0; npic<COUNT_PICS; npic++)
    {
      menuPicture[npic].x = 10;

      menuPicture[npic].image = txLoadImage(menuPicture[npic].adress.c_str());

      menuPicture[npic].w=get_w(menuPicture[npic].adress);
      menuPicture[npic].h=get_h(menuPicture[npic].adress);
      menuPicture[npic].visible=false;

      string stroka = menuPicture[npic].adress;
      int pos_1 = stroka.find("/");
      int pos_2 = stroka.find("/", pos_1+1);
      menuPicture[npic].category = stroka.substr(pos_1+1, pos_2-pos_1-1);
    }

    while(!GetAsyncKeyState (VK_ESCAPE))
    {
     txBegin();
     txSetColor (TX_WHITE);
     txSetFillColor (TX_BLUE);
     txRectangle (0, 0, 1200, 600);
 //Рисование кнопок
 for (int nk=0; nk<COUNT_BTNS; nk++)
 {
  drawButton (btn[nk]);
 }

 //Рисованiе картинок меню
 for (int npic=0; npic<COUNT_PICS; npic++)
 {
  drawPicture(menuPicture[npic]);
 }

//Рисование центральной картинки
for(int npic=0; npic<COUNT_PICS; npic++)
{
 drawPicture2(centralPicture[npic]);
}


//Видимость картинок меню по категории кнопки
for (int nk=0; nk<COUNT_BTNS; nk++)
{
   if (Click(btn[nk]))
        {
           for (int npic=0; npic<COUNT_PICS; npic++)
             {
                 menuPicture[npic].visible=false;
                 if (menuPicture[npic].category == btn[nk].category)
                    {
                      menuPicture[npic].visible=true;
                    }
             }
        }

}

//Видимость центральной картинок по категории меню
for (int npic=0; npic<COUNT_PICS; npic++)
{
  if (txMouseButtons() == 1 &&
      menuPicture[npic].visible &&
      txMouseX() >= menuPicture[npic].x &&
      txMouseX() <= menuPicture[npic].x+100 &&
      txMouseY() >= menuPicture[npic].y &&
      txMouseY() <= menuPicture[npic].y+100)
      {
         while(txMouseButtons() == 1)  txSleep(10);

         centralPicture[numberpic] = {300,
                                      100,
                                      menuPicture[npic].adress,
                                      menuPicture[npic].image,
                                      menuPicture[npic].category,
                                      menuPicture[npic].w,
                                      menuPicture[npic].h,
                                      menuPicture[npic].visible
                                      } ;
          numberpic++;
      }
}


 //выбор центральной картикнки
     for (int npic=0; npic<numberpic; npic++)
     {
          if (txMouseButtons() == 1 &&
          centralPicture[npic].visible &&
          txMouseX() >= centralPicture[npic].x &&
          txMouseX() <= centralPicture[npic].x+centralPicture[npic].w &&
          txMouseY() >= centralPicture[npic].y &&
          txMouseY() <= centralPicture[npic].y+centralPicture[npic].h)
          {
           choose=npic;
           mouse_release=false;
          }
     }
//передвижение центральной картинки
     if(choose>=0)
     {
       if(txMouseButtons() == 1 && !mouse_release)
          {
           centralPicture[choose].x = txMouseX() - centralPicture[choose].w/2;
           centralPicture[choose].y = txMouseY() - centralPicture[choose].h/2;
          }
      else
        if(txMouseButtons() != 1)
        {
        mouse_release=true;
        }
     }


//удаление картинок
if(choose>=0 && GetAsyncKeyState(VK_DELETE))
{
   centralPicture[choose] = centralPicture[numberpic - 1];
   numberpic--;
   choose=-1;
}

  char str[100];
  sprintf(str, "choose = %d numberpic = %d", choose, numberpic);
  txTextOut(800, 500, str);

 //музыка вкл/выкл
 if (Click(btn[8]))
    {
         if (music_on == 0)
         {
             txPlaySound("backmusic.wav");
             music_on = 1;
             txSleep(200);
         }
         else if (music_on == 1)
         {
             txPlaySound(NULL);
             music_on = 0;
             txSleep(200);
         }

     }

  /*
    if (Click(btn[9]))
   {
     pageNumber = START_PAGE;
     countpics = lastpic;
     lastpic = 0;
    }
        */
txSleep(50);
txEnd();
}

for (int npic=0; npic<COUNT_PICS; npic++)
{
 txDeleteDC(menuPicture[npic].image);
}

for (int npic=0; npic<numberpic; npic++)
{
 txDeleteDC(centralPicture[npic].image);
}


     return 0;

}








