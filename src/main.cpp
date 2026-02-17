#include <iostream>
#include <filesystem>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <flower_type.hpp>
#include <flower_image.hpp>
#include <flower_image_container.hpp>
#include <preprocessing.hpp>
#include <matching.h>

#include "sift.h"
#include "sift_processing.h"
#include "print_stats.h"
#include "metrics.h"

#ifdef ENABLE_SURF
#include "surf.h"
#include "surf_processing.h"
#endif

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
    // Preprocessing --> Luca

    const std::string parser_keys {
        "{help h ? | | print this message}"
        "{@path    | | path of the train/test dataset}"
    };
    cv::CommandLineParser parser {argc, argv, parser_keys};
    const std::string about_text {"flower_detector 0.1"};
    parser.about(about_text);

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    std::string data_path_str = parser.get<std::string>("@path");
    if (data_path_str.empty())
    {
        cout << "No path to specified. Using default ('../Final_project_proposal/')" << endl;
        data_path_str = "../Final_project_proposal/";
    }

    // Check that path exists and that it contains train/test datasets
    fs::path data_path {data_path_str};
    bool valid_path {false};
    bool test_dir {false};
    bool train_healthy_dir {false};
    bool train_diseased_dir {false};
    if (fs::exists(data_path) && fs::is_directory(data_path))
    {
        fs::directory_iterator iterator {data_path, fs::directory_options::skip_permission_denied};
        //cout << "Found directories:" << endl;  // DEBUG
        for (const auto& dir_entry : iterator)
        {
            if (fs::is_directory(dir_entry))
            {
                const std::string s {dir_entry.path().filename()};
                //cout << s << endl;  // DEBUG
                if (s == "test_photos")
                    test_dir = true;
                if (s == "train_healthy_photos")
                    train_healthy_dir = true;
                if (s == "train_diseased_photos")
                    train_diseased_dir = true;
            }
        }
    }
    if (test_dir && train_healthy_dir && train_diseased_dir)
    {
        valid_path = true;
    }
    if (!valid_path)
    {
        CV_Error(cv::Error::StsBadArg, "Invalid path, no dataset found!");
    }

    // DEBUG
    cout << "Valid path. Loading images..." << endl;

    // Load images
    FlowerImageContainer test_images;
    FlowerImageContainer train_healthy_images;
    FlowerImageContainer train_diseased_images;

    // CV_Assert(load_images(test_images, train_healthy_images, train_diseased_images));
    if (!loadImages(data_path, test_images, train_healthy_images, train_diseased_images))
    {
        cerr << "Error loading images. Aborting." << endl;
        return 1;
    }
    cout << "Images loaded successfully!" << endl;

    // class_names = {"Daisy", "Dandelion", "Rose", "Sunflower", "Tulip", "NoFlower"}

    // Processing - SIFT --> Marco

    SIFTExtractor sift;

    Metrics sift_metrics = createMetrics(6);

    std::map<FlowerType, cv::Mat> sift_train_descriptors;
    
    // Train SIFT
    trainSIFT(train_healthy_images, train_diseased_images, sift, sift_train_descriptors, class_names, true);
    
    // Test SIFT
    double sift_threshold = 2.0;  // Can be tuned (higher = more matches, lower = stricter)
    testSIFT(test_images, sift_train_descriptors, sift, sift_metrics, class_names, sift_threshold, true);
    
    // Display results
    printClassificationReport(sift_metrics, class_names, "SIFT");


    // Processing - SURF --> Marco
    
    #ifdef ENABLE_SURF
    {
        cout << "\n\n====================\n" << endl;
        
        SURFExtractor surf;

        Metrics surf_metrics = createMetrics(6);

        std::map<FlowerType, cv::Mat> surf_train_descriptors;

        // Train SURF
        trainSURF(train_healthy_images, train_diseased_images, surf, surf_train_descriptors, class_names, true);
        
        // Test SURF
        double surf_threshold = 2.5;  // Can be tuned (higher = stricter, lower = more matches)
        testSURF(test_images, surf_train_descriptors, surf, surf_metrics, class_names, surf_threshold, true);
        
        // Display results
        printClassificationReport(surf_metrics, class_names, "SURF");
    } 
    #else
        cout << "\nSURF is disabled. To enable, recompile with -DCONFIG_ENABLE_SURF=ON \n" << endl;
    #endif
    
  
    // Processing - HOG --> Francesco
  
    // hog(test_images, train_healthy_images, train_diseased_images);
  
    // Processing - BOW --> Francesco
  
    // bow(test_images, train_healthy_images, train_diseased_images);

    return 0;
}
