function I = shave(I, border)
I = I(1+border(1):end-border(1), ...
      1+border(2):end-border(2), :, :);
  
