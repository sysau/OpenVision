#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "Common/OpenCVCommon.hpp"
class PreProcess
{
public:
    PreProcess(){};

    Mat gammaCorrection(Mat image,float gamma)
    {
        Mat tmp;
        image.convertTo(tmp, CV_32FC1,1.0/255.0,0);
        pow(tmp, gamma, tmp);
        tmp.convertTo(tmp,CV_8UC1,255.0,0);
        return tmp;
    }

    Mat illumNorm(Mat image,float sigma0,float sigma1,float alpha=0.1,float tau=10)
    {
      Mat tmp;
      image.convertTo(tmp,CV_32FC1,1.0/255.0,0);
      Mat gaussian0, gaussian1;
      int kernel_sz0 = (1*sigma0);
      int kernel_sz1 = (4*sigma1);
      kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
      kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
      GaussianBlur(tmp, gaussian0, Size(kernel_sz0,kernel_sz0), sigma0, sigma0, BORDER_CONSTANT);
      GaussianBlur(tmp, gaussian1, Size(kernel_sz1,kernel_sz1), sigma1, sigma1, BORDER_CONSTANT);
      cv::subtract(gaussian0,gaussian1,tmp);

      cv::imwrite("/media/software/Dropbox/Dropbox/repository/im/documents/linuxOS/lbp11.png",tmp);
      Mat tmp1;
      //imshow("g21",tmp);
      double meanI = 0.0;
      cv::pow(abs(tmp),alpha,tmp1);
      meanI = mean(tmp1).val[0];
      tmp = tmp / pow(meanI, 1.0f/alpha);

      pow(min(abs(tmp), tau), alpha, tmp1);
      meanI = mean(tmp1).val[0];
      tmp = tmp / pow(meanI, 1.0f/alpha);


      cv::imwrite("/media/software/Dropbox/Dropbox/repository/im/documents/linuxOS/lbp12.png",tmp);

      for(int r = 0; r < tmp.rows; r++) {
          for(int c = 0; c < tmp.cols; c++) {
              tmp.at<float>(r,c) = tanh(tmp.at<float>(r,c) / tau);
          }
      }
      tmp = tau * tmp;
      tmp=tmp+tau;
      cv::normalize(tmp,tmp,0,255,CV_MINMAX);

      cv::imwrite("/media/software/Dropbox/Dropbox/repository/im/documents/linuxOS/lbp13.png",tmp);
      tmp.convertTo(tmp,CV_8UC1,1.0,0);
      return tmp;

    }

    Mat histEqualization(Mat image)
    {
        Mat tmp;
        cv::equalizeHist(image,tmp);
        return tmp;
    }

    Mat contrastStretching(Mat image)
    {
        Mat tmp;
        cv::normalize(image,tmp,0,255,CV_MINMAX);
        return tmp;
    }



};

#endif // PREPROCESS_H
