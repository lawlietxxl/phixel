#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h> 
#include <time.h> 


using namespace cv;
using namespace std;

#define PIXEL_LENGTH 20
#define SINGLE_PIC_LENGTH 20

Mat pixelize(Mat img, int pixelRows, int pixelCols);
Mat pixelizeGrayScale(Mat origin, int pixelRows, int pixelCols);
Mat pixelizeRGB(Mat origin, int pixelRows, int pixelCols);
map<uchar, Mat> mapFilesGrayScale(string folderPath);
map<string, Mat> mapFilesRGB(string folderPath);

uchar pixelOnePicGrayScale(Mat mat);
string pixelOnePicRGB(Mat mat);

Mat assenble(Mat pixelMat, map<uchar, Mat> mapU, int smallImageLength);
Mat assenble(Mat pixelMat, map<string, Mat> mapS, int smallImageLength);

int main( int argc, char** argv )
{   
    srand((unsigned)time(NULL));
    string imageName("./test_pics/test.jpg"); // by default
    string folderPath("./test_pics/candidates");
    string writePath("./test_pics/result_c.jpg");
    string mode("gray");
    //mode = gray/color
    if( argc >= 4)
    {
        imageName = argv[1];
        folderPath = argv[2];
        writePath = argv[3];
        if(argc >= 5){
            mode = argv[4];
            if(mode != "gray" && mode != "color"){
                cout << "Only support gray/color mode" <<endl;
                return -1;
            }
        }
    } else if(argc != 1){
        cout << "Please input: <imageName> <folderPath> <writePath> <mode: gray/color>" <<endl;
        return -1;
    }

    if(mode == "gray"){
        Mat img = imread(imageName.c_str(), IMREAD_GRAYSCALE);
        if( img.empty()){
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }
        Mat pix = pixelize(img, PIXEL_LENGTH, PIXEL_LENGTH);
        cout <<"After pixelization [colums * rows]: " << pix.size() <<endl;
        map<uchar, Mat> mapU = mapFilesGrayScale(folderPath);
        Mat result = assenble(pix, mapU, SINGLE_PIC_LENGTH);
        imwrite(writePath, result);
    }else{
        Mat img = imread(imageName.c_str(), IMREAD_COLOR);
        if( img.empty()){
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }
        Mat pix = pixelize(img, PIXEL_LENGTH, PIXEL_LENGTH);
        cout <<"After pixelization [colums * rows]: " << pix.size() <<endl;
        map<string, Mat> mapS = mapFilesRGB(folderPath);
        Mat result = assenble(pix, mapS, SINGLE_PIC_LENGTH);
        imwrite(writePath, result);
    }
    cout << "The file has been saved to " << writePath <<endl;
    return 0;
}


Mat pixelize(Mat img, int pixelRows, int pixelCols){
    Mat result;
    switch(img.channels()){
        case 1:{
            return pixelizeGrayScale(img, pixelRows, pixelCols);
        }
        case 3:{
            return pixelizeRGB(img, pixelRows, pixelCols);
        }
    }
};



Mat pixelizeGrayScale(Mat origin, int pixelRows, int pixelCols){
    int COL = origin.cols;
    int ROW = origin.rows;
    int area = pixelRows * pixelCols;
    
    int clubRow = ROW/pixelRows;
    int clubCol = COL/pixelCols;

    Mat result = Mat::zeros(clubRow, clubCol, CV_8UC1);

    for(int cr = 0; cr < clubRow; ++cr)
        for(int cc = 0; cc < clubCol; ++cc){
            int baseRow = cr * pixelRows;
            int baseCol = cc * pixelCols;
            long sumUp = 0;
            for(int r = 0; r < pixelRows; ++r)
                for(int c = 0; c < pixelCols; ++c){
                    //baseRow + r, baseCol + c
                    uchar* p = origin.ptr<uchar>(baseRow + r);
                    sumUp += (long)p[baseCol + c];
                }
            uchar *p = result.ptr<uchar>(cr);
            p[cc] = (long)sumUp/area;   
        }
    return result;
}

