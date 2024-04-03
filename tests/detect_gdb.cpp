#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
#include <map>
#include <utility>
#include <vector>

int globalVar1 = 0;
int globalVar2 = 0;
int globalVar3 = 0;

// executeFunction Methode
void executeFunction(int functionIndex) {
  // Temporäre Variablen für zusätzliche Rechenoperationen
  int temp1 = 0, temp2 = 0, temp3 = 0;

  // Dispatcher für die Funktionen basierend auf dem übergebenen Index
  switch (functionIndex) {
  case 0: {
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
  case 1: {
    //make sure race condition is triggered independent of hardware resources
    for (int i = 0; i < 500; ++i) {
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

  default:
    std::cerr << "Ungültiger Funktionindex!" << std::endl;
  }
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

void detectDB(){
	int firstElem = execFunctionsGlobalAntiDB(0,1,1,0, u);
	for(int u = 5; u < 50; u+=5){
		std::cerr << "u="<< u << ":" << std::endl;
		int count = 0;
		for(int cnt0 = 0; cnt0 < 10; cnt0++){
	 	  	if(firstElem==execFunctionsGlobalAntiDB(0,1,1,0, u))
	 	  		count++;
		}
	 	std::cerr << "matches:" << count << "/10" << std::endl;
	}
}

