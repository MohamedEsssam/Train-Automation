//
// Created by mohamedessam on 12/7/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "Train_ Automation.h"


void main(void)
{
   Station station;
   station_init(&station);
   int trainsThread,passengersThread;

   //station input information.
   printf("enter number of Passengers waiting in station: ");
   scanf("%d",&passengersThread);
   printf("enter number of Trains that will works today : ");
   scanf("%d",&trainsThread);

   pthread_t trainsThreadArr[trainsThread],passengersThreadArr[passengersThread];
   struct loadTrain load[trainsThread];

   for(int i=0; i<trainsThread;i++)
   {
      printf("Enter Seat Number of Train %d : ",i+1);
      scanf("%d",&load[i].seatsNum);
      load[i].station = &station;
   }

      for (int i = 0; i < passengersThread; ++i)
      pthread_create(&passengersThreadArr[i], NULL, passenger_thread, &station);

      sleep(1);

      for (int i = 0; i < trainsThread; ++i)
      pthread_create(&trainsThreadArr[i], NULL, train_thread, &load[i]);

      for (int i = 0; i < passengersThread; ++i)
      pthread_join(&passengersThreadArr[i], NULL);

      for (int i = 0; i < trainsThread; ++i)
      pthread_join(&trainsThreadArr[i], NULL);

   return;
}
