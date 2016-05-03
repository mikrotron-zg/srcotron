function ret=exp_smooth(data,p)
    ret(1)=data(1);
    for i=2:length(data)
        ret(i)=(1-p)*ret(i-1)+p*data(i);
    end
end