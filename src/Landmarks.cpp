#include "Landmarks.hh"

Landmarks::Landmark::Landmark()
{
  this->pos[0] = 0.0;
  this->pos[1] = 0.0;
  this->id = -1;
  this->life = LIFE;
  this->totalTimeObserved = 0;
  this->range = -1;
  this->bearing = -1;
}

Landmarks::Landmark::~Landmark()
{}

Landmarks::Landmarks(double degrees)
{
  std::vector<std::pair<int, int> > idCpy(MAXLANDMARKS);
  std::vector<Landmarks::Landmark *> landmarkCpy(MAXLANDMARKS);
  this->DBSize = 0;
  this->EKFLandmarks = 0;
  this->degreePerScan = degrees;
  this->IDtoID = idCpy;
  this->landmarkDB = landmarkCpy;
}

Landmarks::~Landmarks()
{}

int Landmarks::getDBSize() const
{
  return (this->DBSize);
}

int Landmarks::getSLamId(int id) const
{
  for(int i = 0; i < this->EKFLandmarks; ++i)
    {
      if(IDtoID[i].first == id)
	return (IDtoID[i].second);
    }
  return (-1);
}

int Landmarks::addSlamId(int landmarkID, int slamID)
{
  std::pair<int, int> newSlamID;
  newSlamID.first = landmarkID;
  newSlamID.second = slamID;
  this->IDtoID[EKFLandmarks] = newSlamID;
  ++this->EKFLandmarks;
  return (0);
}

int Landmarks::getAssociation(Landmark &lm)
{
  for(int i = 0; i < this->DBSize; ++i)
    {
      if(this->distance(lm, (*landmarkDB[i])) < MAXERROR && landmarkDB[i]->id != -1)
	{
	  landmarkDB[i]->life = LIFE;
	  ++landmarkDB[i]->totalTimeObserved;
	  landmarkDB[i]->bearing = lm.bearing;
	  landmarkDB[i]->range = lm.range;
	  return (landmarkDB[i]->id);
	}
    }
  return -1;
}

double Landmarks::distance(double x1, double y1, double x2, double y2) const
{
  return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

double Landmarks::distance(const Landmark &lm1, const Landmark &lm2) const
{
  return (sqrt(pow(lm1.pos[0] - lm2.pos[0], 2) + pow(lm1.pos[1] - lm2.pos[1], 2)));
}

#include <iostream>

int Landmarks::addToDB(const Landmark &lm)
{
  Landmarks::Landmark	*new_elem;

  if(static_cast<unsigned int>(DBSize + 1) < this->landmarkDB.size())
    {
      new_elem = new Landmarks::Landmark();
      
      new_elem->pos[0] = lm.pos[0];
      new_elem->pos[1] = lm.pos[1];
      new_elem->life = LIFE;
      new_elem->id = DBSize;
      new_elem->totalTimeObserved = 1;
      new_elem->bearing = lm.bearing;
      new_elem->range = lm.range;
      new_elem->a = lm.a;
      new_elem->b = lm.b;

      landmarkDB[DBSize] = new_elem;
      ++DBSize;
      return (DBSize - 1);
    }
  return (-1);
}

double Landmarks::distanceToLine(double x, double y, double a, double b)
{
  double ao = -1.0 / a;
  double bo = y - ao * x;
  double px = (b - bo) / (ao - a);
  double py = ((ao * (b - bo)) / (ao - a)) + bo;
  return (this->distance(x, y, px, py));
}

std::vector<Landmarks::Landmark *>Landmarks::getLandmarkDB() const
{
  std::vector<Landmark *> res;

  for (int i = 0; i < this->DBSize; ++i)
    {
      res[i] = this->landmarkDB[i];
    }
  return (res);
}

void Landmarks::getClosestAssociation(Landmark *lm, int &id, int &totalTimeObserved)
{
  int closestLandmark = 0;
  double temp;
  double leastDistance = 99999; // 100k meter, big distance

  for(int i = 0; i < DBSize; ++i)
    {
      if(static_cast<unsigned int>(landmarkDB[i]->totalTimeObserved) > MINOBSERVATIONS)
	{
	  temp = this->distance(*lm, *landmarkDB[i]);
	  if(temp < leastDistance)
	    {
	      leastDistance = temp;
	      closestLandmark = landmarkDB[i]->id;
	    }
	}
    }
  if(leastDistance == 99999)
    id = -1;
  else
    {
      id = landmarkDB[closestLandmark]->id;
      totalTimeObserved = landmarkDB[closestLandmark]->totalTimeObserved;
    }
}
