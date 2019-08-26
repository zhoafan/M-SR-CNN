% folder='xiaoshu/yuanbic/';
% folder1='xiaoshu/srhui/';
% % folder2='1/';
% 
ims=dir('...\Demo\example_data\test_data\bic\*.bmp');%读取文件夹abc的位置
imt=dir('...\Demo\example_data\test_data\sr_result\*.bmp');
filenames={ims.name}';
filenames2={imt.name}';
% % xlswrite('testz3',filenames,'A1:A22');
file_length=length(ims);%这里得到的结果应该为1000
% up_scale=3;
for i = 1:file_length
    im1 = imread(['...\Demo\example_data\test_data\sr_result\',imt(i).name]);
    im = imread(['...\Demo\example_data\test_data\bic\',ims(i).name]);
    figure, imshow(im);

    if size(im,3)>1
        im = rgb2ycbcr(im);
%         [m,n,z]=size(im);
        im0 = im(:, :, 1);
        im2 = im(:, :, 2);
        im3 = im(:, :, 3);
    end
%     im1(im1>120)=80;
%      im1(im1<2)=0;
%       im1(10>im1>1)=12;
%   [m,n]=size(im1);

% im1=im1+40;
%     im2=imresize(im2,[128 128],'bilinear');
%     im3=imresize(im3,[128 128],'bilinear');
%     im1=double(im1);
%     psnr_srcnn = compute_psnr(im0,im1);
    H(:, :, 1)=im1;
    H(:, :, 2)=im2;
    H(:, :, 3)=im3;

    I = ycbcr2rgb(H);
    % I = im2double(I);res = RollingGuidanceFilter(I,5,0.1,4); % run this if there exist
    % noise in the image
    figure, imshow(I);
%     imwrite(I,strcat('F:\m-SRDCNN\tiduchogngou\GABA1\sresults1\','demo45.bmp'))
    imwrite(I,strcat('...\Demo\example_data\test_data\HR\',ims(i).name))
    
    clear H
    
    
 end