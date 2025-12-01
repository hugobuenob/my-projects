///////////////////////////////////////////////////////////////////////////
//Generate the Table of Features+Label for each instance in the dataset.
///////////////////////////////////////////////////////////////////////////

//Training (Feature Extraction + KNN)
Feature_Set =[]; Label=[];

//Open the Folder with the images to be used for training.
fid = fopen('Outex-TC-00010\000\train.txt'); %train

//Read the name of a file from the txt list.
tline = fgetl(fid);
while ischar(tline)

    //Slit name of the image file and its label (000020.bmp 0)
    newStr = split(tline);

    //Compose the name of the image.
    FullName = strcat('Outex-TC-00010\images\',newStr{1});
    if isfile(FullName)

       //Read the image in I1
       [I1 map] = imread(FullName);

       //Extract Features using the LBP Algorithm.
       I1_Features = LBPFeatures_custom(I1);

       //Compare with MATLAB function.
       //I1_Features = extractLBPFeatures(I1,'Upright',false,'NumNeighbors',8,'Radius',1,'Normalization','None');
       //bar([I1_Features_manual; I1_Features]','grouped')

       //Store the features of the image in a new row in the Feature_Set matrix.
       Feature_Set = [Feature_Set; I1_Features];

       //Save the label of the image, read fromt the file.
       Label = [Label; str2num(string(newStr{2}))];

    end
        tline = fgetl(fid);
end

fclose(fid);

//Export the DataSet to train the Classifier.
T_TRAIN = table(Feature_Set,Label);
