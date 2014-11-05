#ifndef		_LANDMARKS_HH_
# define	_LANDMARKS_HH_

#include <vector>
#include <map>

static const double CONVERSION = (Math.PI / 180.0); // Convert to radians
static const unsigned int MAXLANDMARKS = 3000; // Max number of landmarks
static const double MAXERROR = 0.5; // If a landmarks is within this distance of another landmarks, its the same landmarks
static const unsigned int MINOBSERVATIONS = 15; // Number of times a landmark must be observed to be recongnized as a landmark
static const unsigned int LIFE = 40; // Use to reset life counter (counter use to determine whether to discard a landmark or not)
static const float MAX_RANGE = 1.0;
static const unsigned int MAXTRIALS = 1000; // RANSAC: max times to run algorithm
static const unsigned int MAXSAMPLE = 10; // RANSAC: randomly select x points
static const unsigned int MINLINEPOINTS = 30; // RANSAC: if less than x points left, don't bother trying to find a consensus (stop algorithm)
static const double RANSAC_TOLERANCE = 0.05; // RANSAC: if point is within x distance of line, its part of the line
static const unsigned int RANSAC_CONSENSUS = 30; // RANSAC: at leat x votes required to determine if its a line
static const double DEGRESSPERSCAN = 0.5;

class Landmarks
{
public:
  class  Landmark
  {
  public:
    double pos[2]; // landmarks (x, y) position relative to map
    int id; // lanndmarks unique ID
    int life; // a life counter to determine whether to discard a landmarl
    int totalTimeObserved; // the number of times we have seen the landmark
    double range; // last observed range to landmark
    double bearing; // last observed bearing to landmark

    // RANSAC : Store equation of a line to be reused
    double a;
    double b;
    double rangeError; // distance from robot position to the wall we are using as a landmark (to calculate error)
    double bearingError; // bearing from robot position to the wall we are using as a landmark (to calculate error)

  public:
    Landmark();
    ~Landmark();
  };


public:
  Landmarks();
  ~Landmarks();
  Landmarks(double degreePerScan);

  int getSLamId(int id) const;
  int addSlamId(int landmarkId, int slamId);
  int removeBadLandmarks(double laserdata[], double robotPosition[]); // Possibly change array to vector ? Depends of the robot
  int removeBadLandmarks(const std::vector<double> & laserdata, const std::vector<double> & robotPosition); // both to be sure

  std::vector<Landmark *> updateAndAddLineLandmarks(std::vector<Landmark *> extractedLandmarks); // bad return value
  std::vector<Landmark *> updateAndAddLandmarkUsingEKFResults(bool matched[], int id[], double ranges[], double bearings[], double robotPosition[]);
  int updateLineLandmark(const Landmark &lm);
  std::vector<Landmark *> extractLineLandmarks(double laserdata[], double robotPosition[]);
  int alignLandmarkData(std::vector<Landmark *> &extractedLandmarks, bool &matched[], int &id[],
			double &ranges[], double &bearings[], std::vector<std::vector<double> > &lmrks, std::vector<std::vector<double> > &exlmrks);
  int addToDB(const Landmark &lm);

  int getDBSize() const;
  std::vector<Landmark *> getLandmarkDB() const;

private:
  Landmark &updateLandmark(bool matched, int id, double distance, double readingNo, double robotPosition[]); // perhaps return a pointer of LM is better
  void udpdateLandmark(Landmark &lm); // no need to return the landmark gives in parameter, just modify it

  void leastSquaresLineEstimate(double laserdata[], double robotPosition[], int selectPoints[], int arraySize, double &a, double &b);
  double distanceToLine(double x, double y, double a, double b);
  std::vector<Landmark *> extractSpikeLandmarks(double laserdata[], double robotPosition[]);
  Landmark &getLandmark(double range, int readingNo, double robotPosition[]);
  Landmark &getLineLandmark(double a, double b, double robotPosition[]);
  Landmark &getLine(double a, double b);
  Landmark &getOrigin();
  void getClosestAssociation(Landmark *lm, int &id, int &totalTimeObserved);
  int getAssociation(Landmark &lm);
  std::vector<Landmark *> removeDouble(std::vector<Landmark *> extractedLandmarks);

  double distance(double x1, double y1, double x2, double y2) const;
  double distance(const Landmark &lm1, const Landmark &lm2) const;


private:
  std::vector<Landmark *> landmarkDB;
  int DBSize;
  std::vector<std::pair<int, int> > IDtoID;
  int EKFLandmarks;
};

#endif
