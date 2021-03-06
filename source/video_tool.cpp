//Time_based_video vid_maker(capture, fac, 1000, 1100);
#include <stdio.h>            //?in/out?
#include <opencv2/opencv.hpp> //opencv library
#include <time.h>             //time measuring & seed
#include <string.h>           //? <3 string ?
#include <vector>             //storing image data
#include <sys/types.h>        //?folder?
#include <dirent.h>           //?folder?
#include <errno.h>            //?folder?
#include <iostream>           //?in/out?
#include <math.h>             //math
#include "image_tool.cpp"

using namespace cv;

class Time_based_video{
public:
  Time_based_video(std::string file, Frame_factors& factors, int v_start, int v_end, int len):
      _File(file),
      _Factors(factors),
      _V_start(v_start),
      _V_end(v_end),
      _I_len(len)
      {
          _Pictures=VideoCapture(_File);
      }



  void render(){
    cv::Mat currentFrame;
    _Pictures>>currentFrame;
    _X=currentFrame.cols;
    _Y=currentFrame.rows;
    int totalFrameNumber = _Pictures.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout<<"expects: "<<totalFrameNumber<<" frames\n";

    Size frameSize(static_cast<int>(_X), static_cast<int>(_Y));
    VideoWriter oVideoWriter ("out.avi", CV_FOURCC('P','I','M','1'), 50, frameSize, true);
    cv::Mat dst=cv::Mat(_Y, _X, CV_64FC3, cv::Scalar(0.0f,0.0f,0.0f));
    int err_count=0;
    int faktor_count=0;
    //oVideoWriter.write(read_image(*p, count, power));

    for (unsigned int i = 0;i <totalFrameNumber-1; i++) //through video
    {
      if(i>=_V_start && i+_I_len<=_V_end)                //in our range?
      {
        //std::cout<<"herea   "<<i<<"\n";
        VideoCapture vid= VideoCapture(_File);
        Time_based_picture pic_maker(vid, _Factors, i, i+_I_len-1);
        Mat newImage;
        (pic_maker.render()).convertTo(newImage,  CV_8UC3);

        imwrite( "./outllb.jpg", newImage );
        oVideoWriter.write(newImage);//Mat(_X, _Y, CV_8UC3, Scalar(0,0,0)));//pic_maker.render());
        //std::cout<<"herea   "<<i<<"\n";
      }
      if(i+_I_len>=_V_end)
      {
        break;
      }
    }
    std::cout<<"donedone\n";
    //imwrite( "./outf.jpg", dst );
  }

  void render_dynamic_longtime(){
    cv::Mat currentFrame;
    _Pictures>>currentFrame;
    _X=currentFrame.cols;
    _Y=currentFrame.rows;
    int totalFrameNumber = _Pictures.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout<<"expects: "<<totalFrameNumber<<" frames\n";

    Size frameSize(static_cast<int>(_X), static_cast<int>(_Y));
    VideoWriter oVideoWriter ("out.avi", CV_FOURCC('P','I','M','1'), 50, frameSize, true);
    VideoWriter oVideoWriter2 ("out2.avi", CV_FOURCC('P','I','M','1'), 50, frameSize, true);

    cv::Mat dst=cv::Mat(_Y, _X, CV_64FC3, cv::Scalar(0.0f,0.0f,0.0f));
    cv::Mat dst2=cv::Mat(_Y, _X, CV_64FC3, cv::Scalar(0.0f,0.0f,0.0f));
    int err_count=0;
    int faktor_count=0;
    //oVideoWriter.write(read_image(*p, count, power));
    VideoCapture vid= VideoCapture(_File);
    Mat current;
    for (unsigned int i = 0;i <totalFrameNumber-1; i++) //through video
    {
      vid>>current;
      if(i>=_V_start && i<=_V_end)                //in our range?
      {
        float index=i-_V_start+1;
        std::cout<<"here"<<index<<"\n";
        //alt: n-1/n
        Mat tmp;
        Mat tmp2;
        current.convertTo(tmp,  CV_64FC3);
        addWeighted( dst2, 1.0,tmp, 1.0, 0.0, tmp2);
        addWeighted( dst, (float)(index-1)/((float) index),tmp, 1/index, 0.0, tmp);
        dst=tmp;
        dst2=tmp2;
        Mat tmp_o;//=dst2.clone();
          Mat out;
        Mat out2;
        addWeighted(dst2, 1/index,tmp, 0.0, 0.0, tmp_o);
       tmp_o.convertTo(out2,  CV_8UC3);
        dst.convertTo(out,  CV_8UC3);


        //neu: 1/(n)
        imwrite("./outllb2.jpg", out2 );
        oVideoWriter2.write(out2);
      //  dst2.convertTo(out2,  CV_8UC3);
      //  imwrite("./outllb3.jpg", out2 );
        imwrite("./outllb.jpg", out );
        oVideoWriter.write(out);//Mat(_X, _Y, CV_8UC3, Scalar(0,0,0)));//pic_maker.render());
        //std::cout<<"herea   "<<i<<"\n";
      }
      if(i>=_V_end)
      {
        break;
      }
    }
    std::cout<<"donedone\n";
    //imwrite( "./outf.jpg", dst );
  }

private:
  int                 _X;
  int                 _Y;
  std::string             _File;
  VideoCapture        _Pictures;
  Frame_factors       _Factors;
  int                 _V_start;
  int                 _V_end;
  int                 _I_len;
};
