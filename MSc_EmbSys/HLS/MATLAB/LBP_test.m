
% 'C:\Users\ticro\Documents\MATLAB\HLS\C_translation\Outex-TC-00010\images\'
% mkdir('rawImg');

fid = fopen('features_matrix.h', 'w');
fprintf(fid, 'uint32_t features [4319][256] = {\n');
for Nimg = 0:004319
    nombreArchivo = sprintf('%06d.bmp', Nimg);
    ruta = 'C:\\Users\\ticro\\Documents\\MATLAB\\HLS\\C_translation\\Outex-TC-00010\\images\\';
    [I map] = imread(fullfile(ruta, nombreArchivo));
    

    features = LBPFeatures_custom(I);
    
    % nombreArchivo = sprintf('rawImg\\%06d.raw', Nimg);
    % fid = fopen(nombreArchivo, 'wb');
    % fwrite(fid, I', 'uint8');
    % fclose(fid);

    fprintf(fid, '{\n');
    for i = 1:length(features)
        fprintf(fid, '%f ,\n', features(i));
    end
    fprintf(fid, '}, \n');
    
    
end
fprintf(fid, '}; \n');
fclose(fid);
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Extract the normalized Local Binary Pattern (LBP) features of the Image I.
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Features] = LBPFeatures_custom(I)
    
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

    % fprintf('uint32_t features [256] = {\n');
    % for i = 1:length(Features)
    %     fprintf('%f ,\n', Features(i));
    % end
    % fprintf('}; \n');  

    %Features to compare
    %I1_Features = extractLBPFeatures(I,'Upright',false,'NumNeighbors',8,'Radius',1);
    %bar([I1_Features_hist ; I1_Features]','grouped')

end