#ifndef ZFILEDIFF
#define ZFILEDIFF

#include <QString>
#include <QFile>
#include "zfilediffmodel.h"

class ZFileDiff {

public:
    ZFileDiff();
    ZFileDiff(const QString &fileSrc, const QString &fileDst);
    ~ZFileDiff();

    void setFileSrc(const QString &fileSrc);
    void setFileDst(const QString &fileDst);

    QList<ZFileDiffModel> execute();

private:
    bool hasDifference();
    /*bool initRect();
    bool makeRect();
    void recallRect();*/

private:
    QFile *mFileSrc;
    QFile *mFileDst;

    int **mDiffRect;
    int mDiffRectRows;
    int mDiffRectCols;

    std::vector<unsigned int> mHashSrcLst;
    std::vector<unsigned int> mHashDstLst;
    QList<QString> mLineSrcLst;
    QList<QString> mLineDstLst;
    QList<ZFileDiffModel> mModelLst;
};

#endif // ZFILEDIFF

