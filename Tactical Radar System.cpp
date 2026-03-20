#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <windows.h>

using namespace cv;
using namespace std;

int trackRed = 1, trackBlue = 1, trackYellow = 1;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) return -1;

    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);

    namedWindow("Tactical Radar System", WINDOW_NORMAL);
    namedWindow("CONTROLS", WINDOW_AUTOSIZE);
    createTrackbar("RED", "CONTROLS", &trackRed, 1);
    createTrackbar("BLUE", "CONTROLS", &trackBlue, 1);
    createTrackbar("YELLOW", "CONTROLS", &trackYellow, 1);

    Scalar lowBlue(100, 150, 50), highBlue(140, 255, 255);
    Scalar lowYellow(20, 100, 100), highYellow(30, 255, 255);
    Scalar lowRed1(0, 120, 70), highRed1(10, 255, 255);
    Scalar lowRed2(170, 120, 70), highRed2(180, 255, 255);

    Mat frame, hsv, mask;
    int frameCounter = 0;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        flip(frame, frame, 1);

        int W = frame.cols;
        int H = frame.rows;
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        Mat finalMask = Mat::zeros(H, W, CV_8UC1);
        string activeColorName = "NONE";
        double maxArea = 0; 

        if (trackRed) {
            Mat m1, m2, mRed;
            inRange(hsv, lowRed1, highRed1, m1);
            inRange(hsv, lowRed2, highRed2, m2);
            mRed = m1 | m2;
            finalMask |= mRed;
        }
        if (trackBlue) {
            Mat mBlue;
            inRange(hsv, lowBlue, highBlue, mBlue);
            finalMask |= mBlue;
        }
        if (trackYellow) {
            Mat mYellow;
            inRange(hsv, lowYellow, highYellow, mYellow);
            finalMask |= mYellow;
        }

        erode(finalMask, finalMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(finalMask, finalMask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        vector<vector<Point>> contours;
        findContours(finalMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Scalar hudColor(0, 255, 0);
        int targetX = -1, targetY = -1;
        frameCounter++;

        if (!contours.empty()) {
            int maxIdx = -1;
            for (int i = 0; i < (int)contours.size(); i++) {
                double area = contourArea(contours[i]);
                if (area > maxArea) {
                    maxArea = area;
                    maxIdx = i;
                }
            }

            if (maxIdx != -1 && maxArea > 500) {
                Rect box = boundingRect(contours[maxIdx]);
                targetX = box.x + box.width / 2;
                targetY = box.y + box.height / 2;

                Vec3b hsvPixel = hsv.at<Vec3b>(targetY, targetX);
                int hue = hsvPixel[0];
                if ((hue >= 0 && hue <= 10) || hue >= 170) activeColorName = "RED";
                else if (hue >= 100 && hue <= 140) activeColorName = "BLUE";
                else if (hue >= 20 && hue <= 30) activeColorName = "YELLOW";

                rectangle(frame, box, hudColor, 1);
                line(frame, Point(targetX - 25, targetY), Point(targetX + 25, targetY), hudColor, 1);
                line(frame, Point(targetX, targetY - 25), Point(targetX, targetY + 25), hudColor, 1);
                putText(frame, "TRGT DETECTED: " + activeColorName, Point(box.x, box.y - 15), FONT_HERSHEY_PLAIN, 1.2, hudColor, 1);

                if (maxArea > 60000) {
                    if (frameCounter % 10 < 5) {
                        putText(frame, "WARNING: PROXIMITY", Point(W / 2 - 200, H / 2 + 200), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255), 3);
                    }
                    if (frameCounter % 3 == 0) Beep(2000, 50);
                }
                else {
                    if (frameCounter % 8 == 0) Beep(1200, 40);
                }
            }
        }

        line(frame, Point(W / 2 - 150, H / 2), Point(W / 2 + 150, H / 2), hudColor, 1);
        line(frame, Point(W / 2, H / 2 - 150), Point(W / 2, H / 2 + 150), hudColor, 1);

        rectangle(frame, Rect(20, 20, 280, 70), Scalar(0, 0, 0), -1);
        rectangle(frame, Rect(20, 20, 280, 70), hudColor, 1);
        putText(frame, "SYSTEM: COMBAT_MODE", Point(30, 45), FONT_HERSHEY_PLAIN, 1.2, hudColor, 1);
        putText(frame, "SENSOR: " + activeColorName, Point(30, 75), FONT_HERSHEY_PLAIN, 1, hudColor, 1);

        int mSize = 180;
        Rect mapRect(W - mSize - 40, H - mSize - 40, mSize, mSize);
        rectangle(frame, mapRect, hudColor, 1);
        circle(frame, Point(mapRect.x + mSize / 2, mapRect.y + mSize / 2), mSize / 2, hudColor, 1);

        if (targetX != -1) {
            int mx = mapRect.x + (targetX * mSize / W);
            int my = mapRect.y + (targetY * mSize / H);
           
            circle(frame, Point(mx, my), 6, (maxArea > 60000 ? Scalar(0, 0, 255) : hudColor), -1);
            line(frame, Point(mapRect.x + mSize / 2, mapRect.y + mSize / 2), Point(mx, my), hudColor, 1);
        }

        for (int i = 0; i < H; i += 6) line(frame, Point(0, i), Point(W, i), Scalar(0, 0, 0), 1);

        imshow("Tactical Radar System", frame);
        if (waitKey(30) == 27) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
    //Developed By Ali Durra//
}