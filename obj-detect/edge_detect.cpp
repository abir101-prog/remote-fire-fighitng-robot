#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

//Rect bound(200,100,200,200);
Rect bound(0,0,700,700);
Mat blur, gray, grad;

Point2f computeIntersect(Vec4i a, Vec4i b)
{
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4)))
    {
        Point2f pt;
        pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;
        pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d;
        return pt;
    }
    else
        return Point2f(-1, -1);
}

int main( int argc, char** argv )
{
  Mat src = imread("empty2.jpg");
  Mat roi = src(bound);
  Mat blury, grayy, thresh, dst;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  Mat finals, finals2;


  //bilateralFilter( roi, blury, 50, 5, BORDER_DEFAULT );
  cvtColor( roi, grayy, COLOR_BGR2GRAY );
  //threshold(grayy,thresh,127,255,THRESH_BINARY);
  //dilate( thresh, thresh, MORPH_RECT );
  //dilate( thresh, thresh, MORPH_RECT );
  //dilate( thresh, thresh, MORPH_RECT );
  //Sobel( grayy, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  //Sobel( grayy, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  //convertScaleAbs( grad_x, abs_grad_x );
  //convertScaleAbs( grad_y, abs_grad_y );
  //addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  Canny( grayy, finals, 50, 150, 3 );
  dilate( finals, finals, MORPH_RECT );
  dilate( finals, finals, MORPH_RECT );
  dilate( finals, finals, MORPH_RECT );
  cvtColor(finals, finals2, COLOR_GRAY2BGR);
  vector<Vec4i> lines;
  HoughLinesP(finals, lines, 1, CV_PI/180, 30, 50, 10 );

  vector<Point2f> corners;
  vector<Vec4i> edgedetect;
  for (int i = 0; i < lines.size(); i++)
  {
      for (int j = i+1; j < lines.size(); j++)
      {
          Point2f pt = computeIntersect(lines[i], lines[j]);
          Vec4i l = lines[i];
          Vec4i h = lines[j];
          if (pt.x >= 0 && pt.y >= 0)
                  edgedetect.push_back(l);
                  edgedetect.push_back(h);
                  for (int r = 0; r < lines.size(); r += 2)
                      for(int k = r + 1; k < lines.size(); k++)
                      {
                          Vec4i e = edgedetect[i];
                          Vec4i f = edgedetect[j];
                          line( finals2, Point(e[0], e[1]), Point(e[2], e[3]), Scalar(0,0,255), 3, LINE_AA);
                          line( finals2, Point(f[0], f[1]), Point(f[2], f[3]), Scalar(0,0,255), 3, LINE_AA);
                      }
      }
  }


  namedWindow("original",WINDOW_AUTOSIZE);
  imshow("original",src);
  namedWindow("test",WINDOW_AUTOSIZE);
  imshow("test", finals2);



  waitKey(0);
  return 0;
}