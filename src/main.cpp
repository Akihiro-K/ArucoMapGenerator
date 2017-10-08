#include <aruco/aruco.h>
#include <aruco/markermap.h>
#include <aruco/posetracker.h>
#include <aruco/cvdrawingutils.h>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;

// =============================================================================
// Sample Map Configuration file (.json):

// {
//   "MKR_0": {
//     "id": 111,
//     "centx": 0,
//     "centy": 0,
//     "alt": 0,
//     "size": 0.5
//   },
//   "MKR_1": {
//     "id": 222,
//     "centx": 1,
//     "centy": 0,
//     "alt": 0,
//     "size": 0.5
//   }
// }

// =============================================================================

// =============================================================================
// Parameters:
const int mSize = 0;
const int mInfo = 0;

std::vector<int> ID_LIST;
std::vector<double> CENTX_LIST;
std::vector<double> CENTY_LIST;
std::vector<double> ALT_LIST;
std::vector<double> SIZE_LIST;

// =============================================================================

void readfromfile(std::string filepath);
void savetofile(std::string filepath);
void dispoutput(std::string filepath);
cv::Point3d getcorner(int markernum, int cornernum);

int main(int ac, char** av)
{
  std::string filepath_in = "../input_data/configuration.json"
  std::string filepath_out = "../output_data/map.yml";

  readfromfile(filepath_in);

  savetofile(filepath_out);

  dispoutput(filepath_out);

  return 0;
}

void readfromfile(std::string filepath)
{
  std::ifstream ifs(filepath);
  json j_;
  ifs >> j_;
  mSize = j_.size();
  mInfo = 1;
  for (int i = 0; i < mSize; i++) {
    std::ostringstream oss;
    oss << "MKR_" << i;
    std::string mkr_name = oss.str();
    ID_LIST.push_back(j_[mkr_name.c_str()]["id"]);
    CENTX_LIST.push_back(j_[mkr_name.c_str()]["centx"]);
    CENTY_LIST.push_back(j_[mkr_name.c_str()]["centy"]);
    ALT_LIST.push_back(j_[mkr_name.c_str()]["alt"]);
    SIZE_LIST.push_back(j_[mkr_name.c_str()]["size"]);
  }
}

void savetofile(std::string filepath){
  cv::FileStorage fs(filepath, cv::FileStorage::WRITE);

  fs << "aruco_bc_nmarkers" << mSize;
  fs << "aruco_bc_mInfoType" << mInfo;
  fs << "aruco_bc_markers"
      << "[";

  for (int i=0; i < mSize; i++) {
    fs << "{:"
        << "id" << ID_LIST[i];
    
    fs << "corners"
        << "[:";
    for (int j = 0; j < 4; j++){
      cv::Point3d temp=getcorner(i,j); 
      fs << temp;
    }
    fs << "]";
    fs << "}";
  }
  fs << "]";
}

void dispoutput(srd::string filepath){
  aruco::MarkerMap MM;
  MM.readFromFile(filepath);
  std::cout << "generated map is valid" << std::endl;
  std::cout << std::endl;

  aruco::Dictionary dic = aruco::Dictionary::loadPredefined("ARUCO");
  std::cout << "*****************Generated Map********************" << std::endl;
  std::cout << "aruco_bc_nmarkers: " << (int)MM.size() << std::endl;
  std::cout << "aruco_bc_mInfoType: " << (int)MM.mInfoType << std::endl;;
  std::cout << "aruco_bc_markers: " << std::endl;
  for (size_t i = 0; i < MM.size(); i++) {
    std::cout << i+1;
    if (i%10==0) {
        std::cout << "st marker: "  << "id #" << MM[i].id << std::endl;
    } else if (i%10==1) {
        std::cout << "nd marker: "  << "id #" << MM[i].id << std::endl;
    } else if (i%10==2) {
        std::cout << "rd marker: "  << "id #" << MM[i].id << std::endl;
    } else {
        std::cout << "th marker: "  << "id #" << MM[i].id << std::endl;
    }
    for (size_t c = 0; c < MM[i].size(); c++){
        std::cout << "\t" << "corner" << c+1 << ": ";
        std::cout << MM[i][c].x << "\t" << MM[i][c].y << "\t" << MM[i][c].z << std::endl;
    }
    std::ostringstream oss1;
    oss1 << "#" << MM[i].id;
    std::string windowname = oss1.str();
    cv::imshow(windowname, dic.getMarkerImage_id(MM[i].id,100*SIZE_LIST[i]/SIZE_LIST[0]));
    cv::waitKey(0);
    std::ostringstream oss2;
    oss2 << MM[i].id << ".png";
    std::string filename = oss2.str();
    cv::imwrite(filename, dic.getMarkerImage_id(MM[i].id,100*SIZE_LIST[i]/SIZE_LIST[0]));
  }
  std::cout << "************************************************" << std::endl;
}

cv::Point3d getcorner(int markernum, int cornernum){
  cv::Point3d temp;
  switch(cornernum){
    case 0:
      temp.x=(CENTX_LIST[markernum]-SIZE_LIST[markernum]/2.0);
      temp.y=(CENTY_LIST[markernum]+SIZE_LIST[markernum]/2.0);
      temp.z=ALT_LIST[markernum];
      break;
    case 1:
      temp.x=(CENTX_LIST[markernum]-SIZE_LIST[markernum]/2.0);
      temp.y=(CENTY_LIST[markernum]-SIZE_LIST[markernum]/2.0);
      temp.z=ALT_LIST[markernum];
      break;
    case 2:
      temp.x=(CENTX_LIST[markernum]+SIZE_LIST[markernum]/2.0);
      temp.y=(CENTY_LIST[markernum]-SIZE_LIST[markernum]/2.0);
      temp.z=ALT_LIST[markernum];
      break;
    case 3:
      temp.x=(CENTX_LIST[markernum]+SIZE_LIST[markernum]/2.0);
      temp.y=(CENTY_LIST[markernum]+SIZE_LIST[markernum]/2.0);
      temp.z=ALT_LIST[markernum];
      break;
  }
  return temp;
}