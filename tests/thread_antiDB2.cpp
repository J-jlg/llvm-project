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

void executeFunctionAntiDB(int functionIndex) {

int temp1 = 0, temp2 = 0, temp3 = 0;

  switch (functionIndex) {
    case 0: {
    int count3 = 0, temp5 = 0;
    while (count3 < 12) {
      temp5 ^= globalVar2;
      temp5 |= globalVar1;
      ++count3;
    }

    int temp4 = temp5 % 2;

    for (int i = 0; i < 200; ++i) {
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
  case 1: {
    for (int i = 0; i < 100; ++i) {
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
  case 2: {
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
  case 3: {
    for (int i = 0; i < 400; ++i) {
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
  default:
	for (int i = 0; i < 30; ++i) {
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
 }

int execFunctionsGlobalAntiDB(int k1, int k2, int k3, int k4) {
  // 10mal ausfüren

  std::map<int, int> counters;

  for (int i = 0; i < 4; i++) {
    globalVar1 = 0;
    globalVar2 = 0;
    globalVar3 = 0;

    std::thread t1(executeFunctionAntiDB, k1);
    std::thread t2(executeFunctionAntiDB, k2);
    std::thread t3(executeFunctionAntiDB, k3);
    std::thread t4(executeFunctionAntiDB, k4);

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

void antiDB(){
	int value = 0;
	for(int o = 0; o < 150; o++){
		int firstElem = execFunctionsGlobalAntiDB((0+o)%2,(1+o)%2,(0+o)%2,(2+o)%2);
		int sndElem = execFunctionsGlobalAntiDB((0+o)%2,(1+o)%2,(0+o)%2,(2+o)%2);
		if(firstElem!=sndElem){
			value = firstElem;
			break;
		}
	}
	
	switch(value){
		case 0: {
		   //debugger code
		   //TODO: Junk Code! + hide exit(0);
		   exit(0);
		   break;
		}
		case 455: {
		   //normal execution
		}
		case 301: {
		   //PoC
		}
		case 103: {
		//PoC
		}
		case 243: {
		//PoC
		}
		case 139: {
		//PoC
		}
		case 539: {
		//PoC
		}
		case 39: {
		//PoC
		}
		case 395: {
		//PoC
		}
		case 373: {
		//PoC
		}
		case 120: {
		//PoC
		}
		case -12: {
		   //bogus Code
		}
		default:
		   break;
	}
}

