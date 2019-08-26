ims=dir('...\Demo\example_data\test_data\LR\*.bmp');%Read the folder where the low resolution image is located
filenames={ims.name}';
model = 'mynet600000.mat';%%% Trained model

file_length=length(ims);% the number of LR images
for i = 1:file_length
    im = imread(['...\Demo\example_data\test_data\LR\',ims(i).name]);
    im_gray=bic_scale3(im,ims(i).name);% Interpolation amplification
    imshow(im_gray);
    gradient=gradient_conv(im_gray);% Calculating the gradient map
    imshow(gradient);
    sr_gradient = mynet3_edge(model, gradient);% Gradient map reconstruction
    imshow(sr_gradient);
    imwrite(sr_gradient,strcat('...\Demo\example_data\test_data\sr_gradient\',ims(i).name))
end