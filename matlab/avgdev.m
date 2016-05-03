avg_dev=3.9;
for i=2:length(ch)
    avg_dev=[avg_dev avg_dev(i-1)*0.99+abs(ch(i))*0.01];
    if ch(i)>3*avg_dev(i)&&ch(i-1)<3*avg_dev(i)
        pulse(i)=1;
    else
        pulse(i)=0;
    end
end