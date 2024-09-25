#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;


int main() {

    Mat original_image = imread("C:/6. Semestar/Završni rad/Pictures/licenseplate.jpg");

    if (original_image.empty()) {
        cout << "Could not open or find the image!" << endl;
        return -1;
    }

    Mat gray_image;
    cvtColor(original_image, gray_image, COLOR_BGR2GRAY);
    
    Mat blurred_image;
    GaussianBlur(gray_image, blurred_image, Size(9,9), 1.6);

    Mat sobelx, sobely;
    Sobel(blurred_image, sobelx, CV_32F, 1, 0);
    Sobel(blurred_image, sobely, CV_32F, 0, 1);

    Mat sobx, soby;
    convertScaleAbs(sobelx, sobx);
    convertScaleAbs(sobely, soby);

    Mat gradient;
    addWeighted(sobx, 0.5, soby, 0.5, 0, gradient);

    Mat edges_image;
    Canny(blurred_image, edges_image, 15, 20);



    Mat blurred_image2;
    GaussianBlur(gray_image, blurred_image2, Size(13, 13), 1.7);
    Mat edges_image2;
    Canny(blurred_image2, edges_image2, 50, 90);



    Mat blurred_image3;
    GaussianBlur(gray_image, blurred_image3, Size(19, 19), 3.8);
    Mat edges_image3;
    Canny(blurred_image3, edges_image3, 20, 30);



    Mat edges_image4;
    Canny(gray_image, edges_image4, 15, 20);
    



    namedWindow("Original Image", WINDOW_NORMAL);
    resizeWindow("Original Image", 700, 350);
    imshow("Original Image", original_image);

    namedWindow("Gray Image", WINDOW_NORMAL);
    resizeWindow("Gray Image", 700, 350);
    imshow("Gray Image", gray_image);

    namedWindow("Blurred Image", WINDOW_NORMAL);
    resizeWindow("Blurred Image", 700, 350);
    imshow("Blurred Image", blurred_image);

    namedWindow("Sobelx Image", WINDOW_NORMAL);
    resizeWindow("Sobelx Image", 700, 350);
    imshow("Sobelx Image", sobx);

    namedWindow("Sobely Image", WINDOW_NORMAL);
    resizeWindow("Sobely Image", 700, 350);
    imshow("Sobely Image", soby);

    namedWindow("Grad Image", WINDOW_NORMAL);
    resizeWindow("Grad Image", 700, 350);
    imshow("Grad Image", gradient);

    namedWindow("Edge Detected Image", WINDOW_NORMAL);
    resizeWindow("Edge Detected Image", 700, 350);
    imshow("Edge Detected Image", edges_image);
    
    namedWindow("Blurred Image2", WINDOW_NORMAL);
    resizeWindow("Blurred Image2", 700, 350);
    imshow("Blurred Image2", blurred_image2);

    namedWindow("Edge Detected Image2", WINDOW_NORMAL);
    resizeWindow("Edge Detected Image2", 700, 350);
    imshow("Edge Detected Image2", edges_image2);

    namedWindow("Edge Detected Image3", WINDOW_NORMAL);
    resizeWindow("Edge Detected Image3", 700, 350);
    imshow("Edge Detected Image3", edges_image3);
    

    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Original_image.jpg", original_image);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Gray_image.jpg", gray_image);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Blurred_image.jpg", blurred_image);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Sobelx.jpg", sobx);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Sobely.jpg", soby);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Gradient.jpg", gradient);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Edges_image.jpg", edges_image);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Blurred_image2.jpg", blurred_image2);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Edges_image2.jpg", edges_image2);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Blurred_image3.jpg", blurred_image3);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Edges_image3.jpg", edges_image3);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Edges_image4.jpg", edges_image4);



    Mat compa, comp_statsa, comp_centroidsa;
    int num_compa = connectedComponentsWithStats(edges_image, compa, comp_statsa, comp_centroidsa, 8, CV_16U);
    cout << num_compa << endl;


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    int kernel_size = 3;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(kernel_size, kernel_size));

    Mat dilated_edges;
    dilate(edges_image2, dilated_edges, kernel);

    
    namedWindow("O Image", WINDOW_NORMAL);
    resizeWindow("O Image", 700, 350);
    imshow("O Image", dilated_edges);

    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Dillated_image2.jpg", dilated_edges);
    
    
    Mat comp, comp_stats, comp_centroids;
    int num_comp = connectedComponentsWithStats(dilated_edges, comp, comp_stats, comp_centroids, 8, CV_16U);
    cout << num_comp << endl;



    int size = 250;  

    Mat filteredImage = Mat::zeros(dilated_edges.size(), CV_8UC1);

    for (int i = 1; i < num_comp; i++) {  
        int area = comp_stats.at<int>(i, CC_STAT_AREA);

        if (area >= size) {
            filteredImage.setTo(255, comp == i);
        }
    }


    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/After_cc_image2.jpg", filteredImage);
    namedWindow("C Image", WINDOW_NORMAL);
    resizeWindow("C Image", 700, 350);
    imshow("C Image", filteredImage);

    Mat comp11, comp_stats11, comp_centroids11;
    int num_comp11 = connectedComponentsWithStats(filteredImage, comp11, comp_stats11, comp_centroids11, 8, CV_16U);
    cout << num_comp11 << endl;



    Mat coloredImage = Mat::zeros(filteredImage.size(), CV_8UC3);

    std::vector<Vec3b> colors(num_comp11);

    colors[0] = Vec3b(0, 0, 0); 

    for (int i = 1; i < num_comp11; i++) {
        colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);
    }


    for (int i = 1; i < num_comp11; i++) {  
        int area = comp_stats11.at<int>(i, CC_STAT_AREA);

        if (area >= size) {
            coloredImage.setTo(colors[i], comp11 == i);
        }
    }

    Mat newColorImage = coloredImage;



    for (int i = 1; i < num_comp11; i++) {  
        int area = comp_stats11.at<int>(i, CC_STAT_AREA);

        if (area >= size) {
            coloredImage.setTo(colors[i], comp11 == i);

            int x = static_cast<int>(comp_centroids11.at<double>(i, 0));
            int y = static_cast<int>(comp_centroids11.at<double>(i, 1));

            putText(coloredImage, std::to_string(i), cv::Point(x, y),
                FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 255, 255), 1);
        }
    }

    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Comp_color_image.jpg", coloredImage);
    namedWindow("Color Image", WINDOW_NORMAL);
    resizeWindow("Color Image", 700, 350);
    imshow("Color Image", coloredImage);


    std::vector<int> componentsToRemove = { 1,2,3,13,14,15 };

    for (int i : componentsToRemove) {
        newColorImage.setTo(Vec3b(0, 0, 0), comp11 == i);
    }

    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Final_image.jpg", newColorImage);
    namedWindow("Color2 Image", WINDOW_NORMAL);
    resizeWindow("Color2 Image", 700, 350);
    imshow("Color2 Image", newColorImage);
    


   
    Mat final_image;
    threshold(newColorImage, final_image, 1, 255, THRESH_BINARY);
    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/FFFinal_image.jpg", final_image);
    namedWindow("Final Image", WINDOW_NORMAL);
    resizeWindow("Final Image", 700, 350);
    imshow("Final Image", final_image);



    //2nd dilatation
    /*
    int kernel_size2 = 10;
    Mat kernel2 = getStructuringElement(MORPH_RECT, Size(kernel_size2, kernel_size2));
    Mat dilated_edges2nd;
    dilate(final_image, dilated_edges2nd, kernel2);
    
    namedWindow("O Image", WINDOW_NORMAL);
    resizeWindow("O Image", 700, 350);
    imshow("O Image", dilated_edges);

    imwrite("C:/6. Semestar/Završni rad/Canny slike/Regoznaka/Dillated_image2nd.jpg", dilated_edges2nd);
    */


    waitKey(0);

    return 0;
}