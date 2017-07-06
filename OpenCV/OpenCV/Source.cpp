// リンカーの入力を設定
// デバッグとリリースで入力するファイルが異なるのでこのようにしています。
#ifdef _DEBUG
#define CV_EXT "d.lib"
#else
#define CV_EXT ".lib"
#endif
#pragma comment(lib, "opencv_core320" CV_EXT)
#pragma comment(lib, "opencv_highgui320" CV_EXT)
#pragma comment(lib, "opencv_imgcodecs320" CV_EXT)
#pragma comment(lib, "opencv_imgproc320" CV_EXT)

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main(int argc, char *argv[])
{
    struct Window {
        int w = 10;
        int h = 10;
    };

    struct Window wnd;

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
        }
    }

    cv::Mat img = cv::imread(std::string("img.png"), 1);
    std::cout << img.at<cv::Vec3b>(0,0) << std::endl;
    for (int i = 0; i < (img.rows - wnd.h); i++) {
        for (int j = 0; j < (img.cols - wnd.w); j++) {
            std::string str("./img/" + std::to_string(i) + "x" + std::to_string(j) + ".ppm");

            cv::Mat wnd_img(img, cv::Rect(j, i, wnd.w, wnd.h));
            cv::imwrite(str, wnd_img);
            wnd_img.release();
        }
    }



    cv::namedWindow("img", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
    cv::imshow("img", img);
    cv::waitKey(0);

}