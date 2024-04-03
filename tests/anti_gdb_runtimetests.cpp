#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>

// Globale Variablen
int globalVar1 = 0;
int globalVar2 = 0;
int globalVar3 = 0;

// Mutexe für kritische Abschnitte
std::mutex mutex1;
std::mutex mutex2;
std::mutex mutex3;

// executeFunction Methode
void executeFunction(int functionIndex) {
  // Temporäre Variablen für zusätzliche Rechenoperationen
  int temp1 = 0, temp2 = 0, temp3 = 0;

  // Dispatcher für die Funktionen basierend auf dem übergebenen Index
  switch (functionIndex) {
  case 1: {
    for (int i = 0; i < 5; ++i) {

      if (globalVar1 == 0) {
        globalVar1 += 5;
      } else if (globalVar2 > 10) {
        globalVar1 *= 2;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += globalVar1;
      globalVar3 = globalVar1 + globalVar2;
      globalVar1 *= globalVar3;
      globalVar2 -= globalVar3;
      globalVar3 /= 2;
    }
    if (globalVar1 < 20 && globalVar2 > 5) {
      globalVar1 += 10;
    } else {
      globalVar1 -= 5;
    }
    break;
  }
  case 2: {
    // Zusätzliche Rechenoperationen am Anfang (längere Ausführungsdauer)
    for (int i = 0; i < 3; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      // int temp = i * 2;
      // for (int i = 0; i < 10000000; ++i) {
      //	temp = ((i*2)%globalVar2);
      // }
      // std::this_thread::sleep_for(std::chrono::milliseconds(20));
      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 3: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    if (globalVar1 == 123) {
      globalVar1++;
    }
    // Mutex erst später sperren
    for (int i = 0; i < 4; ++i) {
      if (globalVar3 < 10) {
        globalVar3 += 10;
      } else {
        globalVar3 -= 7;
      }
      globalVar1 -= globalVar3;
      globalVar2 += globalVar3;
      globalVar3 *= 3;
      globalVar2 -= 5;
    }
    break;
  }
  case 4: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    int count3 = 0, temp5 = 0;
    while (count3 < 12) {
      temp5 ^= globalVar2;
      temp5 |= globalVar1;
      ++count3;
    }

    int temp4 = temp5 % 2;

    for (int i = 0; i < 2; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
    }
    break;
  }
  case 5: {
    for (int i = 0; i < 1; ++i) {
      if (globalVar1 == 0) {
        globalVar1 += 5;
      } else if (globalVar2 > 10) {
        globalVar1 *= 2;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += globalVar1;
      globalVar3 = globalVar1 + globalVar2;
      globalVar1 *= globalVar3;
      globalVar2 -= globalVar3;
    }
    if (globalVar1 < 20 && globalVar2 > 5) {
      globalVar1 += 10;
    } else {
      globalVar1 -= 5;
    }
    break;
  }
  case 6: {
    for (int i = 0; i < 10; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp = 0;
      for (int i = 0; i < 2; ++i) {
        temp = ((i * 2) + (globalVar2 + 1));
      }

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 7: {
    for (int i = 0; i < 4; ++i) {
      if (globalVar3 < 10) {
        globalVar3 += 10;
      } else {
        globalVar3 -= 7;
      }
      globalVar1 -= globalVar3;
      globalVar2 += globalVar3;
      globalVar3 *= 3;
      globalVar1 /= globalVar3;
      globalVar2 -= 5;
    }
    break;
  }
  case 8: {
    int count3 = 0, temp3 = 0;
    while (count3 < 100) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }
    int temp4 = temp3 % 2;
    // Verzögerung hinzufügen
    // Mutex erst später sperren
    for (int i = 0; i < 2; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    break;
  }
  case 9: { // Mutex erst später sperren
    for (int i = 0; i < 1; i++) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp5 = 0;
      for (int i = 0; i < 3; i++) {
        temp5 = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 1));
      }
      // std::this_thread::sleep_for(std::chrono::milliseconds(11));
      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 10: {
    for (int i = 0; i < 5; ++i) {
      if (globalVar1 == 0) {
        globalVar1 += 5;
      } else if (globalVar2 > 10) {
        globalVar1 *= 2;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += globalVar1;
      globalVar3 = globalVar1 + globalVar2;
      globalVar1 *= globalVar3;
      globalVar2 -= globalVar3;
      globalVar3 /= 2;
    }
    if (globalVar1 < 20 && globalVar2 > 5) {
      globalVar1 += 10;
    } else {
      globalVar1 -= 5;
    }
    break;
  }
  case 11: {
    for (int i = 0; i < 3; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp = 0;
      for (int i = 0; i < 2; ++i) {
        temp = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 1));
      }

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 12: {
    for (int i = 0; i < 1; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }

      // std::this_thread::sleep_for(std::chrono::milliseconds(8));
      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 13: {
    // Zusätzliche Rechenoperationen am Anfang (längere Ausführungsdauer)
    // Mutex erst später sperren
    for (int i = 0; i < 2; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }
      int temp = i * 2;

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      globalVar2 *= globalVar3;
      for (int i = 0; i < 1; ++i) {
        temp = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 1));
      }
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 14: {
    // Zusätzliche Rechenoperationen am Anfang (längere Ausführungsdauer)
    for (int i = 0; i < 3; ++i) {
      if (globalVar2 % 2 == 0) {
        globalVar2 -= 3;
      } else {
        globalVar2 += 4;
      }

      globalVar1 += globalVar2;
      globalVar3 = globalVar2 - globalVar1;
      int temp = i * 2;
      for (int i = 0; i < 4; ++i) {
        temp = ((i * 2) + (((globalVar2 * globalVar2) % 25) + 4));
      }
      globalVar2 *= globalVar3;
      globalVar1 -= globalVar3;
      globalVar3 += 10;
    }
    break;
  }
  case 15: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 2; i++) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      int count3 = 0, temp3 = 0;
      while (count3 < 23) {
        temp3 ^= globalVar2;
        temp3 |= globalVar1;
        ++count3;
      }

      int temp4 = temp3 % 2;

      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    break;
  }
  case 16: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)

    for (int i = 0; i < 2; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 12) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }

    int temp4 = temp3 % 2;
    
    break;
  }
  case 17: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 2; ++i) {
      
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 31) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }
    int temp4 = temp3 % 2;

    
    break;
  }
  case 18: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 2; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar1 *= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
      globalVar1 /= 3;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 6) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }

    int temp4 = temp3 % 2;

    
    break;
  }
  case 0: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
    for (int i = 0; i < 2; ++i) {
      if (globalVar1 < 5) {
        globalVar1 += 5;
      } else {
        globalVar1 -= 3;
      }
      globalVar2 += 3;
      globalVar3 -= 2;
      globalVar2 -= 4;
      globalVar3 += 6;
    }
    int count3 = 0, temp3 = 0;
    while (count3 < 1) {
      temp3 ^= globalVar2;
      temp3 |= globalVar1;
      ++count3;
    }
    
    break;
  }

  default:
    std::cerr << "Ungültiger Funktionindex!" << std::endl;
  }
}
// oder void und direkt globale variable auslesen? Hilft vielleicht für das
// inlinen, falls nur void geht
int execFunctionsGlobal(int k1, int k2, int k3, int k4) {
  // 10mal ausfüren

  std::map<int, int> counters;

  for (int i = 0; i < 15; i++) {
    globalVar1 = 0;
    globalVar2 = 0;
    globalVar3 = 0;

    std::thread t1(executeFunction, k1);
    int temp = globalVar1 +5;
    temp *= temp;
    temp %= 2;
    std::this_thread::sleep_for(std::chrono::nanoseconds(temp+5));
    std::thread t2(executeFunction, k2);
    temp = globalVar1 + 5;
    temp *= temp;
    temp %= 2;
    t1.join();
    std::this_thread::sleep_for(std::chrono::nanoseconds(temp+5));
    std::thread t3(executeFunction, k3);
    temp = globalVar1 + 5;
    temp *= temp;
    temp %= 2;
    t2.join();
    std::this_thread::sleep_for(std::chrono::nanoseconds(temp+5));
    std::thread t4(executeFunction, k4);

    t3.join();
    t4.join();
    if (globalVar1 < 0)
      globalVar1 = globalVar1 * (-1);
    counters[globalVar1 % 600] = counters[globalVar1 % 600] + 1;
  }
  std::map<int, int>::iterator best = std::max_element(
      counters.begin(), counters.end(),
      [](const std::pair<int, int> &a, const std::pair<int, int> &b) -> bool {
        return a.second < b.second;
      });
  return best->first;
}

