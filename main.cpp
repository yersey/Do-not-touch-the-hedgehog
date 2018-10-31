#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>
#include <sstream>

using namespace std;

SDL_Window*okno;
SDL_Surface*ekran;
SDL_Event zdarzenie;

int frame=1;

SDL_Rect rect1;
SDL_Rect rect2;
SDL_Rect rect3;
SDL_Rect rect4;



int main( int argc, char * args[] )
{
    SDL_Init( SDL_INIT_EVERYTHING );
    okno=SDL_CreateWindow("Do not touch the hedgehog!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
    ekran=SDL_GetWindowSurface(okno);

    //tekst

    TTF_Init();
    TTF_Font*comic=TTF_OpenFont("comic.ttf",200);
    SDL_Color kolor={3, 0, 253};
    SDL_Surface*tekst;

    //muzyka i dzwieki

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT ,2,1024);
    Mix_Music*muzyka_tlo=Mix_LoadMUS("muzyka/muzyka.wav");
    Mix_PlayMusic(muzyka_tlo, -1);
    Mix_Chunk*zycie=Mix_LoadWAV("muzyka/zycie.wav");
    Mix_Chunk*punkt=Mix_LoadWAV("muzyka/punkt.wav");
    Mix_Chunk*koniec=Mix_LoadWAV("muzyka/przegrana.wav");

    bool stanp=false;
    bool muzyka=true;
    bool stanup=false;

    while(true)
    {
//frame poczatek
         if(frame==1)
        {
            //wczytywanie i renderowanie tla starowego
            SDL_Surface*obraz_tlo=IMG_Load("grafika/start.bmp");

            rect2.x=0;
            rect2.y=0;

            SDL_BlitSurface(obraz_tlo, NULL, ekran, &rect2);
            //wyjscie na krzyzyk
            while(frame==1)
            {
                while(SDL_PollEvent(&zdarzenie))
                {
                    if(zdarzenie.type==SDL_QUIT)
                    {
                        return 0;
                    }
                }


                SDL_UpdateWindowSurface(okno); //update surfaceow w oknie

                //przejscie do frame 0 na spacje
                if(GetAsyncKeyState(VK_SPACE))
                {
                    frame=0;
                }
            }
        }

//frame gra
        if(frame==0)
        {
            int posX=10;
            int posY=380;
            int posX_jez=500;
            int posY_jez=430;
            int vel_jez=2;
            int zycia=3;
            int punkty=0;
            int grawitacja=8;
            int skok=0;

            //wczytywanie grafik
            SDL_Surface*obraz_tlo=IMG_Load("grafika/tlo.bmp");
            SDL_Surface*obraz_postac=IMG_Load("grafika/postac.bmp");
            SDL_Surface*obraz_jez=IMG_Load("grafika/jez.bmp");
            SDL_Surface*jeden=IMG_Load("grafika/1.bmp");
            SDL_Surface*dwa=IMG_Load("grafika/2.bmp");
            SDL_Surface*trzy=IMG_Load("grafika/3.bmp");

            while(frame==0)
            {

                //zamiana int punkty na string
                stringstream konwersja;
                konwersja << punkty;
                tekst=TTF_RenderText_Blended(comic,konwersja.str().c_str(), kolor);

                //wyjscie krzyzykiem
                while(SDL_PollEvent(&zdarzenie))
                {
                    if(zdarzenie.type==SDL_QUIT)
                    {
                        return 0;
                    }
                }

                //wyciszanie
                if(GetAsyncKeyState(0x50) && stanp==false && muzyka==true)
                {
                    stanp=true;
                    muzyka=false;
                    Mix_PauseMusic();
                }
                if(GetAsyncKeyState(0x50) && stanp==false && muzyka==false)
                {
                    stanp=true;
                    Mix_ResumeMusic();
                    muzyka=true;
                }
                if(!GetAsyncKeyState(0x50))
                {
                    stanp=false;
                }


                //sterowanie
                if(GetAsyncKeyState(VK_LEFT))
                {
                    posX -= 5;
                }

                if(GetAsyncKeyState(VK_RIGHT))
                {
                    posX += 5;
                }

                if(GetAsyncKeyState(VK_UP) && stanup==false)
                {
                    stanup=true;
                    //skok1
                    if(posY>=380) skok=20;

                }
                if(!GetAsyncKeyState(VK_UP)) stanup=false;

                //renderowanie tla
                rect2.x=0;
                rect2.y=0;
                SDL_BlitSurface(obraz_tlo, NULL, ekran, &rect2);

                //renderowanie zycia
                rect3.x=20;
                rect3.y=15;
                rect3.w=160;
                rect3.h=40;

                if(zycia==3)
                    SDL_BlitScaled(trzy, NULL, ekran, &rect3);
                if(zycia==2)
                    SDL_BlitScaled(dwa, NULL, ekran, &rect3);
                if(zycia==1)
                    SDL_BlitScaled(jeden, NULL, ekran, &rect3);
                //renderowanie punktow
                rect4.x=320;
                rect4.y=-50;
                SDL_BlitSurface(tekst, NULL, ekran, &rect4);
                SDL_FreeSurface(tekst); //usuwanie surface



                //renderowanie postaci i jeza
                rect2.x=posX;
                rect2.y=posY;
                rect2.w=100;
                rect2.h=100;

                rect1.x=posX_jez;
                rect1.y=posY_jez;
                rect1.w=60;
                rect1.h=45;

                SDL_BlitScaled(obraz_postac, NULL, ekran, &rect2);
                SDL_BlitScaled(obraz_jez, NULL, ekran, &rect1);

                //skok2
                if(skok>0)
                {
                    skok--;
                    posY=posY-10;
                }

                //grawitacja
                if(posY<380 && skok==0)
                {
                    posY=posY+grawitacja;
                }

                //poruszanie sie jeza
                posX_jez=posX_jez-vel_jez;

                //dodawanie punktu
                if(posX_jez<=-100)
                {
                    posX_jez=785;
                    punkty++;
                    Mix_PlayChannel(-1,punkt,0);
                    vel_jez++;
                }

                //zderzenie
                if(posX+100>posX_jez && posY+100>posY_jez && posX<posX_jez+60)
                {
                    posX_jez=500;
                    posX=10;
                    posY=380;
                    zycia--;
                    Mix_PlayChannel(-1,zycie,0);
                    if(zycia<=0)
                    {
                        frame=2; //sprawdzanie zycia i przejscie do frame 2

                            //czysczenie surface
                            SDL_FreeSurface(obraz_tlo);
                            SDL_FreeSurface(obraz_postac);
                            SDL_FreeSurface(obraz_jez);
                            SDL_FreeSurface(jeden);
                            SDL_FreeSurface(dwa);
                            SDL_FreeSurface(trzy);
                    }
                }


                SDL_UpdateWindowSurface(okno);//update surfaceow w oknie
                SDL_Delay(16.666666); //odswiezanie
            }
        }
//frame przegrana
        if(frame==2)
        {
            //renderowanie tla koncowego
            rect2.x=0;
            rect2.y=0;
            SDL_Surface*tlokoncowe=IMG_Load("grafika/koniec.bmp");
            SDL_BlitSurface(tlokoncowe, NULL, ekran, &rect2);
            //pauzowanie muzyki i puszczanie koncowej
            Mix_PauseMusic();
            Mix_PlayChannel(-1,koniec,0);
            //wyjscie krzyzykiem
            while(frame==2)
            {
                while(SDL_PollEvent(&zdarzenie))
                {
                    if(zdarzenie.type==SDL_QUIT)
                    {
                        return 0;
                    }
                }
                SDL_UpdateWindowSurface(okno); //update surfaceow w oknie

                //przejscie do frame 0 na spacje
                if(GetAsyncKeyState(VK_SPACE))
                {
                    if(muzyka==true) Mix_ResumeMusic(); //wznawianie muzyki
                    SDL_FreeSurface(tlokoncowe); //czyszczenie surface
                    frame=0;
                }
            }
        }


    } //koniec petli glównej

    return 0;
}
