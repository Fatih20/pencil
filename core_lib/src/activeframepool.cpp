#include "activeframepool.h"
#include "keyframe.h"
#include <QDebug>


ActiveFramePool::ActiveFramePool()
{
}

void ActiveFramePool::put(KeyFrame* key)
{
    if (key == nullptr)
        return;

    Q_ASSERT(key->pos() > 0);

    auto it = mCacheFramesMap.find(key);
    mCacheFramesList.push_front(key);
    if (it != mCacheFramesMap.end())
    {
        mCacheFramesList.erase(it->second);
        mCacheFramesMap.erase(it);
    }
    mCacheFramesMap[key] = mCacheFramesList.begin();

    if (mCacheFramesMap.size() > mMaxSize)
    {
        list_iterator_t last = mCacheFramesList.end();
        last--;
        
        KeyFrame* lastKeyFrame = *last;
        unloadFrame(lastKeyFrame);

        mCacheFramesMap.erase(lastKeyFrame);
        mCacheFramesList.pop_back();
    }
}

size_t ActiveFramePool::size() const
{
    return mCacheFramesMap.size();
}

void ActiveFramePool::clear()
{
    mCacheFramesList.clear();
    mCacheFramesMap.clear();
}

void ActiveFramePool::onKeyFrameDestroy(KeyFrame* key)
{
    auto it = mCacheFramesMap.find(key);
    if (it != mCacheFramesMap.end())
    {
        mCacheFramesList.erase(it->second);
        mCacheFramesMap.erase(it);
    }
}

void ActiveFramePool::unloadFrame(KeyFrame* key)
{
    qDebug() << "Unload :" << key->pos() << key->fileName();
    key->unload();
}
