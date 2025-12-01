%--------------------SAVING DATA -------------------------------
clear all;
addpath(genpath(pwd));
%Load the Trained Model
load('Trained_SVM_Model.mat');

parameters = zeros(length(Model_SVM_Custom.ClassificationSVM.CodingMatrix),2);
BinaryLearner  = zeros(length(Model_SVM_Custom.ClassificationSVM.CodingMatrix),2);
Beta = zeros(length(Model_SVM_Custom.ClassificationSVM.CodingMatrix),256); 

%Cover all the individual binary classifiers 
for k = 1:length(Model_SVM_Custom.ClassificationSVM.CodingMatrix)

    svmlinear = Model_SVM_Custom.ClassificationSVM.BinaryLearners{k,1};
    
    parameters(k,:) = [svmlinear.KernelParameters.Scale svmlinear.Bias];
    Beta(k,:) = [svmlinear.Beta'];
    BinaryLearner(k,:) = [svmlinear.Mu(k) svmlinear.Sigma(k)];
    
end

%data characteristics
data_type='double';       % -> data type used
data_arch='n';            % -> architecture type (n or native, little endian, big endian)

% parameters
archivo = fopen('parameters.bin','w');
fwrite(archivo,parameters, data_type,data_arch);
fclose(archivo);

% parameters
archivo = fopen('Beta.bin','w');
fwrite(archivo,Beta, data_type,data_arch);
fclose(archivo);

% parameters
archivo = fopen('BinaryLearner.bin','w');
fwrite(archivo,BinaryLearner, data_type,data_arch);
fclose(archivo);


features(4319,:) = zeros(1,256);

for Nimg = 0:004319
    nombreArchivo = sprintf('%06d.bmp', Nimg);
    ruta = 'C:\Users\ticro\Documents\MATLAB\HLS\MATLAB\Outex-TC-00010\images\';
    [I map] = imread(fullfile(ruta, nombreArchivo));
    

    features(Nimg + 1,:) = LBP(I);  

    prediction(Nimg + 1) = SVM(features(Nimg + 1,:), Model_SVM_Custom);
end
%%
fid = fopen("data.txt", 'w');

fprintf(fid, 'float scale[24] = {\n');

for K = 1:size(Model_SVM_Custom.ClassificationSVM.BinaryLearners, 1)
    svmlinear = Model_SVM_Custom.ClassificationSVM.BinaryLearners{K,1};
    fprintf(fid, '{');
    for i = 1:256
        fprintf(fid, '%.7f, ', svmlinear.Sigma(i));
    end
    fprintf(fid, '},\n');
end

fprintf(fid, '};\n');
fclose(fid);
