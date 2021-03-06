#include "igloo/igloo_alt.h"
#include "Landmarks.hh"
#include "DataAssociation.hh"
#include "CustomConstraint.hh"
#include "test_slam_common.hh"

using namespace igloo;

When(creating_data_association)
{
  void SetUp()
    {
      landmarks = new Landmarks();
      datas = new DataAssociation(landmarks);
    }

  Then(it_should_set_the_db)
    {
      AssertThatDetail(datas->_landmarkDb, Is().EqualTo(landmarks));
    }

  ::DataAssociation *datas;
  ::Landmarks *landmarks;
};

When(getting_db_from_data_association)
{
  void SetUp()
    {
      landmarks = new Landmarks();
      datas._landmarkDb = landmarks;
    }

  Then(it_should_return_the_db)
    {
      AssertThatDetail(landmarks, Is().EqualTo(datas.getLandmarkDb()));
    }

  ::DataAssociation datas;
  ::Landmarks *landmarks;
};

/*
 * Test landmark association
 */
When(associating_landmarks)
{
  ScenarioAttribute("hasChild", "true")

  void SetUp()
    {
      landmarks = new Landmarks();
      datas = new DataAssociation(landmarks);
      to_test = new Landmarks::Landmark();
      id = to_test->id;
      totalTimeObserved = to_test->totalTimeObserved;
    }

  When(landmarks_db_is_empty)
  {
    ScenarioAttribute("hasParent", "\t")

      void SetUp()
      {
	Root().result = Root().datas->associateLandmarks(Root().to_test);
      }

    Then(it_should_return_false)
    {
      AssertThatDetail(Root().result, Is().EqualTo(false));
    }

    Then(it_should_not_change_landmark_data)
    {
      AssertThatDetail(Root().to_test->id, Is().EqualTo(Root().id));
      AssertThatDetail(Root().to_test->totalTimeObserved,
		   Is().EqualTo(Root().totalTimeObserved));
    }
  };

  When(landmarks_db_is_not_empty)
  {
    ScenarioAttribute("hasParent", "\t")

      void SetUp()
      {
	Root().new_id = Root().landmarks->addToDB(*Root().to_test);
	Root().landmarks->landmarkDB[Root().new_id]->totalTimeObserved =
	  Landmarks::MINOBSERVATIONS + 1;
	Root().result = Root().datas->associateLandmarks(Root().to_test);
      }

    Then(it_should_return_true)
    {
      AssertThatDetail(Root().result, Is().EqualTo(true));
    }

    Then(it_should_change_landmark_data)
    {
      AssertThatDetail(Root().to_test->id, Is().Not().EqualTo(Root().id));
      AssertThatDetail(Root().to_test->totalTimeObserved,
		   Is().Not().EqualTo(Root().totalTimeObserved));
    }

    Then(it_should_set_good_landmark_data)
    {
      AssertThatDetail(Root().to_test->id, Is().EqualTo(Root().new_id));
      AssertThatDetail(Root().to_test->totalTimeObserved,
		   Is().EqualTo(Landmarks::MINOBSERVATIONS + 1));
    }
  };

  ::DataAssociation *datas;
  ::Landmarks *landmarks;
  ::Landmarks::Landmark *to_test;
  bool	result;
  int	id;
  int	totalTimeObserved;
  int	new_id;
};

When(testing_validation_gate)
{
  ScenarioAttribute("hasChild", "true")

    void	SetUp()
    {

      agent = new Agent();
      agent->setPos(2.0, 4.0, 0.0);
      agent->setBearing(0.2);
    }

  When(no_landmarks_are_in_db)
  {
    ScenarioAttribute("hasParent", "\t")

      void	SetUp()
      {
	pcl::PointCloud<pcl::PointXYZ> cloud;
	::Landmarks	*lms = new Landmarks();

	srand(42);
	TestSlamCommon::generateData(cloud, Root().numberSample);
	Root().landmarksTest = lms->extractLineLandmarks(cloud, *Root().agent);
	Root().datas = new DataAssociation(lms);

	Root().datas->validationGate(cloud, *Root().agent, Root().result, Root().reobservedLandmark);
      }

    Then(it_should_add_landmarks_to_result)
    {
      AssertThatDetail(Root().result.size(), Is().GreaterThan(0));
    }
  };

  When(landmarks_are_already_in_db_and_already_enough_observed)
  {
    ScenarioAttribute("hasParent", "\t")

      void	SetUp()
      {
	pcl::PointCloud<pcl::PointXYZ> cloud;
	::Landmarks	*lms = new Landmarks();

	srand(42);
	TestSlamCommon::generateData(cloud, Root().numberSample);
	Root().landmarksTest = lms->extractLineLandmarks(cloud, *Root().agent);
	Root().datas = new DataAssociation(lms);

	for(std::vector<Landmarks::Landmark *>::iterator it = Root().landmarksTest.begin(); it != Root().landmarksTest.end(); ++it)
	  {
      int id = Root().datas->getLandmarkDb()->addToDB(**it);
	    ids.push_back(id);
      Root().datas->getLandmarkDb()->landmarkDB[id]->totalTimeObserved = Landmarks::MINOBSERVATIONS + 1;
	  }

	Root().oldDbSize = Root().datas->getLandmarkDb()->getDBSize();
	Root().datas->validationGate(cloud, *Root().agent, Root().result, Root().reobservedLandmark);
      }

    Then(it_should_have_less_landmark_in_result_than_in_db)
    {
      AssertThatDetail(Root().result.size(), Is().LessThan(Root().landmarksTest.size()));
    }

    Then(it_shoud_add_observed_time_to_landmarks_observed)
    {
      bool	isObserved = false;

      AssertThatDetail(Root().reobservedLandmark.size(), Is().GreaterThan(0));
      for(std::vector<Landmarks::Landmark *>::iterator it = Root().reobservedLandmark.begin(); it != Root().reobservedLandmark.end(); ++it)
	{
	  if (Root().datas->getLandmarkDb()->landmarkDB[(*it)->id]->totalTimeObserved > 1)
	    {
	      isObserved = true;
	      break;
	    }
	}
      AssertThatDetail(isObserved, Is().EqualTo(true));
    }

    std::vector<int> ids;
  };

  static const int numberSample = 500;
  ::DataAssociation *datas;
  std::vector<Landmarks::Landmark *>	landmarksTest;
  Agent	*agent;
  std::vector<Landmarks::Landmark *> result;
  std::vector<Landmarks::Landmark *> reobservedLandmark;
  int		oldDbSize;
  int r;
};
