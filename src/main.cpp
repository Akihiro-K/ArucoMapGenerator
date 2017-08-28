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

using namespace cv;
using namespace aruco;
using namespace std;

// =============================================================================
// Parameters:
#define mSize 16
#define mInfo 1

static int ID_LIST[] = {111,222,333,444,555,667,777,888,999,11,22,33,44,55,66,77};
static double CENTX_LIST[] = {-0.995,0,0.995,0,0.26,-0.26,0.18,-0.18,0.28,-0.28,0.1,-0.1,0.87,-0.87,-0.87,0.87};
static double CENTY_LIST[] = {0,0.995,0,-0.995,0.26,-0.26,-0.39,0.39,-0.13,0.13,-0.1,0.1,0.87,0.87,-0.87,-0.87};
static double ALT_LIST[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static double SIZE_LIST[] = {0.85,0.85,0.85,0.85,0.42,0.42,0.26,0.26,0.16,0.16,0.1,0.1,0.6,0.6,0.6,0.6};
// =============================================================================

void savetofile(string filepath);
void dispoutput(string filepath);
Point3d getcorner(int markernum, int cornernum);

int main(int ac, char** av)
{
    string filepath = "./output.yml";

    savetofile(filepath);

    dispoutput(filepath);

    return 0;
}

void savetofile(string filepath){
    FileStorage fs(filepath, cv::FileStorage::WRITE);

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
            Point3d temp=getcorner(i,j); 
            //fs << setprecision(4) << temp;
            fs << temp;
        }
        fs << "]";
        fs << "}";
    }
    fs << "]";    
}

void dispoutput(string filepath){
    MarkerMap MM;
    MM.readFromFile(filepath);
    cout << "generated map is valid" << endl;
    cout << endl;

    Dictionary dic = Dictionary::loadPredefined("ARUCO");
    cout << "*****************Generated Map********************" << endl;
    cout << "aruco_bc_nmarkers: " << (int)MM.size() << endl;
    cout << "aruco_bc_mInfoType: " << (int)MM.mInfoType << endl;;
    cout << "aruco_bc_markers: " << endl;
    for (size_t i = 0; i < MM.size(); i++) {
        cout << i+1;
        if (i%10==0) {
            cout << "st marker: "  << "id #" << MM[i].id << endl;
        } else if (i%10==1) {
            cout << "nd marker: "  << "id #" << MM[i].id << endl;
        } else if (i%10==2) {
            cout << "rd marker: "  << "id #" << MM[i].id << endl;
        } else {
            cout << "th marker: "  << "id #" << MM[i].id << endl;
        }
        for (size_t c = 0; c < MM[i].size(); c++){
            cout << "\t" << "corner" << c+1 << ": ";
            cout << MM[i][c].x << "\t" << MM[i][c].y << "\t" << MM[i][c].z << endl;
         }
         ostringstream oss1;
         oss1 << "#" << MM[i].id;
         string windowname = oss1.str();
         cv::imshow(windowname, dic.getMarkerImage_id(MM[i].id,100*SIZE_LIST[i]/SIZE_LIST[0]));
         cv::waitKey(0);
         ostringstream oss2;
         oss2 << MM[i].id << ".png";
         string filename = oss2.str();
         cv::imwrite(filename, dic.getMarkerImage_id(MM[i].id,100*SIZE_LIST[i]/SIZE_LIST[0]));    
    }
    cout << "************************************************" << endl;    
}

Point3d getcorner(int markernum, int cornernum){
    Point3d temp;
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
            break;;    
    }
    return temp;
}