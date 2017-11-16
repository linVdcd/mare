#include <iostream>
#include <fstream>
#include "../include/JiebaSegment.h"

std::string getFileString(const std::string& filepath) {
	std::ifstream is(filepath.c_str());
	std::string filebuffer="";
	if (is.is_open()) {
		// get length of file:
		is.seekg (0, is.end);
		long long length = is.tellg();
		is.seekg (0, is.beg);
		char * buffer = new char [length];
		std::cout << "Reading " << filepath << " " << length << " characters... ";
		// read data as a block:
		is.read (buffer,length);
		if (is)
			std::cout << "all characters read successfully." << std::endl;
		else
			std::cout << "error: only " << is.gcount() << " could be read";
		is.close();
		// ...buffer contains the entire file...
		filebuffer = std::string(buffer,length);
		delete[] buffer;
	} else {
		std::cout << filepath << "open faild in getFileString" << std::endl;
	}
	return filebuffer;
}

int main() {
    std::string dataBasePath = "data/";
    std::string jiebaDictPath = dataBasePath + "jieba.dict.utf8";
    std::string hmmModelPath = dataBasePath + "hmm_model.utf8";
    std::string userDictPath = dataBasePath + "user.dict.utf8";
    std::string dictBuffer = getFileString(jiebaDictPath);
    std::string hmmBuffer = getFileString(hmmModelPath);
    std::string userBuffer = getFileString(userDictPath);
    JiebaSegment::Instance()->LoadResource(dictBuffer, hmmBuffer,userBuffer);

    std::vector<std::string> words;
    JiebaSegment::Instance()->Cut("欢迎使用分词服务!", words);
    for (int i = 0; i < words.size(); i++) {
        std::cout << words[i] << std::endl;
    }

    JiebaSegment::Instance()->ReleaseJiebaResource();
    return 0;
}
