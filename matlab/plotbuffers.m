figure
hold on
grid on
for i=0:0.01:0.1
    s=parallel_buffer(data(1,:),data(4,:),300,i);
    plot(s);
end