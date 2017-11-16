#include <iostream>
#include "Mare.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "JiebaSegment.h"
#include <algorithm>
using namespace std;



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


    string test("Yes你好");
    transform(test.begin(),test.end(),test.begin(),::tolower);
    cout<<test;

    std::string marPath = "/home/lin/HS-works/gitlab/MAR/";
    //std::string marPath = "/home/sooda/awe/MetalActionsRecommend/";
    std::string dataBasePath = marPath + "cws/data/";
    std::string actionDataBasePath = marPath + "data/";
    std::string jiebaDictPath = dataBasePath + "jieba.dict.utf8";
    std::string hmmModelPath = dataBasePath + "hmm_model.utf8";

    std::string dictBuffer = getFileString(jiebaDictPath);
    std::string hmmBuffer = getFileString(hmmModelPath);

    std::string idf = getFileString(actionDataBasePath + "IDF.txt");
    std::string kidf = getFileString(actionDataBasePath + "keyword.idf");
    std::string actions = getFileString(actionDataBasePath + "actions.txt");
    std::string ew = getFileString(actionDataBasePath + "synonym.expand");

    vector<string>result;
    Mare mare;
    mare.init(dictBuffer,hmmBuffer,idf,kidf,actions,ew);
    dictBuffer.clear();hmmBuffer.clear();idf.clear();kidf.clear();actions.clear();ew.clear();
    mare.recommend("YesSir好", result);
    for(vector<string>::iterator iter=result.begin();iter!= result.end();iter++)
        cout<<*iter<<endl;
    result.clear();


    return 0;
}
