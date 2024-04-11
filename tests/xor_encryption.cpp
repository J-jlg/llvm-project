char calculatedChar = 'a';

void calcCharValue(char src, char key) {
    calculatedChar = src ^ key;
}

char* decodeChar(char* src, char* key, int len){
	char* out = new char[len];
	for(int i = 0; i < len; i++){
	        //TOOD: Integrate in ThreadObfs
        	std::thread t1(calcCharValue, src[i], key[i]);
        	t1.join();
        	out[i] = calculatedChar;
	}
	return out;
}
