//
// Created by lin on 17-10-17.
//

#include "Mare.h"
#include "JiebaSegment.h"
#include <algorithm>
#include <set>
#include <iostream>

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


void readIDF(const std::string &buffer,std::map<std::string,float> &idfs){
    std::vector<std::string> lines;

    SplitString(buffer,lines,"\n");

    for(const auto &iter:lines)
    {
        std::string line(iter);
        if (line.size()==0)
            continue;
        std::transform(line.begin(),line.end(),line.begin(),::tolower);
        std::vector<std::string> split;
        SplitString(line,split,"\t");
        if (split.size()==1) continue;
        idfs.insert(std::pair<std::string,float>(split[0],atof(split[1].c_str())));
    }
}


void readActions(const std::string &buffer,std::map<std::string,std::vector<std::string>> &actions){
    std::vector<std::string> lines;

    SplitString(buffer,lines,"\n");

    for(const auto &iter:lines)
    {
        std::string line(iter);
        if (line.size()==0)
            continue;
        std::transform(line.begin(),line.end(),line.begin(),::tolower);
        std::vector<std::string> split;
        SplitString(line,split,"\t");
        if (split.size()==1)continue;
        std::vector<std::string> words;
        SplitString(split[1],words,",");
        actions.insert(std::pair<std::string,std::vector<std::string>>(split[0],words));
    }
}

void readExpandKeyword(const std::string &buffer,std::map<std::string,std::map<std::string,float>> &expandWords){
    std::vector<std::string> lines;

    SplitString(buffer,lines,"\n");
    std::vector<std::string> ww;

    for(const auto& iter:lines)
    {
        std::string line(iter);
        if (line.size()==0)
            continue;
        std::transform(line.begin(),line.end(),line.begin(),::tolower);
        std::vector<std::string> split;
        SplitString(line,split,"\t");
        if(split.size()==1)
        {
            std::map<std::string,float> temp;
            temp.clear();
            expandWords.insert(std::pair<std::string,std::map<std::string,float> >( split[0],temp));
            continue;
        }

        std::vector<std::string> words;
        SplitString(split[1],words,",");

        for(const auto &witer:words)
        {

            std::string word = witer;
            if(word.size()==0)continue;


            SplitString(word,ww,":");
            if(ww[0]==split[0])
                continue;
            if(expandWords.find(split[0])!=expandWords.end()){
                expandWords[split[0]].insert(std::pair<std::string,float>(ww[0],atof(ww[1].c_str())));
            } else
            {std::map<std::string,float> temp;
            temp.insert(std::pair<std::string,float>(ww[0],atof(ww[1].c_str())));
                expandWords.insert(std::pair<std::string,std::map<std::string,float> >( split[0],temp));}
            ww.clear();

        }
    }
}





void Mare::init(const std::string &dictBuffer, const std::string &hmmBuffer, const std::string &idfsBuffer,const  std::string &keyWordIdfsBuffer,
                const std::string &actionsBuffer,const std::string &expandWordsBuffer) {

    std::map<std::string,float> keyWordIdfs;
    std::map<std::string,std::vector<std::string>> actions;
    readIDF(idfsBuffer,IDF);
    readIDF(keyWordIdfsBuffer,keyWordIdfs);
    readActions(actionsBuffer,actions);
    readExpandKeyword(expandWordsBuffer,ExWords);
    std::set<std::string> ds;
    std::string userDict;

    for(const auto &iter:ExWords)
    {
        ds.insert(iter.first);
        std::map<std::string,float> temp = iter.second;
        for(auto iterV =temp.begin();iterV!=temp.end();iterV++)
            ds.insert(iterV->first);
    }
    for(const auto &iter:keyWordIdfs)
    {
        ds.insert(iter.first);
    }



    for(const auto &iter:ds)
    {


        userDict+=iter +" 3 nz\n";
    }

    JiebaSegment::Instance()->LoadResource(dictBuffer, hmmBuffer,userDict);//init jieba

    //测试自定义词典是否起作用
    /*std::vector<std::string> senteceSeg;
    for(const auto&iter:ds){
        senteceSeg.clear();
        JiebaSegment::Instance()->Cut(iter, senteceSeg);
        if(senteceSeg.size()>1)
            std::cout<<iter<<std::endl;
    }*/

    int index=0;

    for(const auto &iter:actions)
    {
        std::string name = iter.first;
        if (name.size()==0)
            continue;
        MetaActionsName.push_back(name);
        std::vector<std::string> keywors=iter.second;


        for(const auto &witer:keywors)
        {
            std::string w = witer;
            if(keyWordIdfs.find(w)!=keyWordIdfs.end())
                invertIndex[w].push_back(MetalW(index,keyWordIdfs[w]));
            else
                invertIndex[w].push_back(MetalW(index,1.0));
        }
        index++;
    }
}


std::string Mare::preFilter(const std::string& origSentence) {
    std::string filterSentence = JiebaSegment::Instance()->filterContinueWord(origSentence);
    //std::cout << origSentence << " " << filterSentence << std::endl;
    return filterSentence;
}






void Mare::recommend(const std::string &sentence,std::vector<std::string> &result) {
    std::string filterSentence = preFilter(sentence);
    std::transform(filterSentence.begin(),filterSentence.end(),filterSentence.begin(),::tolower);
    std::vector<std::string> senteceSeg;
    JiebaSegment::Instance()->Cut(filterSentence, senteceSeg);
    for(const auto &iter:senteceSeg)
        std::cout<<iter<<" ";
    std::cout<<std::endl;
    resultScore.clear();
    for(const auto &iter:MetaActionsName)
        resultScore.push_back(MetalScore(0.0));

    for(const auto &iter:senteceSeg)
    {
        std::string k = iter;
        float idf_k = 1.0;
        if(IDF.find(k)!=IDF.end()) idf_k=IDF[k];
        if (invertIndex.find(k)!=invertIndex.end()){
            for(const auto &metawIter:invertIndex[k])
            {
                int docid = metawIter.docID;
                float weight = metawIter.weight;
                resultScore[docid].score += weight*idf_k;
            }
        } else{
            for(const auto &iiiter:invertIndex)
            {
                std::string word=iiiter.first;
                if (word.size()==0||ExWords.find(word)==ExWords.end()) continue;
                std::vector<struct MetalW> id_weight = iiiter.second;
                for(const auto &metawIter:id_weight)
                {
                    int docid = metawIter.docID;
                    float weight = metawIter.weight;
                    std::map<std::string,float> expand_words=ExWords[word];
                    if(expand_words.find(k)!=expand_words.end())
                        resultScore[docid].score += weight*expand_words[k]*idf_k;
                }
            }
        }
    }

    std::vector< int>  idx(resultScore.size());
    for (int i = 0; i != idx.size(); ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), index_cmp(resultScore));
    for(int i=0;i<idx.size();i++){
        if(resultScore[idx[i]].score>0.0001)
            result.push_back(MetaActionsName[idx[i]]);
    }

}

Mare::~Mare() {
    if(JiebaSegment::Instance()!=NULL)
        JiebaSegment::Instance()->ReleaseJiebaResource();
}
