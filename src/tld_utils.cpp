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

//均值
int meanDev(unsigned char* src, int w, int h)
{
    int sum = 0;
    int num = w * h;
    int mean = 0;
    int i, j;
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            sum += *(src + i * w + j);
        }
    }

    mean = sum / num;
    return mean;
}

Mat my_resize(const Mat &img, int width, int height)
{
	Mat output = Mat::zeros(Size(width, height), CV_8UC1);
	uchar* dataDst = output.data;
	uchar* dataSrc = img.data;
	double xRatio = (double)img.cols / width;
	double yRatio = (double)img.rows / height;

	for (int i = 0; i < height; i++)
	{
		double srcY = i * yRatio;//源图像“虚”坐标的y值
		int IntY = (int)srcY;//向下取整
		double v = srcY - IntY;//获取小数部分
		double v1 = 1.0 - v;
		for (int j = 0; j < width; j++)
		{
		    double srcX = j * xRatio;//源图像“虚”坐标的x值
		    int IntX = (int)srcX;//向下取整
		    double u = srcX - IntX;//获取小数部分
		    double u1 = 1.0 - u;

		    int Index00 = IntY * img.cols + IntX;//得到原图左上角的像素位置
		    int Index10;                            //原图左下角的像素位置
		    if (IntY < img.rows - 1)
		    {
		        Index10 = Index00 + img.cols;
		    }
		    else
		    {
		        Index10 = Index00;
		    }
		    int Index01;                            //原图右上角的像素位置
		    int Index11;                            //原图右下角的像素位置
		    if (IntX < img.cols - 1)
		    {
		        Index01 = Index00 + 1;
		        Index11 = Index10 + 1;
		    }
		    else
		    {
		        Index01 = Index00;
		        Index11 = Index10;
		    }
		    double temp0 = (v1 * (u * dataSrc[Index01] + u1 * dataSrc[Index00]) +
		                    v * (u * dataSrc[Index11] + u1 * dataSrc[Index10]));
			*(dataDst + i*width + j) = temp0;
		}
	}

	return output;
}
