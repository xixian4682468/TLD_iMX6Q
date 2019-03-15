#include <tld_utils.h>


void drawBox(Mat& image, CvRect box, Scalar color, int thick)
{
	rectangle( image, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),color, thick);
} 

void drawPoints(Mat& image, vector<Point2f> points,Scalar color)
{
	for(vector<Point2f>::const_iterator i = points.begin(), ie = points.end(); i != ie; ++i)
	{
		Point center( cvRound(i->x ), cvRound(i->y));
		circle(image,*i,2,color,1);
	}
}

Mat createMask(const Mat& image, CvRect box)
{
	Mat mask = Mat::zeros(image.rows,image.cols,CV_8U);
	drawBox(mask,box,Scalar::all(255),CV_FILLED);
	return mask;
}

float median(vector<float> v)
{
    int n = floor(v.size() / 2);
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}

vector<int> index_shuffle(int begin,int end)
{
	vector<int> indexes(end-begin);
	for (int i=begin;i<end;i++)
		indexes[i]=i;

	random_shuffle(indexes.begin(),indexes.end());
	return indexes;
}

void myResize(const unsigned char *dataSrc, unsigned char *dataDst, int src_width, int src_height, int width, int height)
{
  double xRatio = (double)src_width / width;
  double yRatio = (double)src_height / height;

  for (int i = 0; i < height; i++)
  {
        double srcY = (i + 0.5) * yRatio - 0.5;//源图像“虚”坐标的y值
    int IntY = (int)srcY;//向下取整
    double v = srcY - IntY;//获取小数部分
    double v1 = 1.0 - v;
    for (int j = 0; j < width; j++)
    {
            double srcX = (j + 0.5) * xRatio - 0.5;//源图像“虚”坐标的x值
        int IntX = (int)srcX;//向下取整
        double u = srcX - IntX;//获取小数部分
        double u1 = 1.0 - u;

        int Index00 = IntY * src_width + IntX;//得到原图左上角的像素位置
        int Index10;                            //原图左下角的像素位置
        if (IntY < src_height - 1)
        {
            Index10 = Index00 + src_width;
        }
        else
        {
            Index10 = Index00;
        }
        int Index01;                            //原图右上角的像素位置
        int Index11;                            //原图右下角的像素位置
        if (IntX < src_width - 1)
        {
            Index01 = Index00 + 1;
            Index11 = Index10 + 1;
        }
        else
        {
            Index01 = Index00;
            Index11 = Index10;
        }
        int temp0 = (int)(v1 * (u * dataSrc[Index01] + u1 * dataSrc[Index00]) +
                        v * (u * dataSrc[Index11] + u1 * dataSrc[Index10]));
      *(dataDst + i*width + j) = temp0;
    }
  }
}

double myTemplateMatch(const Mat * pTemplate, const Mat * src, int w, int h)
{

    int nr= pTemplate->rows; // number of rows
    int nc= pTemplate->cols * pTemplate->channels(); // total number of elements per line
    uchar pTemplate_data[nc*nr];
    for (int j=0; j<nr; j++)
    {
      const uchar* data= pTemplate->ptr<uchar>(j);
      for (int i=0; i<nc; i++)
      {
        pTemplate_data[nc * j + i]= data[i];
      }
    }

    int nrs= src->rows; // number of rows
    int ncs= src->cols * src->channels(); // total number of elements per line
    uchar src_data[ncs*nrs];
    for (int j=0; j<nrs; j++)
    {
      const uchar* data= src->ptr<uchar>(j);
      for (int i=0; i<ncs; i++)
      {
        src_data[ncs * j + i]= data[i];
      }
    }


  int i, j, m, n;
  double dSumT;
  double dSumS;
  double dSumST;
  double R;

  double MaxR;

  int nMaxX;
  int nMaxY;
  int nHeight = /*src->rows*/ h;
  int nWidth = /*src->cols*/ w;

  int nTplHeight = /*pTemplate->rows*/ h;
  int nTplWidth = /*pTemplate->cols*/ w;


  dSumT = 0;
  for (m = 0; m < nTplHeight; m++)
  {
    for (n = 0; n < nTplWidth; n++)
    {

            int nGray =/**pTemplate->ptr(m, n)*/ *(pTemplate_data + m * nTplWidth + n);
      dSumT += (double)nGray*nGray;
    }
  }


  MaxR = 0;
  for (i = 0; i < nHeight - nTplHeight + 1; i++)
  {
    for (j = 0; j < nWidth - nTplWidth + 1; j++)
    {
      dSumST = 0;
      dSumS = 0;
      for (m = 0; m < nTplHeight; m++)
      {
        for (n = 0; n < nTplWidth; n++)
        {
            int nGraySrc = /**src->ptr(i + m, j + n)*/ *(src_data + (i + m) * nTplWidth + j + n);
            int nGrayTpl = /**pTemplate->ptr(m, n)*/ *(pTemplate_data + m * nTplWidth + n);
            dSumS += (double)nGraySrc*nGraySrc;
            dSumST += (double)nGraySrc*nGrayTpl;
        }
      }
      R = dSumST / (sqrt(dSumS)*sqrt(dSumT));

      if (R > MaxR)
      {
        MaxR = R;
        nMaxX = j;
        nMaxY = i;
      }
    }
  }
return MaxR;
}
