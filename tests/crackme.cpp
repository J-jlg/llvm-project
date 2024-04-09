#include <stdio.h>
#include <string.h>
 
int main ( int argc, char *argv[] )
{
    char stringArray[23] ;
	
	char username[20];
	
	char* pleasEN = new char[30];
    strncpy(pleasEN, "Please enter your holy name: ", 30);
	pleasEN[0] = 'P';
	printf("%s", pleasEN ) ;
	if (fgets(username, 20, stdin) == NULL)
    {
        puts("error");
        return -1;
    }
     
    printf( "Name the creator of the Java Bible: " ) ;
    if (fgets(stringArray, 23, stdin) == NULL)
    {
        puts("error");
        return -1;
    }

	bool isRightPassword = true;
	if(stringArray[0] != 'g'){
		isRightPassword = false;
	}
	if(stringArray[1] != 'r'){
		isRightPassword = false;
	}
	if(stringArray[2] != 'a'){
		isRightPassword = false;
	}
	if(stringArray[3] != 'd'){
		isRightPassword = false;
	}
	if(stringArray[4] != 'l'){
		isRightPassword = false;
	}
	if(stringArray[5] != 'e'){
		isRightPassword = false;
	}
	if(stringArray[6] != '.'){
		isRightPassword = false;
	}
	if(stringArray[7] != 'g'){
		isRightPassword = false;
	}
	if(stringArray[8] != 'u'){
		isRightPassword = false;
	}
	if(stringArray[9] != 'r'){
		isRightPassword = false;
	}
	if(stringArray[10] != 'u'){
		isRightPassword = false;
	}
	if(stringArray[11] != '@'){
		isRightPassword = false;
	}
	if(stringArray[12] != 'j'){
		isRightPassword = false;
	}
	if(stringArray[13] != 'a'){
		isRightPassword = false;
	}
	if(stringArray[14] != 'v'){
		isRightPassword = false;
	}
	if(stringArray[15] != 'a'){
		isRightPassword = false;
	}
	if(stringArray[16] != '.'){
		isRightPassword = false;
	}
	if(stringArray[17] != 'b'){
		isRightPassword = false;
	}
	if(stringArray[18] != 'i'){
		isRightPassword = false;
	}
	if(stringArray[19] != 'b'){
		isRightPassword = false;
	}
	if(stringArray[20] != 'l'){
		isRightPassword = false;
	}
	if(stringArray[21] != 'e'){
		isRightPassword = false;
	}
	if(!isRightPassword){
			puts("wrong :(!");
			char *s = new char[strlen("try again ")+strlen(username)+1];
			strcpy(s,"try again ");
			strcat(s,username);
			puts(s);
			return -1;
	}
		
	puts("success JAVA is great1!1!11!");
    return 0; 
}
