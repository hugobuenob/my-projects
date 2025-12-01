%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Predict the class corresponding to a given image feature set using a 
%pre-trained SVM model.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Prediction] = SVM(Infer_Features,SVM_Model)

    format long;

    %%Store the distance to the binary classifier corresponding to each
    %%class.
    Classes = []; Classes_Distances = []; 

    %Cover all the individual binary classifiers 
    for k = 1:length(SVM_Model.ClassificationSVM.CodingMatrix)
        
        %Select the binary classifier to be used.
        svmlinear = SVM_Model.ClassificationSVM.BinaryLearners{k,1};

        %For all the features in the input set (256)
        for i = 1:length(Infer_Features)
            predictors(i) = (Infer_Features(i) - svmlinear.Mu(i))/svmlinear.Sigma(i);
        end

        %SVM Parameters.
        b = svmlinear.Bias;
        z = predictors./svmlinear.KernelParameters.Scale;
        w = svmlinear.Beta;

        %Apply SVM Classifier.
        predicted = z*w + b;
        
        %Store the distance to the Hyperplane corresponding to each class.
        Classes_Distances = [Classes_Distances; predicted];    
        
    end
    
    [M,I] = max(Classes_Distances);
    
    %The range of class labels starts in 0, the array index starts in 0. 
    Prediction = I-1;