int execFunctionsGlobalAntiDB(int k1, int k2, int k3, int k4, int rounds) {
  // 10mal ausfüren

  std::map<int, int> counters;

  for (int i = 0; i < rounds; i++) {
    globalVar1 = 0;
    globalVar2 = 0;
    globalVar3 = 0;

    std::thread t1(executeFunction, k1);
    std::thread t2(executeFunction, k2);
    std::thread t3(executeFunction, k3);
    std::thread t4(executeFunction, k4);

    // Auf Beendigung der Threads warten

    t1.join();    
    t2.join();
    t3.join();
    t4.join();
	if (globalVar1 < 0)
      globalVar1 = globalVar1 * (-1);
    counters[globalVar1 % 600] = counters[globalVar1 % 600] + 1;
  }
  std::map<int, int>::iterator best = std::max_element(
      counters.begin(), counters.end(),
      [](const std::pair<int, int> &a, const std::pair<int, int> &b) -> bool {
        return a.second < b.second;
      });
  return best->first;
}

char calculatedChar = 'a';

void calcCharValue(char src, char key) {
    calculatedChar = src + key;
}

void antiDB(){
	
	for(int u = 2; u < 3; u+=1){
		std::cerr << "u="<< u << ":" << std::endl;
		int count = 0;
		//for(int z = 0; z < 10; z++){ 
			count = 0;
			for(int o = 0; o < 5; o++){
				int firstElem = execFunctionsGlobalAntiDB(6,6,5,6, u);
		    		std::cerr << std::endl << "-.-" << std::endl;
		    		int sndElem = execFunctionsGlobalAntiDB(6,6,5,6, u);
		    		if(firstElem==sndElem)
		 	  			count++;
		 	  	std::cerr << sndElem << std::endl;
			}
		 	std::cerr << std::endl << count << std::endl;
		 	
		 	count = 0;
			for(int o = 0; o < 5; o++){
				int firstElem = execFunctionsGlobalAntiDB(6,6,7,7, u);
		    		std::cerr << std::endl << "-.-" << std::endl;
		    		int sndElem = execFunctionsGlobalAntiDB(6,6,7,7, u);
		    		if(firstElem==sndElem)
		 	  			count++;
		 	  	std::cerr << sndElem << std::endl;
			}
		 	std::cerr << std::endl << count << std::endl;
	 	//}
	}
	
 	
 	
 	exit(0);
}

char* decodeChar(char* src, char* key, int len){
	char* out = new char[len];
	for(int i = 0; i < len; i++){
        std::thread t1(calcCharValue, src[i], key[i]);
        t1.join();
        out[i] = calculatedChar;
	}
	return out;
}

