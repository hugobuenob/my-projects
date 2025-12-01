%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Extract the normalized Local Binary Pattern (LBP) features of the Image I.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Features] = LBP(I)

    format long;
    
    Transform = zeros(128,128);
                
    %Iterate over every pixel in the 128x128 image
    for i = 2:127
        for j = 2:127

            %Extract the neighbourhood of the pixel.
            Neighbourhood =[I(i,j+1) I(i-1,j+1) I(i-1,j) I(i-1,j-1) I(i,j-1) I(i+1,j-1) I(i+1,j) I(i+1,j+1)];

            %Transform the neighbourhood to binary values.
            for kk = 1:1:8
                if I(i,j) < Neighbourhood(kk)
                    %Compute the two's power of the transform.
                    Transform(i,j) = Transform(i,j) + 2^(kk-1);
                end
            end     
        end
    end

    %Create the histogram
    I1_Features_hist = histcounts(reshape(Transform(2:127,2:127),[1,126*126]),256);
    
    %Normalize the histogram.
    Features = I1_Features_hist ./vecnorm(I1_Features_hist,2);
    
    %Features to compare
    %I1_Features = extractLBPFeatures(I,'Upright',false,'NumNeighbors',8,'Radius',1);
    %bar([I1_Features_hist ; I1_Features]','grouped')