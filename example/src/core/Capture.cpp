#include "Capture.hh"

Capture::Capture()
  : _grabber(new pcl::io::OpenNI2Grabber())
{
  boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> getData = boost::bind (&Capture::captureData, this, _1);
  _grabber->registerCallback(getData);
  this->start();
}

Capture::~Capture()
{
  _grabber->stop();
  delete _grabber;
}

void Capture::start() const {
  _grabber->start();
  std::cerr << "grabber start" << std::endl;
}

void Capture::stop() const {
  _grabber->stop();
  std::cerr << "grabber stop" << std::endl;
}

void Capture::captureData(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
{
  // if (pcl::io::loadPCDFile<pcl::PointXYZ> ("pcdData/data/tutorials/ism_test_cat.pcd", _cloud) == -1) //* load the file
  // {
  //   PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
  //   exit(1);
  // }
  // std::cerr << "Loaded "
  //           << _cloud.width * _cloud.height
  //           << " data points from test_pcd.pcd with the following fields: "
  //           << std::endl;
  // for (size_t i = 0; i < _cloud.points.size (); ++i)
  //   std::cerr << "    " << _cloud.points[i].x
  //             << " "    << _cloud.points[i].y
  //             << " "    << _cloud.points[i].z << std::endl;
  // pcl::PointCloud<pcl::PointXYZ>::Ptr cloudClean(new pcl::PointCloud<pcl::PointXYZ>());
  // /// Clean noise
  // pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
  // sor.setInputCloud(cloud);
  // sor.setMeanK(50);
  // sor.setStddevMulThresh(1.0);
  // sor.filter(*cloudClean);
  // /// Reduce cloud to send size to 10% of orignial size more or less
  // pcl::VoxelGrid<pcl::PointXYZ> vox;
  // vox.setInputCloud(cloudClean);
  // vox.setLeafSize(0.05f, 0.05f, 0.05f);
  // vox.filter(*cloudClean);
  std::cerr << "Start capture" << std::endl;
  if (!_cloud->empty())
      _cloud->clear();
  pcl::copyPointCloud(*cloud, *_cloud);
  this->dispatch("takeDataEvent");
  boost::this_thread::sleep(boost::posix_time::millisec(10));

  // std::cerr << "Loaded "
  //           << _cloud.points.size()
  //           << " data points"
  //           << std::endl;
}