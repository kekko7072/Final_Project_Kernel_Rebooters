#include <iostream>
#include <filesystem>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
    // Preprocessing --> Luca
    import_images();          // Import the test images of the chosen flower type
    extract_descriptors();    // Extract global and local descriptor from the test image

    // Processing
    for each test_image :
        sift(); // Marco          // Compare the feature of the test image with the ones from the train folder using SIFT
        surf(); // Marco          // Compare the feature of the test image with the ones from the train folder using SURF
        tm();   // Luca           // Compare the feature of the test image with the ones from the train folder using template matching
        hog();  // Francesco      // Compare the feature of the test image with the ones from the train folder using Histogram Orientation Gradient
        bow();  // Francesco      // Compare the feature of the test image with the ones from the train folder using Bag Of Words
        is_health();              // Check if the test flower is healthy or not (maybe looking at the colors of the flower?)

    // Performance measurement --> Marco
    print_output();           // Display the image with a bounding box and a similarity score near it
    total_acc();              // Calculate the entire test set to quantify the overall robustness of the system: Number of images correctly predicted / number of test images     
    category_acc();           // Percentage of correct classification for each of the 5 categories: Number of images correctly predicted for a category / number of test images of that category
    mean_processing_time();   // Average time required to classify an input image (maybe with pipobarra while processing the processing?)

    return 0;
}
