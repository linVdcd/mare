//
// Created by lin on 17-10-17.
//

#ifndef MAR_MARE_H
#define MAR_MARE_H

#include <vector>
#include <string>
#include <map>
//Mare->Metal Actions Recommend

class Mare {
    std::map<std::string,float> IDF;
    struct MetalW{
        MetalW(const int id,const float w):docID(id),weight(w){}
        int docID;
        float weight;
    };
    struct MetalScore{
        MetalScore(const float s):score(s){}
        float score;
    };
    struct index_cmp {
        index_cmp(std::vector<struct MetalScore> arr) : arr(arr) {}
        bool operator()(const size_t a, const size_t b) const
        { return arr[a].score > arr[b].score; }
        const std::vector<struct MetalScore> arr;
    };
    std::map<std::string ,std::vector<struct MetalW>> invertIndex;
    std::map<std::string,std::map<std::string,float>> ExWords;
    std::vector<struct MetalScore> resultScore;
    std::vector<std::string> MetaActionsName;

public:

    void init(const std::string &dictBuffer, const std::string &hmmBuffer, const std::string &idfsBuffer, const std::string &keyWordIdfsBuffer,
              const std::string &actionsBuffer,const std::string &expandWordsBuffer);
    void recommend(const std::string &sentence,std::vector<std::string> &result);
    std::string preFilter(const std::string& origSentence);
    ~Mare();
};


#endif //MAR_MARE_H
