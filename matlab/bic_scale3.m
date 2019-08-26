function bic_gray=bic_scale3(im_input,name)
up_scale=3;
if size(im_input,3)>1
    im_input = rgb2ycbcr(im_input);
    [m,n,z]=size(im_input);
    im1 = im_input(:, :, 1);
    im2 = im_input(:, :, 2);
    im3 = im_input(:, :, 3);
end
%      im_gnd = modcrop(im1, up_scale);
    im_gnd = single(im1)/255;
%     im_l = imresize(im_gnd, 1/up_scale, 'bicubic');     
    im_b = imresize(im_gnd, up_scale, 'bicubic');
    bic_gray = uint8(im_b * 255);
%      figure, imshow(im_b);
%   imwrite(im_b,strcat('F:\m-SRDCNN\tiduchogngou\figure5\bic_gray\',ims(i).name))
%%%%%%%%
% im_b = shave(uint8(im_b * 255), [up_scale, up_scale]);
   
%     imwrite(im_b,[folder,'bichui',num2str(i),'.bmp']);
%     imwrite(im_b,[folder1,'20170517-1_Plot',num2str(i),'.bmp']);
    imwrite(bic_gray,strcat('...\Demo\example_data\test_data\bic_gray\',name))
    [m,n]=size(bic_gray);
    im2=imresize(im2,[m,n]);
    im3=imresize(im3,[m,n]);
    M(:, :, 1)=bic_gray;
    M(:, :, 2)=im2;
    M(:, :, 3)=im3;

    im = ycbcr2rgb(M);
%     imwrite(im,[folder1,'biccai',num2str(i),'.bmp']);
    figure, imshow(im);
     imwrite(im,strcat('...\Demo\example_data\test_data\bic\',name))
 %imwrite(im,strcat('F:\m-SRDCNN\tiduchogngou\GABA1\result.bmp'))
     clear M
end