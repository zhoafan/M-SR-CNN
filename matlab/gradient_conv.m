 function gradient=gradient_conv(im_gray)
   
    
    wx=[1,-1];
    wy=[1,-1]';
%% work on illuminance only
% if size(im_gray,3)>1
%     im = rgb2ycbcr(im_gray);
% 
%     I = im(:, :, 1);
% end
% figure(1), imshow(im1); 

% im=gradient(double(im));
image=im2double(im_gray);
%imwrite(image,strcat('F:\m-SRDCNN\tiduchogngou\5\neurons\gray\',ims(i).name));
%  imwrite(im1,strcat('E:\2017\CNN\image super_resolution\tiduchogngou\contents_grapic\',ims(i).name))
image_gradx=imfilter(double(image),wx,'conv','replicate');
image_grady=imfilter(double(image),wy,'conv','replicate');
G1=sqrt(image_gradx.^2+image_grady.^2);
 G1=double(G1*255);
%J=imadjust(G1,[0.004,1],[0.5,1],0.3);
A=max(max(G1));
 G1=G1/A;
gradient=uint8(G1*255);
% figure(2),imshow(gradient);
% imwrite(Image1,strcat('E:\2017\CNN\image super_resolution\tiduchogngou\MSI_50quannao\90\','1.1.bmp'))
% % imwrite(Image1,[folder,'gradient',num2str(i),'.bmp']);
% imwrite(gradient,strcat('F:\m-SRDCNN\tiduchogngou\figure5\example_data\test\gradient\',name))
end