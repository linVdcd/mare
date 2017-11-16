import codecs
f = codecs.open('dict.utf8',mode='r',encoding='utf-8')
fo = codecs.open('jieba.dict.utf8',mode='w',encoding='utf-8')
for line in f.readlines():
    tl = line
    line = line.split(' ')
    if len(line[0])>1:
        fo.write(tl)
f.close()
fo.close()