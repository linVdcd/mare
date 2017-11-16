#include "../include/JiebaSegment.h"
#include "../3rdparty/cppjieba/Jieba.hpp"

JiebaSegment *JiebaSegment::_instance = NULL;

JiebaSegment *JiebaSegment::Instance() {
    if (0 == _instance) {
        _instance = new JiebaSegment();
    }
    return _instance;
}

void JiebaSegment::LoadResource(std::string dir) {
    if (_app != NULL) {
        return;
    }

    cppjieba::Jieba *app = new cppjieba::Jieba(dir + "/jieba.dict.utf8",
                                               dir + "/hmm_model.utf8",
                                               dir + "/user.dict.utf8");
    _app = (cppjieba::Jieba *) app;
}

void JiebaSegment::LoadResource(const std::string &jieba_buffer,
                                const std::string &hmm_model_buffer,
                                const std::string &user_buffer) {
    if (_app != NULL) {
        std::cout << "jieba had loaded resource" << std::endl;
        return;
    }
    cppjieba::Jieba *app = new cppjieba::Jieba(jieba_buffer,
                                               hmm_model_buffer,
                                               user_buffer,
                                               1);
    _app = (cppjieba::Jieba *) app;
}

bool JiebaSegment::hadLoadedResource() {
    if (_app) {
        return true;
    } else {
        return false;
    }
}

void JiebaSegment::ReleaseJiebaResource() {
    if (_app != NULL) {
        delete (cppjieba::Jieba*)_app;
        _app = NULL;
        std::cout << "release jieba resource" << std::endl;
    } else {
        XLOG(ERROR) << "double free jieba resource, please check it";
    }
}

void JiebaSegment::Cut(const std::string &sentence, std::vector<std::string> &words) {
    if (_app == NULL) {
        XLOG(ERROR) << "jieba have not been initialize";
        return;
    }
    cppjieba::Jieba *app = (cppjieba::Jieba *) _app;
    app->Cut(sentence, words);
}

void JiebaSegment::Tag(const std::string &sentence, std::vector<std::pair<std::string, std::string> > &wordTags) {
    if (_app == NULL) {
        XLOG(ERROR) << "jieba have not been initialize";
        return;
    }
    cppjieba::Jieba *app = (cppjieba::Jieba *) _app;
    app->Tag(sentence, wordTags);
}

//void JiebaSegment::Extract(const std::string &sentence, std::vector<std::string> &words,int topN)
//{
//    if(_app==NULL)
//    {
//        return;
//    }
//    cppjieba::Jieba* app=(cppjieba::Jieba*)_app;
//    app->extract(sentence, words, topN);
//}

void JiebaSegment::InsertNewWords(std::vector<std::string> &words) {
    if (_app == NULL) {
        XLOG(ERROR) << "jieba have not been initialize";
        return;
    }
    cppjieba::Jieba *app = (cppjieba::Jieba *) _app;
    for (int i = 0; i < words.size(); i++) {
        app->InsertUserWord(words[i]);
    }
}


std::string JiebaSegment::filterContinueWord(const std::string& origStr) {
    cppjieba::RuneStrArray sentence;
    cppjieba::RuneStrArray filterSentence;
    cppjieba::DecodeRunesInString(origStr, sentence);
    cppjieba::RuneStrArray::const_iterator iter;
    int index = 0;
    int repeat = 0;
	int prevCode = -1;
    int maxRepeatNum = 3;
    for (iter = sentence.begin(); iter != sentence.end(); iter++) {
		int currentCode = iter->rune;
        cppjieba::RuneStr currentRuneStr = (*iter);
		if (currentCode != prevCode) {
            prevCode = currentCode;
            repeat = 1;
            filterSentence.push_back(currentRuneStr);
        } else {
            repeat += 1;
            if (repeat <= maxRepeatNum) {
                filterSentence.push_back(currentRuneStr);
            }
        }
        index++;
    }
	std::string filterStr = "";
    for (iter = filterSentence.begin(); iter != filterSentence.end(); iter++) {
		std::string temp = cppjieba::GetStringFromRunes(origStr, iter, iter);
		filterStr = filterStr + temp;
    }
    std::cout << "filter result: " << filterStr << std::endl;
    return filterStr;
}
