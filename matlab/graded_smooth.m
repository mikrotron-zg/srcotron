function ret=graded_smooth(data,ps)
    ret=data;
    for i=1:length(ps)
        ret=exp_smooth(ret,ps(i));
    end
end