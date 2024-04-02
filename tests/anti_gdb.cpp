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
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 2: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 3: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 4: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 5: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 6: {
  mutex1.lock();
    for (int i = 0; i < 3; ++i) {
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
    mutex1.unlock();
    break;
  }
  case 7: {
  mutex1.lock();
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
    mutex1.unlock();
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
    mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 9: { // Mutex erst später sperren
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 10: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 11: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 12: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 13: {
    // Zusätzliche Rechenoperationen am Anfang (längere Ausführungsdauer)
    // Mutex erst später sperren
    mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 14: {
  mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 15: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
mutex1.lock();
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
    mutex1.unlock();
    break;
  }
  case 16: {
    // Zusätzliche Rechenoperationen am Anfang (noch längere Ausführungsdauer)
mutex1.lock();

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
    mutex1.unlock();
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
mutex1.lock();
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
    mutex1.unlock();
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
mutex1.lock();
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
    mutex1.unlock();
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
    mutex1.lock();
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
mutex1.unlock();
    
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


void antiDB(){
	int firstElem = execFunctionsGlobalAntiDB(3,8,1,4, 5);
	for(int u = 5; u < 50; u+=5){
	std::cerr << "u="<< u << ":" << std::endl;
	int count = 0;
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
 	std::cerr << count << std::endl;
 	
 	count = 0;
 	int firstElem = execFunctionsGlobalAntiDB(3,8,1,4, 5);
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	for(int cnt0 = 0; cnt0 < 10; cnt0++){
 	  		if(firstElem==execFunctionsGlobalAntiDB(3,8,1,4, 5))
 	  			count++;
	}
	std::cerr << count << std::endl;
 	std::cerr << "-----" << std::endl;
	}
}

