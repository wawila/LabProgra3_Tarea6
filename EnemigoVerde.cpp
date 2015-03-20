#include "EnemigoVerde.h"

//
//EnemigoVerde::EnemigoVerde(SDL_Renderer* renderer, list<Personaje*> *personajes)
//{
//    vector<SDL_Texture*>*texturas = new vector<SDL_Texture*>();
//    vector<SDL_Texture*>*texturas_left = new vector<SDL_Texture*>();
//
//    texturas->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing/1.png"));
//    texturas->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing/2.png"));
//    texturas->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing/3.png"));
//    texturas->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing/4.png"));
//
//    texturas_left->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing_left/1.png"));
//    texturas_left->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing_left/2.png"));
//    texturas_left->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing_left/3.png"));
//    texturas_left->push_back(IMG_LoadTexture(renderer,"EnemigoVerde/standing_left/4.png"));
//
//    mapa_texturas[ANIMACION_IDLE_RIGHT] = texturas_left;
//    mapa_texturas[ANIMACION_IDLE_RIGHT] = texturas;
//
//    estado_actual = DERECHA;
//
//    rect.x = 500;
//    rect.y = 250;
//
//    init(renderer, personajes);
//}
//

EnemigoVerde::EnemigoVerde(SDL_Renderer* renderer, list<Personaje*> *personajes)
{
    ifstream in("personaje_verde.txt");

    if(!in)
    {
        cout<<"Error no se encontro el archivo!"<<endl;
        return;
    }

    string animacion;
    while(in>>animacion)
    {
        string llave;
        in>>llave;
        string path;
        in>>path;
        mapa_texturas[enumStringToInt(animacion)] = new vector<SDL_Texture*>();
        while(path!="}")
        {
            mapa_texturas[enumStringToInt(animacion)]->push_back(IMG_LoadTexture(renderer,path.c_str()));
            in>>path;
        }
    }

    estado_actual = DERECHA;

    rect.x = 350;
    rect.y = 50;

    estado = INICIO;
    frame_estado = 0;

    init(renderer,personajes);
}

EnemigoVerde::~EnemigoVerde()
{
    //dtor
}

int EnemigoVerde::estadoRandom()
{
    Personaje* sho = NULL;
    for(list<Personaje*>::iterator i = personajes->begin();
        i!=personajes->end();
        i++)
    {
        if((*i)->soySho())
        {
            sho = (*i);
            if(colision(sho->hitbox))
            {
                return true;
            }
        }
    }
    if(sho!=NULL)
    {
        int mi_random = rand()%2;

        if(mi_random==0)
        {
            if(sho->rect.y>rect.y)
                return ABAJO;

            if(sho->rect.y<rect.y)
                return ARRIBA;
        }
        else
        {
            if(sho->rect.x<rect.x)
            {
                return IZQUIERDA;
            }

            if(sho->rect.x>rect.x)
                return DERECHA;
        }
    }
}

bool EnemigoVerde::estoyCerca()
{
    Personaje* sho = NULL;
    for(list<Personaje*>::iterator i = personajes->begin();
        i!=personajes->end();
        i++)
    {
        if((*i)->soySho())
        {
            sho = (*i);
            if(colision(sho->hitbox))
            {
                return true;
            }
        }
    }


    return false;
}

void EnemigoVerde::cambiarEstado(int estado)
{
    this->estado = estado;
    frame_estado = 0;
}

void EnemigoVerde::act()
{
//    if(frame%100==1)
//        atacando = true;
//    else
//        atacando = false;

    frame_estado++;
    switch(estado)
    {
        case INICIO:
            atacando = false;
            if(estoyCerca())
                cambiarEstado(CERCA);
            else
                cambiarEstado(LEJOS);
        break;

        case CERCA:
            estado = ATACANDO_STARTUP;
            if(orientacion=='r')
                setAnimacion(ANIMACION_ATACANDO_STARTUP_RIGHT);
            else
                setAnimacion(ANIMACION_ATACANDO_STARTUP_LEFT);
        break;

        case ATACANDO_STARTUP:
            if(frame_estado%200==0)
            {
                atacando = true;
                cambiarEstado(ATACANDO_ACTIVE);
                if(orientacion=='r')
                    setAnimacion(ANIMACION_ATACANDO_ACTIVE_RIGHT);
                else
                    setAnimacion(ANIMACION_ATACANDO_ACTIVE_LEFT);
            }
        break;

        case ATACANDO_ACTIVE:
            if(frame_estado%200==0)
            {
                atacando = false;
                cambiarEstado(ATACANDO_RECOVERY);
                if(orientacion=='r')
                    setAnimacion(ANIMACION_ATACANDO_RECOVERY_RIGHT);
                else
                    setAnimacion(ANIMACION_ATACANDO_RECOVERY_LEFT);
            }
        break;

        case ATACANDO_RECOVERY:
            if(frame_estado%200==0)
            {
                cambiarEstado(INICIO);
                setAnimacion(ANIMACION_IDLE_RIGHT);
            }
        break;

        case LEJOS:
            estado = estadoRandom();
            switch(estado)
            {
                case ARRIBA:
                    setAnimacion(ANIMACION_WALKING_RIGHT);
                    orientacion='r';
                break;
                case ABAJO:
                    setAnimacion(ANIMACION_WALKING_LEFT);
                    orientacion='l';
                break;
                case DERECHA:
                    setAnimacion(ANIMACION_WALKING_RIGHT);
                    orientacion='r';
                break;
                case IZQUIERDA:
                    setAnimacion(ANIMACION_WALKING_LEFT);
                    orientacion='l';
                break;
            }

        break;

        case IZQUIERDA:
            rect.x--;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case DERECHA:
            rect.x++;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case ARRIBA:
            rect.y--;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case ABAJO:
            rect.y++;
            if(frame_estado%100==0)
                cambiarEstado(INICIO);
        break;

        case PARADO:
            if(frame_estado%50==0)
                cambiarEstado(INICIO);
        break;
    }

//    this->estado_actual = estado;

}
