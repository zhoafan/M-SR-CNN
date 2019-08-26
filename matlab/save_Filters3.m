caffe.reset_all();
clear; close all;

% caffe.version
folder = 'model2/';folder1 = '';

net_model = [folder1 'mynet3_edge_mat.prototxt'];
net_weights = [folder '_iter_600000.caffemodel'];
savemodelpath = [folder1 'mynet600000.mat'];


phase = 'test';
% -----------------------
net = caffe.Net(net_model, net_weights, phase);

layers = 4;
net.layer_names
%net.blobs
%% load model using mat_caffe
%net = caffe.Net(model,weights,'test');

%% reshap parameters
weights_conv = cell(layers,1);
weights2_conv = cell(layers,1);

% weights_conv3=cell(layers,1);
% 
% weights_conv3_1=cell(layers,1);
% weights_conv9=cell(layers,1);

for idx = 1 : layers
    conv_filters = net.layers(['conv' num2str(idx)]).params(1).get_data();
    [~,fsize,channel,fnum] = size(conv_filters);

    if channel == 1
        weights = single(ones(fsize^2, fnum));
    else
        weights = single(ones(channel, fsize^2, fnum));
    end
    
    for i = 1 : channel
        for j = 1 : fnum
             temp = conv_filters(:,:,i,j);
             if channel == 1
                weights(:,j) = temp(:);
             else
                weights(i,:,j) = temp(:);
             end
        end
    end

    weights_conv{idx} = weights;
    biases_conv{idx}= net.layers(['conv' num2str(idx)]).params(2).get_data();
end

for idx = 1 : 2
    conv_filters = net.layers(['conv' num2str(idx), '_1']).params(1).get_data();
    [~,fsize,channel,fnum] = size(conv_filters);

   

    if channel == 1
        weights = single(ones(fsize^2, fnum));
    else
        weights = single(ones(channel, fsize^2, fnum));
    end
    
    for i = 1 : channel
        for j = 1 : fnum
             temp = conv_filters(:,:,i,j);
             if channel == 1
                weights(:,j) = temp(:);
             else
                weights(i,:,j) = temp(:);
             end
        end
    end

    weights2_conv{idx} = weights;
    biases2_conv{idx}= net.layers(['conv',num2str(idx), '_1']).params(2).get_data();
end
--













%% save parameters
% weights_conv1 = weights_conv{1};
% weights_conv2 = weights_conv{2};
% weights_conv3 = weights_conv{3};


%save(savepath,'weights_conv1','biases_conv1','weights_conv2','biases_conv2','weights_conv3','biases_conv3');

save(savemodelpath,'weights_conv','weights2_conv','biases_conv','biases2_conv');
