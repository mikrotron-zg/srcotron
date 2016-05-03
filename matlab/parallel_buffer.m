function ret=parallel_buffer(data, sync, len,p)
    buffer=zeros(len,1);
    plot_buffer=zeros(len,1);
    data_pos=1;
    buffer_pos=1;
    buff_count=0;
    max_len=0;
    avg_len=0;
    figure
    hold on
    grid on
    while sync==0
        data_pos=pos+1
    end
    for i=data_pos:length(data)
        if buffer_pos>len||sync(i)==1
            if buffer_pos>max_len
                max_len=buffer_pos
            end
            avg_len=avg_len+buffer_pos;
            buffer_pos=1;
            plot(plot_buffer,'g')
            %plot(buffer,'b');
            buff_count=buff_count+1;
        end
        buffer(buffer_pos)=buffer(buffer_pos)*(1-p)+data(i)*p;
        plot_buffer(buffer_pos)=data(i);
        buffer_pos=buffer_pos+1;
    end
    ideal=buffer;
    avg_len=avg_len/buff_count;
    plot(ideal(1:avg_len),'r')
    ret=ideal;
end