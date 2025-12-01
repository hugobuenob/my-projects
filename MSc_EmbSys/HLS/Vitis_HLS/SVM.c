#include

struct svmlinear {
  Bias
  Beta
}

int main {

function [Prediction] = Predict_SVM(Infer_Features,SVM_Model)

    //format long;

    //Store the distance to the binary classifier corresponding to each class.
    uint8_t Classes = [];
    uint8_t Classes_Distances = [];

    //Cover all the individual binary classifiers
    for (k = 0; k<length(SVM_Model.ClassificationSVM.CodingMatrix); k++) {

        //Select the binary classifier to be used.
        svmlinear = SVM_Model.ClassificationSVM.BinaryLearners{k,1};

        //For all the features in the input set (256)
        for (i = 0; i<length(Infer_Features); i++)
            predictors(i) = (Infer_Features(i) - svmlinear.Mu(i))/svmlinear.Sigma(i);

        //SVM Parameters.
        float b = svmlinear.Bias;
        float z = predictors./svmlinear.KernelParameters.Scale;
        float w = svmlinear.Beta;

        //Apply SVM Classifier.
        float predicted = z*w + b;

        //Store the distance to the Hyperplane corresponding to each class.
        Classes_Distances = [Classes_Distances; predicted]; //concatenar

    }

    [M,I] = max(Classes_Distances);

    //The range of class labels starts in 0, the array index starts in 0.
    Prediction = I-1;

    return 0;
}
