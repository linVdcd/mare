#ifndef WORDS_PROCESS_H
#define WORDS_PROCESS_H

#include "stdio.h"
#include <vector>
#include <string>
#include <utility>

class JiebaSegment {
public:
    static JiebaSegment *Instance();

    void LoadResource(std::string dir);
    bool hadLoadedResource();

    void LoadResource(const std::string &jieba_buffer,
                      const std::string &hmm_model_buffer,
                      const std::string &user_buffer);
    void ReleaseJiebaResource();

    void Cut(const std::string &sentence, std::vector<std::string> &words);

    void Tag(const std::string &sentence, std::vector<std::pair<std::string, std::string> > &wordTags);

    void Extract(const std::string &sentence, std::vector<std::string> &words, int topN = 1);

    void InsertNewWords(std::vector<std::string> &words);
    std::string filterContinueWord(const std::string& origSentence);

protected:
    JiebaSegment(void) {
        _app = NULL;
    }

    virtual ~JiebaSegment(void) {
    }

    static JiebaSegment *_instance;
    void *_app;
};

#endif
