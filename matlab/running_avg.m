function ret=running_avg(data)
    ret(1)=data(1);
    ret(2)=0.6*data(1)+0.4*data(2);
    ret(3)=0.3*data(1)+0.3*data(2)+0.4*data(3);
    for i=4:length(data)
        ret(i)=0.15*data(i)+0.15*data(i-1)+0.15*data(i-2)+0.4*data(i-3);
    end
end