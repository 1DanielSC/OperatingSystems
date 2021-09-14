#include "trem.h"
#include <QtCore>

#include <semaphore.h>
#include <pthread.h>

QMutex mutex[7];
QSemaphore sem[3], sem_2[2];


//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->x_0 = x;
    this->y_0 = y;
    this->velocidade = 200;
    this->incremento = 10;

}

void Trem::inicializarSemaphores(){

    sem[0].release(2); //SEMAFORO QUE CONTROLA O CRUZAMENTO A
    sem[1].release(2); //SEMAFORO QUE CONTROLA O CRUZAMENTO B
    sem[2].release(2); //SEMAFORO QUE CONTROLA O CRUZAMENTO C

    sem_2[0].release(3);//SEMAFORO QUE CONTROLA O CRUZAMENTO AB
    sem_2[1].release(3);//SEMAFORO QUE CONTROLA O CRUZAMENTO BC

}

void Trem::setVelocidade(const int value){
     this->velocidade = value;
     if (velocidade == VelocidadeMaxima) this->incremento = 0;
     else this->incremento = 10;

}


/*
 * 3 abstrações (regiões críticas):
 *      Mutex -> Dois trens
 *      Sem -> Três trens
 *      Sem_2 -> Quatro trens
 *
 *
*/


//Função a ser executada após executar trem->START
void Trem::run(){
    while(true){

        switch(this->ID){
        case 1:

                    if(x == 310 && y == 30){
                       //O trem1 precisará de dois recursos (trilhos) desse semaforo para realizar seu percurso
                       sem_2[0].acquire(2);
                       //O trem1 precisará de dois recursos (trilhos) desse semaforo para realizar seu percurso
                       sem[0].acquire(2);
                       //O trem1 precisará deste mutex para realizar seu percurso
                       mutex[0].lock();
                    }

                    if(x == 330 && y == 130){
                        mutex[2].lock();
                    }

                    if(x == 310 && y == 150){
                        sem_2[0].release(1);
                        sem[0].release(1);
                        mutex[0].unlock();
                    }


                    if(x == 170 && y == 150){
                        sem_2[0].release(1);
                        sem[0].release(1);
                        mutex[2].unlock();
                    }

                    break;

                case 2: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2

                    if(x == 580 && y == 30){

                        sem_2[1].acquire(3);
                        sem[2].acquire(2);
                        mutex[1].lock();
                    }
                    if(x == 600 && y == 130){
                        sem_2[0].acquire(3);
                        sem[1].acquire(2);
                        mutex[4].lock();
                    }
                    if(x == 580 && y == 150){
                        sem_2[1].release(1);
                        sem[2].release(1);
                        mutex[1].unlock();
                    }



                    if(x == 480 && y == 150){
                        sem[0].acquire(2);
                        mutex[3].lock();
                    }


                    if(x == 440 && y == 150){
                        sem_2[1].release(1);
                        sem_2[0].release(1);
                        sem[2].release(1);
                        sem[1].release(1);
                        mutex[4].unlock();
                    }


                    if(x == 350 && y == 150){
                        mutex[0].lock();
                    }


                    if(x == 330 && y == 130){
                        sem_2[1].release(1);
                        sem_2[0].release(1);
                        sem[1].release(1);
                        sem[0].release(1);
                        mutex[3].unlock();
                    }
                    if(x == 350 && y == 30){
                        sem_2[0].release(1);
                        sem[0].release(1);
                        mutex[0].unlock();
                    }
                    break;



                case 3:
                    if(x == 750 && y == 150){
                        sem_2[1].acquire(2);
                        sem[2].acquire(2);
                        mutex[5].lock();
                    }
                    if(x == 620 && y == 150){
                        mutex[1].lock();
                    }

                    if(x == 600 && y == 130){
                        sem_2[1].release(1);
                        sem[2].release(1);
                        mutex[5].unlock();
                    }
                    if(x == 620 && y == 30){
                        sem_2[1].release(1);
                        sem[2].release(1);
                        mutex[1].unlock();
                    }
                    break;



                case 4:
                    if(x == 190 && y == 170){
                        sem_2[1].acquire(2);
                        sem_2[0].acquire(3);
                        sem[0].acquire(2);
                        mutex[2].lock();
                    }
                    if(x == 310 && y == 150){
                        sem[1].acquire(2);
                        mutex[3].lock();
                    }
                    if(x == 350 && y == 150){
                        sem_2[0].release(1);
                        sem[0].release(1);
                        mutex[2].unlock();
                    }
                    if(x == 440 && y == 150){
                        mutex[6].lock();
                    }
                    if(x == 460 && y == 170){
                        sem_2[1].release(1);
                        sem_2[0].release(1);
                        sem[0].release(1);
                        sem[1].release(1);
                        mutex[3].unlock();
                    }
                    if(x == 440 && y == 270){
                        sem_2[1].release(1);
                        sem_2[0].release(1);
                        sem[1].release(1);
                        mutex[6].unlock();
                    }
                    break;



                case 5:
                    if(x == 480 && y == 270){
                        sem_2[1].acquire(3);
                        sem[1].acquire(2);
                        mutex[6].lock();
                    }
                    if(x == 460 && y == 170){
                        sem[2].acquire(2);
                        mutex[4].lock();
                    }
                    if(x == 480 && y == 150){
                        sem_2[1].release(1);
                        sem[1].release(1);
                        mutex[6].unlock();
                    }
                    if(x == 580 && y == 150){
                        mutex[5].lock();
                    }
                    if(x == 620 && y == 150){
                        sem_2[1].release(1);
                        sem[1].release(1);
                        sem[2].release(1);
                        mutex[4].unlock();
                    }
                    if(x == 730 && y == 170){
                        sem_2[1].release(1);
                        sem[2].release(1);
                        mutex[5].unlock();
                    }
                    break;

                default:
                    break;
        }

         if (y == y_0 && x < (x_0 + 270)){
             x+=incremento;
         }

         else if (x == (x_0 + 270) && y < (y_0 + 120)){
             y+=incremento;
         }

         else if (y == (y_0 + 120) && x > x_0){
             x-=incremento;
         }

         else y-=incremento;

         emit updateGUI(ID, x,y);    //Emite um sinal
         msleep(velocidade);
    }
}