Mat pixelizeRGB(Mat origin, int pixelRows, int pixelCols){
    int COL = origin.cols;
    int ROW = origin.rows;
    int area = pixelRows * pixelCols;
    
    int clubRow = ROW/pixelRows;
    int clubCol = COL/pixelCols;

    Mat result = Mat::zeros(clubRow, clubCol, CV_8UC3);

    for(int cr = 0; cr < clubRow; ++cr)
        for(int cc = 0; cc < clubCol; ++cc){
            int baseRow = cr * pixelRows;
            int baseCol = cc * pixelCols * 3;
            long R = 0;
            long G = 0;
            long B = 0;
            for(int r = 0; r < pixelRows; ++r)
                for(int c = 0; c < pixelCols; ++c){
                    //baseRow + r, baseCol + c*3
                    uchar* p = origin.ptr<uchar>(baseRow + r);
                    R += (long)p[baseCol + c*3];
                    G += (long)p[baseCol + c*3 + 1];
                    B += (long)p[baseCol + c*3 + 2];
                }
            uchar *p = result.ptr<uchar>(cr);
            p[cc*3] = ((long)R)/area;  
            p[cc*3 + 1] = ((long)G)/area; 
            p[cc*3 + 2] = ((long)B)/area;  
        }
    return result;
}

map<uchar, Mat> mapFilesGrayScale(string folderPath){
    vector<String> filenames;
    glob(folderPath, filenames);
    map<uchar, Mat> result;
    for(size_t i = 0; i < filenames.size(); ++i){
        Mat m = imread(filenames[i], IMREAD_GRAYSCALE);
        result[pixelOnePicGrayScale(m)] = m;
    }
    return result;
}

map<string, Mat> mapFilesRGB(string folderPath){
    vector<String> filenames;
    glob(folderPath, filenames);
    map<string, Mat> result;
    for(size_t i = 0; i < filenames.size(); ++i){
        Mat m = imread(filenames[i], IMREAD_COLOR);
        result[pixelOnePicRGB(m)] = m;
    }
    return result;
};

Mat cutToSquare(Mat mat){
    int COL = mat.cols;
    int ROW = mat.rows;
    if(COL == ROW)
        return mat;
    
    if(COL > ROW){
        int offset = (COL - ROW) / 2;
        return mat(Range::all(), Range(offset, offset + ROW));
    }else{
        int offset = (ROW - COL) / 2;
        return mat(Range(offset, offset + COL), Range::all());
    }
};

uchar pixelOnePicGrayScale(Mat mat){
    Mat square = cutToSquare(mat);
    int length = mat.rows;
    long sumUp = 0;
    for(int row = 0; row < length; ++row){
        uchar* p = square.ptr<uchar>(row);
        for(int col = 0; col < length; ++col)
            sumUp += (long)p[col];
    }
    return (uchar)(sumUp/(length * length));
};

string ito3s(long r){
    stringstream ss;
    string s;
    ss << r;
    ss >> s; 
    if(r < 10)
        return "00"+s;
    else if(r < 100)
        return "0"+s;
    else
        return ""+s;
}

string itoRGB(long r, long g, long b){
    return ito3s(r) + ito3s(g) + ito3s(b);
}

string pixelOnePicRGB(Mat mat){
    Mat square = cutToSquare(mat);
    //cout << "square successfuly" <<endl;
    int length = square.rows;
    long R = 0, G = 0, B = 0;
    unsigned long area = length * length;
    for(int row = 0; row < length; ++row){
        uchar* p = square.ptr<uchar>(row);
        for(int col = 0; col < length; ++col){
            R += (long)p[col*3];
            G += (long)p[col*3 + 1];
            B += (long)p[col*3 + 2];
        }
    }
    return itoRGB(R/area,G/area,B/area);
};

Mat findPic(uchar target, map<uchar, Mat> mapU){
    //0 - 255
    //what about the 0 or 255
    int diff = 0;
    while(true){
        if(target-diff >= 0){
            map<uchar, Mat>::iterator it = mapU.find(target-diff);
            if(it != mapU.end())
                return it->second;
        }
        if(target+diff <= 255){
            map<uchar, Mat>::iterator it = mapU.find(target+diff);
            if(it != mapU.end())
                return it->second;
        }
        ++diff;
    }
};


