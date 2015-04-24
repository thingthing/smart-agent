#include "network/ChunkFactory.h"

namespace Network
{

/*
 * Class used :
 * "pcl::PointCloud::PointXYZ"
 *      float x
 *      float y
 *      float z // not avaible in "pcl::PointCloud::PointXY"
 *
 * "Landmarks::Landmark"
 *  pcl::PointCloud::PointXY pos;
 *  pcl::PointCloud::PointXYZ robotPos;
 *  int id;
 *  int life;
 *  int totalTimeObserved;
 *  double bearing;
 *  double range;
 */

// PUBLIC

ChunkFactory::ChunkFactory():   _fullChunkReadiness(false),
                                _chunkReadiness(false),
                                _sizeChunks(0),
                                _maxSizeChunk(512) {}

// TODO clean the deque if needed?
ChunkFactory::~ChunkFactory() {}

void ChunkFactory::processData(const std::vector< Landmarks::Landmark >& landmarks_)
{
    std::vector<Landmarks::Landmark>::const_iterator it = landmarks_.begin();
    for ( ; it != landmarks_.end(); ++it)
        processData(*it);
}

void ChunkFactory::processData(const Landmarks::Landmark& landmark_)
{
    addEncodedClassToChunk("L(" + fromLandmarkToString(landmark_) + ")");
}

// ATTENTION must convert PointCloud in all PointXYZ first
void ChunkFactory::processData(const pcl::PointCloud< pcl::PointXYZ >& points)
{
    // TODO do a loop on each point of the cloud?
    // addEncodedClassToChunk("P(" + fromPclPointToString(points) + ")");
}

// Getters
bool ChunkFactory::isFullChunkReady() const { return _fullChunkReadiness; }
bool ChunkFactory::isChunkReady() const     { return _chunkReadiness; }

std::string ChunkFactory::getChunk()
{
    std::string chunk = "";

    if (_chunks.size() != 0)
    {
        chunk = _chunks.back();
        _chunks.pop_back();
        decreaseSizeChunks(chunk.size());
    }
    return chunk;
}

// PRIVATE

void ChunkFactory::pushChunkToChunks()
{
    unsigned int chunkSize = _tmpChunk.size();

    if (chunkSize > 0)
    {
        _chunks.push_front(_tmpChunk);
        increaseSizeChunks(chunkSize);
        _tmpChunk.erase();
        _chunkReadiness = false;
    }
}

void ChunkFactory::addEncodedClassToChunk(const std::string& encodedClass)
{
    if (_tmpChunk.size() + encodedClass.size() > _maxSizeChunk)
        pushChunkToChunks();

    _tmpChunk += encodedClass;
    if (!_chunkReadiness)
        _chunkReadiness = true;
}

// Return a string like "cccciiiioooooooo"
std::string ChunkFactory::fromLandmarkToString(const Landmarks::Landmark& landmark_)
{
    std::string strLandmark = "";

/*
 * ATTENTION "pos" and "robotPos" are cloud of points!
 * pcl::PointCloud::PointXY pos;
 * pcl::PointCloud::PointXYZ robotPos;
*/
    strLandmark += fromPclPointToString(landmark_.pos);
    strLandmark += fromPclPointToString(landmark_.robotPos);
    strLandmark += encodeNbIntoString((void*)&(landmark_.id), sizeof(landmark_.id));
    strLandmark += encodeNbIntoString((void*)&(landmark_.life), sizeof(landmark_.life));
    strLandmark += encodeNbIntoString((void*)&(landmark_.totalTimeObserved), sizeof(landmark_.totalTimeObserved));
    strLandmark += encodeNbIntoString((void*)&(landmark_.bearing), sizeof(landmark_.bearing));
    strLandmark += encodeNbIntoString((void*)&(landmark_.range), sizeof(landmark_.range));

    return strLandmark;
}

std::string ChunkFactory::fromPclPointCloudToString(const pcl::PointCloud< pcl::PointXYZ >& pointCloud)
{
    // TODO
    return "";
}

// Return a string like "cccciiii" FOR PointXY
std::string ChunkFactory::fromPclPointToString(const pcl::PointXY& points)
{
    std::string stringPoints = "";

    stringPoints += encodeNbIntoString((void*)&(points.x), sizeof(points.x));
    stringPoints += encodeNbIntoString((void*)&(points.y), sizeof(points.y));

    return stringPoints;
}

// Return a string like "cccciiiioooo" FOR PointXYZ
std::string ChunkFactory::fromPclPointToString(const pcl::PointXYZ& points)
{
    std::string stringPoints = "";

    stringPoints += encodeNbIntoString((void*)&(points.x), sizeof(points.x));
    stringPoints += encodeNbIntoString((void*)&(points.y), sizeof(points.y));
    stringPoints += encodeNbIntoString((void*)&(points.z), sizeof(points.z));

    return stringPoints;
}

std::string ChunkFactory::encodeNbIntoString(void* nb, unsigned long nbOfByte)
{
  char* nb_ = (char*)nb;
  std::string encoded = "";

  for (unsigned long i = 0; i < nbOfByte; ++i)
    {
      char tmp = 0;
      tmp |= nb_[i];
      encoded += tmp;
    }
  //  std::cout << std::hex << encoded;
  return encoded;
}

void ChunkFactory::increaseSizeChunks(unsigned int cSize) { _sizeChunks += cSize; }
void ChunkFactory::decreaseSizeChunks(unsigned int cSize) { _sizeChunks -= cSize; }

} // end of namespace
