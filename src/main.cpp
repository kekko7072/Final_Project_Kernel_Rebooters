#include <iostream>
#include <filesystem>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <flower_type.hpp>
#include <flower_image.hpp>
#include <flower_image_container.hpp>
#include <flower_template.hpp>
#include <preprocessing.hpp>
#include <template_match.hpp>
#include <matching.h>

#include "sift.h"
#include "sift_processing.h"
#include "print_stats.h"
#include "metrics.h"
#include "orb.h"
#include "orb_processing.h"

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

    // Load template images
    std::vector<FlowerTemplate> daisy_templates;
    std::vector<FlowerTemplate> dandelion_templates;
    std::vector<FlowerTemplate> rose_templates;
    std::vector<FlowerTemplate> sunflower_templates;
    std::vector<FlowerTemplate> tulip_templates;
    bool templates_loaded = loadTemplates(
        data_path,
        daisy_templates, dandelion_templates, rose_templates, sunflower_templates, tulip_templates
        );
    if (!templates_loaded)
    {
        cerr << "Error loading templates. Aborting." << endl;
        return 1;
    }
    cout << "Templates loaded successfully!" << endl;

    // Create results directory if it doesn't exist
    fs::path output_dir = "../results";

    if (!fs::exists(output_dir)) {
        fs::create_directory(output_dir);
    }


    // Processing - SIFT --> Marco
    sift(test_images, train_healthy_images, train_diseased_images, output_dir.string());


    // Processing - SURF --> Marco
    #ifdef ENABLE_SURF
    {
        surf(test_images, train_healthy_images, train_diseased_images, output_dir.string());
    } 
    #else
        cout << "\nSURF is disabled. To enable, recompile with -DCONFIG_ENABLE_SURF=ON \n" << endl;
    #endif

    
    // Processing - ORB --> Marco
    orb(test_images, train_healthy_images, train_diseased_images, output_dir.string());
    
    // Processing - Template Matching --> Luca
    bool tm_success {false};
    template_match(
        test_images,
        daisy_templates, dandelion_templates, rose_templates, sunflower_templates, tulip_templates,
        tm_success
    );

    if (!tm_success)
    {
        cerr << "Template Matching classifier failed!\n" << endl;
    }
  
    // Processing - HOG --> Francesco
  
    // hog(test_images, train_healthy_images, train_diseased_images);
  
    // Processing - BOW --> Francesco
  
    // bow(test_images, train_healthy_images, train_diseased_images);

    return 0;
}
