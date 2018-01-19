#ifndef PIN_MAP_H_
#define PIN_MAP_H_
typedef struct pinMap{
  int A[16]; // gpio analog pins
  int Asize;
  int D[28]; // digital gpio pins
  int Dsize;
  int S[ 4]; // Servo Pins
  int Ssize; 
} PinMap;

PinMap init_mega_PinMap(void) {
   PinMap pm{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
             16,
             {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
              35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
              48, 49},
             28,
             {11,12, 0, 0},
          	 4};
}



int index_of_pin(PinMap map, char type, int pin){
	int size = 0;
	int* array = 0;
	int index = -1;
	if(type == 'A' or type == 'a') {
		size = map.Asize;
		array = map.A;
	}
	if (type == 'D' or type == 'd') {
		size = map.Dsize;
		array = map.D;
	}
	if(type == 'S' or type == 's') {
		size = map.Ssize;
		array = map.S;
	}

	for(int i = 0; i < size; i++){
		if (pin == array[i])
		{
			index = i;
			break;
		}
	}
	return index;
}

#endif // PIN_MAP