Mat assenble(Mat pixelMat, map<uchar, Mat> mapU, int smallImageLength){
    int pRows = pixelMat.rows;
    int pCols = pixelMat.cols;
    Mat mat(pRows*smallImageLength, pCols*smallImageLength, CV_8UC1);
    for(int pr = 0; pr < pRows; ++pr){
        uchar* prPtr = pixelMat.ptr<uchar>(pr);
        cout << "ROW: "<<pr<<" ";
        for(int pc = 0; pc < pCols; ++pc){
            Mat fill = findPic(prPtr[pc], mapU);
            resize(fill, fill, Size(smallImageLength, smallImageLength));
            for(int r = 0; r < smallImageLength; ++r){
                //pr * smallImageLength, pc*smallImageLength
                uchar* rPtr = mat.ptr<uchar>(pr*smallImageLength + r);
                uchar* fillPtr = fill.ptr<uchar>(r);
                for(int c = 0; c < smallImageLength; ++c)
                    rPtr[pc*smallImageLength + c] = fillPtr[c];
            }
        }
    }
    cout <<endl;
    return mat;
};


vector<string> findSurface(int arg0, int arg1, int arg2, int diff){
    vector<string> points;
    if(diff == 0){
        points.push_back(""+arg0+arg1+arg2);
        return points;
    }
    for(int i = 0; i < diff;++i)
        for(int j = 0; i + j < diff; ++j){
            int k = diff - i - j;
            //arg0 +- i, arg1 +- j, arg2 +- k
            vector<string> arg0V;
            vector<string> arg1V;
            vector<string> arg2V;
            if(arg0 + i <= 255)
                arg0V.push_back(ito3s(arg0 + i));
            if(arg0 - i >= 0)
                arg0V.push_back(ito3s(arg0 - i));
            if(arg1 + j <= 255)
                arg1V.push_back(ito3s(arg1 + j));
            if(arg1 - j >= 0)
                arg1V.push_back(ito3s(arg1 - j));
            if(arg2 + k <= 255)
                arg2V.push_back(ito3s(arg2 + k));
            if(arg2 - k >= 0)
                arg2V.push_back(ito3s(arg2 - k));
            for(int ti = 0; ti < arg0V.size(); ++ ti)
                for(int tj = 0; tj < arg1V.size(); ++tj)
                    for(int tk = 0; tk < arg2V.size(); ++tk){
                        points.push_back(arg0V[ti]+arg1V[tj]+arg2V[tk]);
                    }
        }
    return points;
}

Mat findPic(string target, map<string, Mat> mapS){
    //3d cube
    int diff = 0;
    int R = atoi(target.substr(0,3).c_str());
    int G = atoi(target.substr(3,3).c_str());
    int B = atoi(target.substr(6,3).c_str());
    //cout << R <<G<<B<<endl;
    vector<string> candi;
    while(true){
        vector<string> points = findSurface(R, G, B, diff);
        candi.clear();
        bool flag = false;
        for(int i = 0; i < points.size(); ++i){
            map<string, Mat>::iterator it = mapS.find(points[i]);
            if(it != mapS.end()){
                candi.push_back(points[i]);
                flag = true;
            }
        }
        if(flag) break;
        ++diff;
    }
    //cout <<"find a pic" <<endl;
    int l = 0, r = candi.size();

    return mapS[candi[rand() % (r-l)]];
    //return mapS[candi[candi.size()/2]];
};

Mat assenble(Mat pixelMat, map<string, Mat> mapS, int smallImageLength){
    int ROWS = pixelMat.rows * smallImageLength;
    int COLS = pixelMat.cols * smallImageLength;
    Mat result(ROWS, COLS, CV_8UC3);
    for(int pr = 0; pr < pixelMat.rows; ++pr){
        cout << "ROW: "<<pr<<" " << flush;
        uchar* prPtr = pixelMat.ptr<uchar>(pr);
        for(int pc = 0; pc < pixelMat.cols; ++pc){
            string rgb = itoRGB(prPtr[pc*3], prPtr[pc*3 + 1], prPtr[pc*3 + 1]);
            Mat fill = findPic(rgb, mapS);
            resize(fill, fill, Size(smallImageLength, smallImageLength));
            //fill[i][j + x] result[pr*smallImageLength][pc*smallImageLength*3 +j*3 + x]
            for(int i = 0; i < smallImageLength; ++i){
                uchar* fillPtr = fill.ptr<uchar>(i);
                uchar* resultPtr = result.ptr<uchar>(pr*smallImageLength + i);
                for(int j = 0; j < smallImageLength; ++j)
                    for(int k = 0; k < 3; ++k)
                        resultPtr[pc*smallImageLength*3 + j*3 +k] = fillPtr[j*3 + k];
            }
        }
    }
    cout << endl;
    return result;
};