# Final_Project_Kernel_Rebooters

## Goal
We should end up with a fully‑functional pipeline that takes a single test image,
pre‑processes it, extracts global and local descriptors, compares against
prototypical feature models and outputs the most likely flower species
identified along with a similarity score and if a flower has been identified,
determine whether it is in a healthy state.

## Project Workflow

To efficiently tackle the project, we divided the work into the following components:

- **Pre-processing:** Construct the training dataset and apply necessary image pre-processing techniques to prepare the data for modeling.
- **Model Training:** Develop and train one or more models using the processed data. This includes all code related to model architecture, training routines, and experimentation with different approaches.
- **Performance Evaluation:** Implement code to assess model performance, including metrics calculation, result analysis, and visualization of outcomes.

## Performance metrics
- Accuracy 
    - total accuracy
    - accuracy per category
- Confusion matrix
- Mean processing time

## Compile (Command Line)
Recommended (CMake):
```bash
cmake -S . -B build
cmake --build build -j4
```

Alternative (single command with OpenCV via pkg-config):
```bash
g++ -std=c++17 src/*.cpp -Iinclude $(pkg-config --cflags --libs opencv4) -o flower_classifier
g++ -std=c++17 src/*.cpp -Iinclude $(pkg-config --cflags --libs opencv) -o flower_classifier
```

## Run
```bash
./build/flower_classifier Final_project_proposal
```
