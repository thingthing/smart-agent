#ifndef   _CAPTURE_HH_
# define  _CAPTURE_HH_

#include <string>

#include <pcl/io/openni2_grabber.h>
#include <pcl/common/common.h>
#include <pcl/impl/point_types.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/io/openni_grabber.h>
#include <pcl/io/oni_grabber.h>
#include <pcl/point_types.h>
#include <pcl/common/projection_matrix.h>

#include "ICapture.hh"

class   Capture : public ICapture
{
public:

  Capture();
  virtual ~Capture();

  virtual void captureData(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&);
  virtual void start() const;
  virtual void stop() const;

private:
  pcl::Grabber *_grabber;
};


#endif    /* !_CAPTURE_HH_ */
